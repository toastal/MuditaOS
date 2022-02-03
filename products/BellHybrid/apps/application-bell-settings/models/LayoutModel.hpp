// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <common/models/SettingsModel.hpp>
#include <service-db/Settings.hpp>

#include <ListItemProvider.hpp>
#include <apps-common/InternalModel.hpp>

namespace gui
{
    class LayoutClassicWithTempListItem;
} // namespace gui

namespace app::bell_settings
{

    class LayoutModel : public app::InternalModel<gui::ListItem *>, public gui::ListItemProvider
    {
      public:
        // explicit LayoutModel(ApplicationCommon *app);
        // std::string getValue() const;
        // void setValue(std::string value);

        void createData();

      private:
        [[nodiscard]] auto getItem(gui::Order order) -> gui::ListItem * override;
        [[nodiscard]] auto requestRecordsCount() -> unsigned int override;
        [[nodiscard]] auto getMinimalItemSpaceRequired() const -> unsigned int override;
        void requestRecords(uint32_t offset, uint32_t limit) override;

        gui::LayoutClassicWithTempListItem *layoutClassicWithTempItem = nullptr;
    };
} // namespace app::bell_settings