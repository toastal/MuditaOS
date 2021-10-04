// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/BasePresenter.hpp>
#include <models/alarm_settings/AbstractPrewakeUpSettingsModel.hpp>
#include <memory>

namespace gui
{
    class ListItemProvider;
} // namespace gui

namespace app::bell_settings
{
    class PrewakeUpListItemProvider;

    class PrewakeUpWindowContract
    {
      public:
        class View
        {
          public:
            virtual ~View() noexcept = default;
            virtual void exit()      = 0;
        };

        class Presenter : public BasePresenter<View>
        {
          public:
            virtual ~Presenter() noexcept                                                   = default;
            virtual auto getPagesProvider() const -> std::shared_ptr<gui::ListItemProvider> = 0;
            virtual auto saveData() -> void                                                 = 0;
            virtual auto loadData() -> void                                                 = 0;
            virtual auto eraseProviderData() -> void                                        = 0;
        };
    };

    class PrewakeUpWindowPresenter : public PrewakeUpWindowContract::Presenter
    {
      public:
        PrewakeUpWindowPresenter(std::shared_ptr<PrewakeUpListItemProvider> provider,
                                 std::unique_ptr<AbstractPrewakeUpSettingsModel> model);

        auto getPagesProvider() const -> std::shared_ptr<gui::ListItemProvider> override;
        auto saveData() -> void override;
        auto loadData() -> void override;
        auto eraseProviderData() -> void override;

      private:
        std::shared_ptr<PrewakeUpListItemProvider> provider;
        std::unique_ptr<AbstractPrewakeUpSettingsModel> model;
    };
} // namespace app::bell_settings