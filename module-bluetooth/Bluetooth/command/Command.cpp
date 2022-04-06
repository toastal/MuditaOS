// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Command.hpp"

namespace bluetooth
{

    Command::Command(Command::Type type, const std::shared_ptr<CommandData> &data) : type(type)
    {
        if (data != nullptr) {
            commandData = data->clone();
        }
    }

    void Command::cleanup()
    {
        LOG_DEBUG("Command cleanup");
        delete commandData;
    }

    auto Command::getType() const noexcept -> Command::Type
    {
        return type;
    }

    auto Command::getData() -> DataVariant
    {
        if (commandData == nullptr) {
            LOG_ERROR("Terrible,terrible damage!");
            return DataVariant{};
        }
        return commandData->getData();
    }
} // namespace bluetooth
