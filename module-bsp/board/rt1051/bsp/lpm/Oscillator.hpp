// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

namespace bsp
{
    void EnableExternalOscillator();
    void DisableExternalOscillator();
    [[nodiscard]] bool IsExternalOscillatorEnabled();
} // namespace bsp
