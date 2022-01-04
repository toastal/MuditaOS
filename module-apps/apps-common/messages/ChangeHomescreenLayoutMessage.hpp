// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <BaseAppMessage.hpp>

namespace gui
{
    class ChangeHomeScreenLayoutMessage : public app::AppMessage
    {
        std::string layoutName;

      public:
        ChangeHomeScreenLayoutMessage(std::string layoutName) : layoutName(std::move(layoutName))
        {}

        std::string getLayoutName()
        {
            return layoutName;
        }
    };
}; // namespace gui
