// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <ticks.hpp>
#include "SystemManager/CpuPrinter.hpp"
#include "SystemManager/SysCpuUpdateResult.hpp"
#include <SystemManager/SystemManagerCommon.hpp>
#include "third-party/msgpack11/msgpack11/msgpack11.hpp"
#include "battery_charger/battery_charger.hpp"
#include "time/time_conversion.hpp"

#include "third-party/segger/rtt/SEGGER_RTT.h"

// #ifndef SEGGER_RTT_Write
// #define SEGGER_RTT_Write(...)
// #endif

namespace sys::cpu::stats
{

    using namespace msgpack11;
    enum class PackID
    {
        Usage   = 1,
        ProcEnd = 2,
        Proc    = 3,
        Power   = 4,
        SysTs   = 5,
    };

    void PackPrinter::printSysUsage(struct task_prof_data *data, size_t size)
    {
        MsgPack procEnd = MsgPack::object({{"id", uint8_t(PackID::ProcEnd)}});
        auto ts         = static_cast<uint32_t>(cpp_freertos::Ticks::TicksToMs(cpp_freertos::Ticks::GetTicks()));
        vTaskSuspendAll();
        {
            for (size_t i = 0; i < size; ++i) {
                if (data[i].exec_time == 0 && data[i].switches == 0) {
                    continue;
                }
                MsgPack obj = MsgPack::object{{"name", SystemManagerCommon::ServiceProcessor(i)},
                                              {"tcb", uint16_t(data[i].task_TCB_id)},
                                              {"time", data[i].exec_time},
                                              {"id", uint8_t(PackID::Proc)},
                                              {"ts", ts}};

                SEGGER_RTT_Write(1, obj.dump().c_str(), obj.dump().size());
            }
        }
        xTaskResumeAll();
        SEGGER_RTT_Write(1, procEnd.dump().c_str(), procEnd.dump().size());
    }

    void PackPrinter::printCPUChange(const cpu::UpdateResult &ret)
    {
        MsgPack obj = MsgPack::object{
            {"id", uint8_t(PackID::Usage)},
            {"freq", uint32_t(ret.frequencySet)},
            {"name", ret.data.name},
            {"reason", ret.data.reason},
            {"requested", uint32_t(ret.data.frequency)},
            {"avgA", int32_t(bsp::battery_charger::getAvgCurrent())},
            {"nowA", int32_t(bsp::battery_charger::getCurrentMeasurement())},
            {"ts", static_cast<uint32_t>(cpp_freertos::Ticks::TicksToMs(cpp_freertos::Ticks::GetTicks()))}};

        SEGGER_RTT_Write(1, obj.dump().c_str(), obj.dump().size());
        //        if ( !sysTsSent ) {
        //            sysTsSent = true;
        //            MsgPack obj = MsgPack::object{{"id", uint8_t(PackID::SysTs)},
        //                                          {"sts", static_cast<uint32_t>()},
        //                                          {"ts",
        //                                          static_cast<uint64_t>(utils::time::getCurrentTimestamp().getTime())}};
        //            SEGGER_RTT_Write(1, obj.dump().c_str(), obj.dump().size());
        //        }
    }

    void PackPrinter::printPowerConsumption()
    {
        MsgPack obj = MsgPack::object{
            {"id", uint8_t(PackID::Power)},
            {"avgA", int32_t(bsp::battery_charger::getAvgCurrent())},
            {"nowA", int32_t(bsp::battery_charger::getCurrentMeasurement())},
            {"ts", static_cast<uint32_t>(cpp_freertos::Ticks::TicksToMs(cpp_freertos::Ticks::GetTicks()))}};
        SEGGER_RTT_Write(1, obj.dump().c_str(), obj.dump().size());
    }
} // namespace sys::cpu::stats
