// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <cstdint>

namespace bsp::eeprom
{
    constexpr inline auto M24256_MEM_DEVICE_ADDR = 0xA0;
    constexpr inline auto M24256_ID_DEVICE_ADDR  = 0xB0;

    constexpr inline auto M24256_PAGE_SIZE  = 64;
    constexpr inline auto M24256_TOTAL_SIZE = (32 * 1024); // bytes

} // namespace bsp::eeprom