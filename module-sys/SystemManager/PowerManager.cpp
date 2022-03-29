// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "SystemManager/cpu/algorithm/FrequencyDown.hpp"
#include "SystemManager/cpu/algorithm/FrequencyHold.hpp"
#include "SystemManager/cpu/algorithm/ImmediateUpscale.hpp"
#include "SystemManager/cpu/algorithm/FrequencySteppingUp.hpp"
#include "SystemManager/cpu/algorithm/FrequencySteppingDown.hpp"
#include "cpu/AlgorithmFactory.hpp"
#include "magic_enum.hpp"
#include <SystemManager/CpuStatistics.hpp>
#include <SystemManager/PowerManager.hpp>
#include <gsl/util>
#include <log/log.hpp>

namespace sys
{
    PowerManager::PowerManager(CpuStatistics &stats) : powerProfile{bsp::getPowerProfile()}, cpuStatistics(stats)
    {
        driverSEMC      = drivers::DriverSEMC::Create(drivers::name::ExternalRAM);
        lowPowerControl = bsp::LowPowerMode::Create().value_or(nullptr);
        cpuGovernor     = std::make_unique<CpuGovernor>();

        cpuAlgorithms = std::make_unique<cpu::AlgorithmFactory>();
        cpuAlgorithms->emplace(sys::cpu::AlgoID::ImmediateUpscale, std::make_unique<sys::cpu::ImmediateUpscale>());
        cpuAlgorithms->emplace(sys::cpu::AlgoID::FrequencySteppingUp,
                               std::make_unique<sys::cpu::FrequencySteppingUp>(powerProfile));
        cpuAlgorithms->emplace(sys::cpu::AlgoID::FrequencySteppingDown,
                               std::make_unique<sys::cpu::FrequencySteppingDown>(powerProfile));
    }

    PowerManager::~PowerManager()
    {}

    int32_t PowerManager::PowerOff()
    {
        return lowPowerControl->PowerOff();
    }

    int32_t PowerManager::Reboot()
    {
        return lowPowerControl->Reboot(bsp::LowPowerMode::RebootType::NormalRestart);
    }

    int32_t PowerManager::RebootToUsbMscMode()
    {
        return lowPowerControl->Reboot(bsp::LowPowerMode::RebootType::GoToUsbMscMode);
    }

    int32_t PowerManager::RebootToUpdate(UpdateReason reason)
    {
        switch (reason) {
        case UpdateReason::FactoryReset:
            return lowPowerControl->Reboot(bsp::LowPowerMode::RebootType::GoToUpdaterFactoryReset);
        case UpdateReason::Recovery:
            return lowPowerControl->Reboot(bsp::LowPowerMode::RebootType::GoToUpdaterRecovery);
        case UpdateReason::Update:
            return lowPowerControl->Reboot(bsp::LowPowerMode::RebootType::GoToUpdaterUpdate);
        default:
            return -1;
        }
    }

    [[nodiscard]] cpu::UpdateResult PowerManager::UpdateCpuFrequency(int cause)
    {
        uint32_t cpuLoad = cpuStatistics.GetPercentageCpuLoad();
        cpu::UpdateResult retval;
        cpu::AlgorithmData data{
            cpuLoad, lowPowerControl->GetCurrentFrequencyLevel(), cpuGovernor->GetMinimumFrequencyRequested()};

        auto algorithms = {
            sys::cpu::AlgoID::FrequencyHold,
            sys::cpu::AlgoID::FrequencySteppingUp,
            sys::cpu::AlgoID::ImmediateUpscale,
            sys::cpu::AlgoID::FrequencySteppingDown,
        };

        auto result    = cpuAlgorithms->calculate(algorithms, data, &retval.id);
        if (result.change == cpu::algorithm::Change::NoChange or result.change == cpu::algorithm::Change::Hold) {
            retval.frequencySet = lowPowerControl->GetCurrentFrequencyLevel();
            retval.data         = data.sentinel;
            return retval;
        }
        retval.changed = result.change;
        SetCpuFrequency(result.value);
        LOG_INFO("=> algorithm: %s : %d : %d %%cpu cause: %s",
                 magic_enum::enum_name(retval.id).data(),
                 int(result.value),
                 int(cpuLoad),
                 cause == 1   ? "timer"
                 : cause == 2 ? "up"
                 : cause == 3 ? "down"
                              : "lol");
        cpuAlgorithms->reset(algorithms);
        retval.frequencySet = lowPowerControl->GetCurrentFrequencyLevel();
        retval.data         = data.sentinel;
        return retval;
    }

    void PowerManager::RegisterNewSentinel(std::shared_ptr<CpuSentinel> newSentinel) const
    {
        if (cpuGovernor->RegisterNewSentinel(newSentinel)) {
            newSentinel->ReadRegistrationData(lowPowerControl->GetCurrentFrequencyLevel());
        }
    }

    void PowerManager::RemoveSentinel(std::string sentinelName) const
    {
        cpuGovernor->RemoveSentinel(sentinelName);
    }

    void PowerManager::SetCpuFrequencyRequest(const std::string &sentinelName, bsp::CpuFrequencyMHz request)
    {
        cpuGovernor->SetCpuFrequencyRequest(sentinelName, request);
        auto ret = UpdateCpuFrequency(2);
        cpuStatistics.TrackChange(ret);
    }

    void PowerManager::ResetCpuFrequencyRequest(const std::string &sentinelName)
    {
        cpuGovernor->ResetCpuFrequencyRequest(sentinelName);
        auto ret = UpdateCpuFrequency(3);
        cpuStatistics.TrackChange(ret);
    }

    bool PowerManager::IsCpuPernamentFrequency()
    {
        return cpuAlgorithms->get(sys::cpu::AlgoID::FrequencyHold) != nullptr;
    }

    void PowerManager::SetPernamentFrequency(bsp::CpuFrequencyMHz freq)
    {
        cpuAlgorithms->emplace(sys::cpu::AlgoID::FrequencyHold,
                               std::make_unique<sys::cpu::FrequencyHold>(freq, powerProfile));
    }

    void PowerManager::ResetPernamentFrequency()
    {
        cpuAlgorithms->remove(sys::cpu::AlgoID::FrequencyHold);
    }

    void PowerManager::SetCpuFrequency(bsp::CpuFrequencyMHz freq)
    {
        while (lowPowerControl->GetCurrentFrequencyLevel() != freq) {
            lowPowerControl->SetCpuFrequency(freq);
            cpuGovernor->InformSentinelsAboutCpuFrequencyChange(freq);
        }
    }

    [[nodiscard]] auto PowerManager::getExternalRamDevice() const noexcept -> std::shared_ptr<devices::Device>
    {
        return driverSEMC;
    }

    void PowerManager::SetBootSuccess()
    {
        lowPowerControl->SetBootSuccess();
    }
} // namespace sys
