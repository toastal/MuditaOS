// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <models/LayoutModel.hpp>
#include "data/LayoutClassicWithTempListItem.hpp"
#include <apps-common/ApplicationCommon.hpp>
#include <db/SystemSettings.hpp>
#include <ListView.hpp>

namespace app::bell_settings
{

    // std::string LayoutModel::getValue() const
    // {
    //     return settings.getValue(bell::settings::Layout::defaultLayout, settings::SettingsScope::Global);
    // }

    auto LayoutModel::getItem(gui::Order order) -> gui::ListItem *
    {
        return getRecord(order);
    }
    auto LayoutModel::requestRecordsCount() -> unsigned int
    {
        return internalData.size();
    }
    auto LayoutModel::getMinimalItemSpaceRequired() const -> unsigned int
    {
        return style::sidelistview::list_item::w;
    }
    void LayoutModel::requestRecords(uint32_t offset, uint32_t limit)
    {
        setupModel(offset, limit);
        list->onProviderDataUpdate();
    }

    void LayoutModel::createData()
    {
        layoutClassicWithTempItem = new gui::LayoutClassicWithTempListItem();
        internalData.push_back(layoutClassicWithTempItem);
        layoutClassicWithTempItem->deleteByList = false;
        // setValue(spinnerDefaultValue);
    }
} // namespace app::bell_settings