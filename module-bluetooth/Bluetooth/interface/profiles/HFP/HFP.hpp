// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Profile.hpp"
#include <interface/profiles/PhoneInterface.hpp>
#include <service-bluetooth/BluetoothMessage.hpp>
#include <btstack_run_loop.h>
#include <PhoneNumber.hpp>

namespace bluetooth
{

    class HFP : public CallProfile
    {
        // static constexpr auto CLASS_OF_DEVICE = 0x400204;
        // Service class: Telephony, Major device class: Phone, Minor device class: Cellular
      public:
        HFP();
        ~HFP() override;

        HFP(const HFP &other) = delete;
        auto operator=(const HFP &rhs) -> HFP & = delete;
        HFP(HFP &&other) noexcept;
        auto operator=(HFP &&other) noexcept -> HFP &;

        auto init() -> Error::Code override;
        void setDevice(const Devicei &device) override;
        void setOwnerService(const sys::Service *service) override;

        void connect() override;
        void disconnect() override;
        /// @brief Starts ring
        /// @return Success
        [[nodiscard]] auto startRinging() const noexcept -> Error::Code override;
        /// @brief Stops ring
        /// @return Success
        [[nodiscard]] auto stopRinging() const noexcept -> Error::Code override;
        /// @brief Initializes bluetooth audio call which is divided into two parts:
        /// - Ring stop
        /// - SCO link establishment
        /// @return Success
        [[nodiscard]] auto initializeCall() const noexcept -> Error::Code override;
        [[nodiscard]] auto terminateCall() const noexcept -> Error::Code override;
        [[nodiscard]] auto callActive() const noexcept -> Error::Code override;
        [[nodiscard]] auto callStarted(const std::string &number) const noexcept -> Error::Code override;
        [[nodiscard]] auto setIncomingCallNumber(const std::string &num) const noexcept -> Error::Code override;
        /// @brief Sets the signal strength bars data
        /// @return Success
        [[nodiscard]] auto setSignalStrength(int bars) const noexcept -> Error::Code override;
        [[nodiscard]] auto setOperatorName(const std::string_view &name) const noexcept -> Error::Code override;
        [[nodiscard]] auto setBatteryLevel(const BatteryLevel &level) const noexcept -> Error::Code override;
        [[nodiscard]] auto setNetworkRegistrationStatus(bool registered) const noexcept -> Error::Code override;
        auto setRoamingStatus(bool enabled) const noexcept -> Error::Code override;

        void setAudioDevice(std::shared_ptr<bluetooth::BluetoothAudioDevice> audioDevice) override;

      private:
        class HFPImpl;
        std::unique_ptr<HFPImpl> pimpl;
        const sys::Service *ownerService{};
    };

} // namespace bluetooth
