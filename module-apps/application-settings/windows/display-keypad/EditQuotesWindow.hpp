// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <application-settings/windows/BaseSettingsWindow.hpp>
#include <application-settings/models/display-keypad/QuoteGroupOptionModel.hpp>

namespace gui
{
    class CheckBoxWithLabel;

    class EditQuotesWindow : public BaseSettingsWindow
    {
      public:
        explicit EditQuotesWindow(app::ApplicationCommon *app, app::settingsInterface::WallpaperSettings *settings);

      private:
        auto buildOptionsList() -> std::list<Option> override;
        void onBeforeShow(ShowMode mode, SwitchData *data) override;

        std::unique_ptr<QuoteGroupOptionModel> optionModel;

        Item *quotes;
    };
} // namespace gui
