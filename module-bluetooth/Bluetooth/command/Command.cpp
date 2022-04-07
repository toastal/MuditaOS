// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Command.hpp"
#include <log/log.hpp>

namespace bluetooth
{

    Command::Command(CommandPack &&pack) : data(std::move(pack))
    {}

    auto Command::getType() const noexcept -> Command::Type
    {
        return data.commandType;
    }

    auto Command::getData() -> DataVariant
    {
        if (data.data == nullptr) {
            LOG_ERROR("Terrible,terrible damage!");
            return DataVariant{};
        }
        return data.data->getData();
    }
} // namespace bluetooth
