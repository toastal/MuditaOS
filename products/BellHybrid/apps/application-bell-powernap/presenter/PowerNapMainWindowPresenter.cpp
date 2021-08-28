// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "PowerNapMainWindowPresenter.hpp"
#include "models/PowerNapModel.hpp"
#include <apps-common/Application.hpp>
#include <service-db/agents/settings/SystemSettings.hpp>

namespace
{
    constexpr auto powernapDBRecordName = "powernapPeriod";
} // namespace

namespace app::powernap
{
    PowerNapMainWindowPresenter::PowerNapMainWindowPresenter(app::Application *app, settings::Settings *settings)
        : model{std::make_shared<PowerNapModel>()}, settings{settings}
    {}

    auto PowerNapMainWindowPresenter::getNapTimeProvider() -> std::shared_ptr<gui::ListItemProvider>
    {
        return model;
    }

    void PowerNapMainWindowPresenter::loadData()
    {
        const auto value = settings->getValue(powernapDBRecordName, settings::SettingsScope::AppLocal);
        model->createData();
        model->setValue(std::chrono::minutes{utils::getNumericValue<int>(value)});
    }

    void PowerNapMainWindowPresenter::activate()
    {
        const auto currentValue = model->getCurrentValue();
        settings->setValue(
            powernapDBRecordName, utils::to_string(currentValue.count()), settings::SettingsScope::AppLocal);
    }
} // namespace app::powernap