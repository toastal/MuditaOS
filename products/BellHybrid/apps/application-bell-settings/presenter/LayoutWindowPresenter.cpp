// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "LayoutWindowPresenter.hpp"
#include <service-appmgr/Controller.hpp>
#include <common/layouts/HomeScreenLayouts.hpp>
#include <apps-common/messages/ChangeHomescreenLayoutMessage.hpp>

namespace app::bell_settings
{
    LayoutWindowPresenter::LayoutWindowPresenter(app::ApplicationCommon *app,
                                                 std::unique_ptr<AbstractLayoutModel> &&layoutModel)
        : app(app), layoutModel{std::move(layoutModel)}
    {
        initLayoutOptions();
    }

    std::vector<gui::Item *> LayoutWindowPresenter::getLayouts() const
    {
        std::vector<gui::Item *> layouts;

        for (auto const &option : layoutOptions) {
            layouts.push_back(option.first);
        }

        return layouts;
    }

    gui::Item *LayoutWindowPresenter::getSelectedLayout() const
    {
        const auto layoutSelected = layoutModel->getValue();

        for (auto const &option : layoutOptions) {
            if (option.second == layoutSelected) {
                return option.first;
            }
        }

        return layoutOptions.at(0).first;
    }

    void LayoutWindowPresenter::setLayout(gui::Item *selectedLayout)
    {
        for (auto const &option : layoutOptions) {
            if (option.first == selectedLayout) {
                auto layoutChangeRequest = std::make_shared<gui::ChangeHomeScreenLayoutMessage>(option.second);
                app->bus.sendMulticast(layoutChangeRequest, sys::BusChannel::LayoutChangeRequests);
                break;
            }
        }
        // auto layoutChangeRequest = std::make_shared<gui::ChangeHomeScreenLayoutMessage>(selectedLayout);
        // app->bus.sendMulticast(layoutChangeRequest, sys::BusChannel::LayoutChangeRequests);
    }

    void LayoutWindowPresenter::initLayoutOptions()
    {
        auto layoutGeneratorTemp = gui::homeScreenLayouts.at("ClassicWithTemp");
        auto layoutGeneratorAmPm = gui::homeScreenLayouts.at("ClassicWithAmPm");
        layoutOptions.push_back({layoutGeneratorTemp()->getLayout(), "ClassicWithTemp"});
        layoutOptions.push_back({layoutGeneratorAmPm()->getLayout(), "ClassicWithAmPm"});
    }
} // namespace app::bell_settings