// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "SignalStrengthData.hpp"

namespace bluetooth
{
    SignalStrengthData::SignalStrengthData(const Store::SignalStrength &signalStrength) : signalStrength(signalStrength)
    {}
    auto SignalStrengthData::clone() -> CommandData *
    {
        CommandData *clone = new SignalStrengthData(*this);
        return clone;
    }
    auto SignalStrengthData::getData() -> DataVariant
    {
        return signalStrength;
    }
} // namespace bluetooth
