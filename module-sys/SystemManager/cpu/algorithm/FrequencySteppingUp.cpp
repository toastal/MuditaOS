// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "FrequencySteppingUp.hpp"
#include "SystemManager/CpuGovernor.hpp"

namespace sys::cpu
{

    FrequencySteppingUp::FrequencySteppingUp(const bsp::PowerProfile &powerProfile) : powerProfile(powerProfile)
    {}

    bsp::CpuFrequencyMHz stepUp(bsp::CpuFrequencyMHz freq)
    {
        switch (freq) {
        case bsp::CpuFrequencyMHz::Level_6:
        case bsp::CpuFrequencyMHz::Level_5:
            return bsp::CpuFrequencyMHz::Level_6;
        case bsp::CpuFrequencyMHz::Level_4:
            return bsp::CpuFrequencyMHz::Level_5;
        case bsp::CpuFrequencyMHz::Level_3:
            return bsp::CpuFrequencyMHz::Level_4;
        case bsp::CpuFrequencyMHz::Level_2:
            return bsp::CpuFrequencyMHz::Level_3;
        case bsp::CpuFrequencyMHz::Level_1:
            return bsp::CpuFrequencyMHz::Level_2;
        case bsp::CpuFrequencyMHz::Level_0:
            return bsp::CpuFrequencyMHz::Level_1;
        }
        return freq;
    }

    AlgorithmResult FrequencySteppingUp::calculateImplementation(const AlgorithmData &data)
    {
        const auto load           = data.CPUload;
        const auto startFrequency = data.curentFrequency;
        const auto noChange       = [&]() { return AlgorithmResult{algorithm::Change::NoChange, startFrequency}; };

        if (startFrequency == bsp::CpuFrequencyMHz::Level_6) {
            return noChange();
        }

        if (load > powerProfile.frequencyShiftUpperThreshold) {
            aboveThresholdCounter++;
            if (aboveThresholdCounter >= powerProfile.maxAboveThresholdCount) {
                reset();
                if (startFrequency < bsp::CpuFrequencyMHz::Level_4) {
                    return {algorithm::Change::UpScaled, stepUp(startFrequency)};
                }
                else {
                    return {algorithm::Change::UpScaled, bsp::CpuFrequencyMHz::Level_6};
                }
            }
        }

        return noChange();
    }

    void FrequencySteppingUp::resetImplementation()
    {
        aboveThresholdCounter = 0;
    }
} // namespace sys::cpu
