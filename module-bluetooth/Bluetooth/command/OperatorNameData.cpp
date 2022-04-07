// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "OperatorNameData.hpp"

namespace bluetooth
{
    OperatorNameData::OperatorNameData(const std::string &operatorName) : operatorName(operatorName)
    {}
    auto OperatorNameData::getData() -> DataVariant
    {
        return operatorName;
    }
} // namespace bluetooth
