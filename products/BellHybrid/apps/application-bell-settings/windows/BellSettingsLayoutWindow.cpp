// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "BellSettingsLayoutWindow.hpp"
#include <common/options/OptionBellMenu.hpp>
#include <common/layouts/HomeScreenLayouts.hpp>
#include <common/windows/BellFinishedWindow.hpp>

// namespace {
//         static int layoutNr = 1;
//         static std::vector<std::string> layouts = {
//             "ClassicWithTemp",
//             "ClassicWithAmPm",
//             "ClassicWithBattery",
//         };
// }
namespace gui
{
    BellSettingsLayoutWindow::BellSettingsLayoutWindow(
        app::ApplicationCommon *app,
        std::unique_ptr<app::bell_settings::LayoutWindowPresenter::Presenter> &&presenter,
        const std::string &name)
        : AppWindow(app, name), presenter{std::move(presenter)}
    {
        this->presenter->attach(this);
        buildInterface();
    }

    void BellSettingsLayoutWindow::buildInterface()
    {
        AppWindow::buildInterface();

        statusBar->setVisible(false);
        header->setTitleVisibility(false);
        navBar->setVisible(false);

        auto layouts = presenter->getLayouts();
        spinner      = new WidgetSpinner(this, {layouts.begin(), layouts.end()}, Boundaries::Fixed);
        spinner->setSize(style::window_width, style::window_height);
        spinner->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        spinner->setFocusEdges(RectangleEdge::None);
        auto selectedLayout = presenter->getSelectedLayout();
        spinner->setCurrentValue(selectedLayout);
        //  spinner->onValueChanged = [this](const auto &) {

        //  };
        setFocusItem(spinner);
    }

    bool BellSettingsLayoutWindow::onInput(const InputEvent &inputEvent)
    {
        if (spinner->onInput(inputEvent)) {
            return true;
        }
        else if (inputEvent.isShortRelease(KeyCode::KEY_ENTER)) {
            presenter->setLayout(spinner->getCurrentValue());
            application->switchWindow(
                window::bell_finished::defaultName,
                BellFinishedWindowData::Factory::create("circle_success_big", gui::name::window::main_window));
            // auto name = layouts[(layoutNr++) % layouts.size()];
            // presenter->setLayout(name);
            return true;
        }
        return AppWindow::onInput(inputEvent);
    }

} // namespace gui