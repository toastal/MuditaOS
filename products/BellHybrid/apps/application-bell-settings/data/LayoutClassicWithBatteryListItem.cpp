// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "LayoutClassicWithBatteryListItem.hpp"
#include <common/layouts/HomeScreenLayouts.hpp>
#include <common/LanguageUtils.hpp>
#include <Text.hpp>

constexpr auto alarmTime              = 1000 * 60 * 60 * 12;
constexpr auto clockTime              = 1000 * 60 * 60 * 12;
constexpr Store::Battery batteryState = {
    .levelState = Store::Battery::LevelState::Normal,
    .state      = Store::Battery::State::Discharging,
    .level      = 100,
};

namespace gui
{
    LayoutClassicWithBatteryListItem::LayoutClassicWithBatteryListItem()
        : BellSideListItem(utils::translate("app_bell_settings_layout"))
    {
        auto layoutGeneratorBattery = gui::homeScreenLayouts.at("ClassicWithBattery");
        layoutBattery               = std::move(layoutGeneratorBattery());
        layoutBattery->setAlarmEdit(false);
        layoutBattery->setAlarmActive(true);
        layoutBattery->setTime(clockTime);
        // Trzeba pobrać format
        // layoutBattery->setTimeFormat(timeModel->getTimeFormat());
        layoutBattery->setAlarmTime(alarmTime);
        layoutBattery->setBatteryLevelState(batteryState);
        layoutBattery->setViewState(app::home_screen::ViewState::Activated);
        auto layout = layoutBattery->getLayout();

        addWidget(layout);
    }

    bool LayoutClassicWithBatteryListItem::onInput(const InputEvent &inputEvent)
    {
        return false;
    }

} // namespace gui
