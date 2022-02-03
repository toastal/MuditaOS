// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/BasePresenter.hpp>
#include <apps-common/ApplicationCommon.hpp>
#include "models/LayoutModel.hpp"

#include <vector>
#include <string>
#include <Item.hpp>

namespace gui
{
    class ListItemProvider;
}

namespace app::bell_settings
{
    class LayoutModel;
    class LayoutWindowContract
    {
      public:
        class View
        {
          public:
            virtual ~View() = default;
        };

        class Presenter : public BasePresenter<LayoutWindowContract::View>
        {
          public:
            // virtual std::vector<gui::Item *> getLayouts() const = 0;
            // virtual gui::Item *getSelectedLayout() const        = 0;
            virtual auto getModel() -> std::shared_ptr<gui::ListItemProvider> = 0;
            virtual void loadData()                                           = 0;

            // virtual void setLayout(gui::Item *selectedLayout)   = 0;
        };
    };

    class LayoutWindowPresenter : public LayoutWindowContract::Presenter
    {
      private:
        app::ApplicationCommon *app{};
        settings::Settings *settings = nullptr;
        std::shared_ptr<LayoutModel> layoutModel;
        // std::vector<std::pair<gui::Item *, const std::string>> layoutOptions;
        // void initLayoutOptions();

      public:
        LayoutWindowPresenter(app::ApplicationCommon *app, settings::Settings *settings);

        // std::vector<gui::Item *> getLayouts() const override;
        // gui::Item *getSelectedLayout() const override;

        auto getModel() -> std::shared_ptr<gui::ListItemProvider> override;
        void loadData() override;

        // void setLayout(gui::Item *selectedLayout) override;
    };
} // namespace app::bell_settings
