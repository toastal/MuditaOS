// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once
#include <apps-common/widgets/BellSideListItem.hpp>
//#include <apps-common/widgets/spinners/Spinners.hpp>
#include <Item.hpp>

namespace gui
{
    class Label;

    class LayoutClassicWithTempListItem : public BellSideListItem
    {
        gui::Item *layout;

      public:
        LayoutClassicWithTempListItem();
    };
} // namespace gui