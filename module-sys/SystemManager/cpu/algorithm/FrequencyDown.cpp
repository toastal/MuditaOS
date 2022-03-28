// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "FrequencyDown.hpp"
#include <algorithm>
#include <list>

namespace sys::cpu
{
    FrequencyDown::FrequencyDown()
    {}

    auto decrease(const bsp::CpuFrequencyMHz &val)
    {
        switch (val) {
        case bsp::CpuFrequencyMHz::Level_0:
        case bsp::CpuFrequencyMHz::Level_1:
            return bsp::CpuFrequencyMHz::Level_0;
        case bsp::CpuFrequencyMHz::Level_2:
            return bsp::CpuFrequencyMHz::Level_1;
        case bsp::CpuFrequencyMHz::Level_3:
            return bsp::CpuFrequencyMHz::Level_2;
        case bsp::CpuFrequencyMHz::Level_4:
            return bsp::CpuFrequencyMHz::Level_3;
        case bsp::CpuFrequencyMHz::Level_5:
            return bsp::CpuFrequencyMHz::Level_4;
        case bsp::CpuFrequencyMHz::Level_6:
            return bsp::CpuFrequencyMHz::Level_5;
        }
        return bsp::CpuFrequencyMHz::Level_0;
    }

    AlgorithmResult FrequencyDown::calculateImplementation(const AlgorithmData &data)
    {
        const auto load = data.CPUload;
        if (load < 30 && data.curentFrequency > bsp::CpuFrequencyMHz::Level_0) {
            return {algorithm::Change::Downscaled, decrease(data.curentFrequency)};
        }
        return {algorithm::Change::NoChange, data.curentFrequency};
    }
}; // namespace sys::cpu
