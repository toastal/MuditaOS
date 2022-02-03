// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "LayoutClassicWithTempListItem.hpp"
#include <common/layouts/HomeScreenLayouts.hpp>
#include <common/LanguageUtils.hpp>

namespace gui
{
    LayoutClassicWithTempListItem::LayoutClassicWithTempListItem()
        : BellSideListItem(utils::translate("app_bell_settings_layout"))
    {
        auto layoutGeneratorTemp = gui::homeScreenLayouts.at("ClassicWithTemp");
        layout                   = layoutGeneratorTemp()->getLayout();
    }

} // namespace gui