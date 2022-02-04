// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "LayoutClassicWithTempListItem.hpp"
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
constexpr utils::temperature::Temperature temperature = {
    .unit  = utils::temperature::Temperature::Unit::Celsius,
    .value = 21.0f,
};

namespace gui
{
    LayoutClassicWithTempListItem::LayoutClassicWithTempListItem()
        : BellSideListItem(utils::translate("app_bell_settings_layout"))
    {
        auto layoutGeneratorTemp = gui::homeScreenLayouts.at("ClassicWithTemp");
        layoutTemp               = std::move(layoutGeneratorTemp());
        layoutTemp->setAlarmEdit(false);
        layoutTemp->setAlarmActive(true);
        layoutTemp->setTime(clockTime);
        // Trzeba pobrać format
        // layoutTemp->setTimeFormat(timeModel->getTimeFormat());
        layoutTemp->setAlarmTime(alarmTime);
        layoutTemp->setBatteryLevelState(batteryState);
        layoutTemp->setViewState(app::home_screen::ViewState::Activated);
        layoutTemp->setTemperature(temperature);
        addWidget(layoutTemp->getLayout());
    }

    bool LayoutClassicWithTempListItem::onInput(const InputEvent &inputEvent)
    {
        return false;
    }
} // namespace gui
