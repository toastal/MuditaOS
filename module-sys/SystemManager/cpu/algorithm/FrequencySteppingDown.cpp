// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "FrequencySteppingDown.hpp"

namespace sys::cpu
{

    FrequencySteppingDown::FrequencySteppingDown(const bsp::PowerProfile &powerProfile) : powerProfile(powerProfile)
    {}

    bsp::CpuFrequencyMHz stepDown(bsp::CpuFrequencyMHz freq, const bsp::PowerProfile &profile)
    {
        switch (freq) {
        case bsp::CpuFrequencyMHz::Level_6:
            return bsp::CpuFrequencyMHz::Level_5;
        case bsp::CpuFrequencyMHz::Level_5:
            return bsp::CpuFrequencyMHz::Level_4;
        case bsp::CpuFrequencyMHz::Level_4:
            return bsp::CpuFrequencyMHz::Level_3;
        case bsp::CpuFrequencyMHz::Level_3:
            return bsp::CpuFrequencyMHz::Level_2;
        case bsp::CpuFrequencyMHz::Level_2:
            [[fallthrough]];
        case bsp::CpuFrequencyMHz::Level_1:
            [[fallthrough]];
        case bsp::CpuFrequencyMHz::Level_0:
            return profile.minimalFrequency;
        }
        return freq;
    }

    AlgorithmResult FrequencySteppingDown::calculateImplementation(const AlgorithmData &data)
    {
        const auto load           = data.CPUload;
        const auto startFrequency = data.curentFrequency;
        const auto noChange       = [&]() { return AlgorithmResult{algorithm::Change::NoChange, startFrequency}; };

        if (load < powerProfile.frequencyShiftLowerThreshold && startFrequency > powerProfile.minimalFrequency) {
            belowThresholdCounter++;

            if (belowThresholdCounter >= (isFrequencyLoweringInProgress ? powerProfile.maxBelowThresholdInRowCount
                                                                        : powerProfile.maxBelowThresholdCount)) {
                reset();
                return {algorithm::Change::Downscaled, stepDown(startFrequency, powerProfile)};
            }
        }
        return noChange();
    }

    void FrequencySteppingDown::resetImplementation()
    {
        belowThresholdCounter         = 0;
        isFrequencyLoweringInProgress = false;
    }
} // namespace sys::cpu
