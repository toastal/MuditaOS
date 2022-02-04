
// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once
#include <apps-common/widgets/BellSideListItem.hpp>
#include <common/layouts/HomeScreenLayouts.hpp>
//#include <apps-common/widgets/spinners/Spinners.hpp>
#include <Item.hpp>

namespace gui
{
    class Label;

    class LayoutClassicWithBatteryListItem : public BellSideListItem
    {
        std::unique_ptr<BaseHomeScreenLayoutProvider> layoutBattery;
        bool onInput(const InputEvent &inputEvent) override;

      public:
        LayoutClassicWithBatteryListItem();
    };
} // namespace gui
