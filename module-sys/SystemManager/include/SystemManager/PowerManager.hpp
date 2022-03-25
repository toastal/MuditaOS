// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#ifndef PUREPHONE_POWERMANAGER_HPP
#define PUREPHONE_POWERMANAGER_HPP

#include <functional>

#include "bsp/lpm/bsp_lpm.hpp"
#include "drivers/semc/DriverSEMC.hpp"
#include "SysCpuUpdateResult.hpp"
#include "CpuGovernor.hpp"
#include <bsp/lpm/PowerProfile.hpp>
#include <vector>

namespace sys::cpu
{
    class AlgorithmFactory;
}

namespace sys
{
    class CpuStatistics;

    class PowerManager
    {
      public:
        explicit PowerManager(CpuStatistics &stats);
        ~PowerManager();

        int32_t PowerOff();
        int32_t Reboot();
        int32_t RebootToUsbMscMode();
        int32_t RebootToUpdate(UpdateReason reason);

        /// called periodically to calculate the CPU requirement
        ///
        /// if for the last 'maxAboveThresholdCount' periods the current CPU consumption has been above the set upper
        /// limit (frequencyShiftUpperThreshold), CPU frequency is increased; if for the last 'maxBelowThresholdCount'
        /// periods the current CPU usage was below the lower limit (frequencyShiftLowerThreshold), CPU frequency is
        /// reduced frequency
        [[nodiscard]] cpu::UpdateResult UpdateCpuFrequency();

        [[nodiscard]] auto getExternalRamDevice() const noexcept -> std::shared_ptr<devices::Device>;

        void RegisterNewSentinel(std::shared_ptr<CpuSentinel> newSentinel) const;
        void RemoveSentinel(std::string sentinelName) const;
        void SetCpuFrequencyRequest(const std::string &sentinelName, bsp::CpuFrequencyMHz request);
        void ResetCpuFrequencyRequest(const std::string &sentinelName);
        bool IsCpuPernamentFrequency();
        void SetPernamentFrequency(bsp::CpuFrequencyMHz freq);
        void ResetPernamentFrequency();

        void SetBootSuccess();

      private:
        void ResetFrequencyShiftCounter();
        void SetCpuFrequency(bsp::CpuFrequencyMHz freq);

        TickType_t lastCpuFrequencyChangeTimestamp{0};

        std::shared_ptr<drivers::DriverSEMC> driverSEMC;
        std::unique_ptr<bsp::LowPowerMode> lowPowerControl;
        std::unique_ptr<CpuGovernor> cpuGovernor;
        const bsp::PowerProfile powerProfile;

        std::unique_ptr<sys::cpu::AlgorithmFactory> cpuAlgorithms;
        CpuStatistics &cpuStatistics;
    };

} // namespace sys

#endif // PUREPHONE_POWERMANAGER_HPP
