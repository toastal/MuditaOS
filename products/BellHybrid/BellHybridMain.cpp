﻿// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "PlatformFactory.hpp"
#include "init_prof.hpp"

// applications
#include <application-bell-alarm/ApplicationBellAlarm.hpp>
#include <application-bell-onboarding/ApplicationBellOnBoarding.hpp>
#include <application-bell-background-sounds/ApplicationBellBackgroundSounds.hpp>
#include <application-bell-bedtime/ApplicationBellBedtime.hpp>
#include <application-bell-main/ApplicationBellMain.hpp>
#include <application-bell-meditation-timer/MeditationTimer.hpp>
#include <application-bell-settings/ApplicationBellSettings.hpp>
#include <application-bell-powernap/ApplicationBellPowerNap.hpp>

// modules
#include <module-db/Databases/MultimediaFilesDB.hpp>
#include <module-db/Interface/MultimediaFilesRecord.hpp>

// services
#include <appmgr/ApplicationManager.hpp>
#include <audio/ServiceAudio.hpp>
#include <db/ServiceDB.hpp>
#include <evtmgr/EventManager.hpp>
#include <Service/ServiceCreator.hpp>
#include <service-appmgr/Constants.hpp>
#include <service-desktop/ServiceDesktop.hpp>
#include <service-eink/ServiceEink.hpp>
#include <service-gui/ServiceGUI.hpp>
#include <service-time/ServiceTime.hpp>
#include <service-fileindexer/ServiceFileIndexer.hpp>

#include <Application.hpp>
#include <ApplicationLauncher.hpp>
#include <log/log.hpp>
#include <logdump/logdump.h>
#include <Logger.hpp>
#include <product/version.hpp>
#include <sys/SystemManager.hpp>
#include <SystemWatchdog/SystemWatchdog.hpp>
#include <thread.hpp>
#include <time/AlarmOperations.hpp>
#include "alarms/include/AlarmSoundPaths.hpp"

#include <memory>
#include <vector>

#if SYSTEM_VIEW_ENABLED
#include <SEGGER/SEGGER_SYSVIEW.h>
#endif

namespace
{
    void validate_assets_path()
    {
        if (const auto ret = alarms::paths::validate(); not ret.empty()) {
            for (const auto &e : ret) {
                LOG_FATAL("%s path missing", e.c_str());
            }
            abort();
        }
    }
} // namespace

int main()
{
    constexpr auto ApplicationName = "BellHybrid";

    std::vector<std::string> fileIndexerAudioPaths = {alarms::paths::getBackgroundSoundsDir()};

#if SYSTEM_VIEW_ENABLED
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_ENTER);
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_EXIT);
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_TO_SCHEDULER);
    SEGGER_SYSVIEW_WaitForConnection();
    SEGGER_SYSVIEW_Start();
#endif

    prof::init();

    auto platformFactory = bellhybrid::PlatformFactory();
    auto platform        = platformFactory.makePlatform();

    if (!sys::SystemWatchdog::getInstance().init()) {
        LOG_ERROR("System watchdog failed to initialize");
        // wait for the hardware watchdog (initialized in reset ISR) to reset the system
        while (true)
            ;
    }

    std::vector<std::unique_ptr<sys::BaseServiceCreator>> systemServices;
    systemServices.emplace_back(sys::CreatorFor<EventManager>([]() { return dumpLogs(); }));
    systemServices.emplace_back(sys::CreatorFor<service::ServiceFileIndexer>(std::move(fileIndexerAudioPaths)));
    systemServices.emplace_back(sys::CreatorFor<ServiceDB>());
    systemServices.emplace_back(sys::CreatorFor<service::Audio>());
    systemServices.emplace_back(sys::CreatorFor<ServiceDesktop>());
    systemServices.emplace_back(sys::CreatorFor<stm::ServiceTime>(std::make_shared<alarms::AlarmOperationsFactory>()));
    systemServices.emplace_back(sys::CreatorFor<service::eink::ServiceEink>(service::eink::ExitAction::None));
    systemServices.emplace_back(
        sys::CreatorFor<service::gui::ServiceGUI>(gui::Size{BOARD_EINK_DISPLAY_RES_X, BOARD_EINK_DISPLAY_RES_Y}));

    auto sysmgr = std::make_shared<sys::SystemManager>(std::move(systemServices));
    sysmgr->StartSystem(
        [&platform]() {
            try {
                platform->init();
            }
            catch (const std::runtime_error &e) {
                LOG_FATAL("%s", e.what());
                abort();
            }

            Log::Logger::get().init(Log::Application{ApplicationName, GIT_REV, VERSION, GIT_BRANCH});
            /// force initialization of PhonenumberUtil because of its stack usage
            /// otherwise we would end up with an init race and PhonenumberUtil could
            /// be initiated in a task with stack not big enough to handle it
            i18n::phonenumbers::PhoneNumberUtil::GetInstance();

            validate_assets_path();

            return true;
        },
        [sysmgr]() {
            // vector with launchers to applications
            std::vector<std::unique_ptr<app::ApplicationLauncher>> applications;
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellMain>(app::applicationBellName, app::Closeable::False));
            applications.push_back(app::CreateLauncher<app::ApplicationBellSettings>(app::applicationBellSettingsName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellAlarm>(app::applicationBellAlarmName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellBedtime>(app::applicationBellBedtimeName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellPowerNap>(app::applicationBellPowerNapName));
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellOnBoarding>(app::applicationBellOnBoardingName));
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellBackgroundSounds>(app::applicationBellBackgroundSoundsName));
            applications.push_back(app::CreateLauncher<app::MeditationTimer>(app::MeditationTimer::defaultName));
            // start application manager
            return sysmgr->RunSystemService(
                std::make_shared<app::manager::ApplicationManager>(
                    service::name::appmgr, std::move(applications), app::applicationBellName),
                sysmgr.get());
        },
        [&platform] {
            try {
                LOG_DEBUG("System deinit");
                if (dumpLogs() != 1) {
                    LOG_ERROR("Cannot dump logs");
                }
                platform->deinit();
            }
            catch (const std::runtime_error &e) {
                LOG_FATAL("%s", e.what());
                abort();
            }
            sys::SystemWatchdog::getInstance().deinit();
            return true;
        });

    LOG_PRINTF("Launching %s \n", ApplicationName);
    LOG_PRINTF("commit: %s tag: %s branch: %s\n", GIT_REV, VERSION, GIT_BRANCH);
    cpp_freertos::Thread::StartScheduler();

    return 0;
}
