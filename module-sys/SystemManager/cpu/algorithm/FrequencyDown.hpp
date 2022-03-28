// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Algorithm.hpp"
#include "lpm/PowerProfile.hpp"

namespace sys::cpu
{
    class FrequencyDown : public Algorithm
    {
        [[nodiscard]] AlgorithmResult calculateImplementation(const AlgorithmData &data) override;

      public:
        explicit FrequencyDown();
    };
} // namespace sys::cpu
