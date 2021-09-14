// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <common/ApplicationBell.hpp>

namespace gui::window::name
{
    inline constexpr auto powernapProgress = "PowerNapProgressWindow";
    inline constexpr auto powernapSessionEnded = "PowerNapSessionEndedWindow";
}
namespace app
{
    namespace powernap
    {
        class PowerNapAlarmImpl;
    }

    inline constexpr auto applicationBellPowerNapName = "ApplicationBellPowerNap";

    class ApplicationBellPowerNap : public ApplicationBell
    {
        std::unique_ptr<powernap::PowerNapAlarmImpl> alarm;

      public:
        ApplicationBellPowerNap(std::string name                            = applicationBellPowerNapName,
                                std::string parent                          = "",
                                sys::phone_modes::PhoneMode mode            = sys::phone_modes::PhoneMode::Offline,
                                sys::bluetooth::BluetoothMode bluetoothMode = sys::bluetooth::BluetoothMode::Disabled,
                                StartInBackground startInBackground         = {false});
        ~ApplicationBellPowerNap();
        sys::ReturnCodes InitHandler() override;

        void createUserInterface() override;
        void destroyUserInterface() override
        {}

        sys::MessagePointer DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) override;

        sys::ReturnCodes SwitchPowerModeHandler(const sys::ServicePowerMode mode) override final
        {
            return sys::ReturnCodes::Success;
        }
    };

    template <> struct ManifestTraits<ApplicationBellPowerNap>
    {
        static auto GetManifest() -> manager::ApplicationManifest
        {
            return {{manager::actions::Launch}};
        }
    };
} // namespace app