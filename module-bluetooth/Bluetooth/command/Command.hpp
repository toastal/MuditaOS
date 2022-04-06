// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once
#include "CommandData.hpp"

namespace bluetooth
{
    class Command
    {
      public:
        enum Type
        {
            StartScan,
            StopScan,
            getDevicesAvailable,
            VisibilityOn,
            VisibilityOff,
            ConnectAudio,
            DisconnectAudio,
            PowerOn,
            PowerOff,
            Pair,
            Unpair,
            StartRinging,
            StopRinging,
            StartRouting,
            StartStream,
            StopStream,
            SwitchProfile,
            CallAnswered,
            IncomingCallNumber,
            SignalStrengthData,
            OperatorNameData,
            None,
        };

        /*
         *  Calling constructor with data parameter causes creation of internal copy of the data, that
         *  has to be managed outside the command - this is needed for passing the command's data via
         *  FreeRTOS queue.
         */
        Command(Command::Type type, const std::shared_ptr<CommandData> &data);
        explicit Command(Command::Type type) : type(type)
        {}
        /*
         * Be aware of calling cleanup() after command with data processing has finished, otherwise
         * a memory leak will occur.
         */
        void cleanup();
        auto getType() const noexcept -> Command::Type;
        auto getData() -> DataVariant;

      private:
        Type type;
        CommandData *commandData = nullptr;
    };

} // namespace bluetooth
