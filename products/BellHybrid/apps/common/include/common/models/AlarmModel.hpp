// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "AbstractAlarmModel.hpp"

#include <apps-common/AsyncTask.hpp>
#include <module-db/Interface/AlarmEventRecord.hpp>
#include <service-db/Settings.hpp>

#include <functional>

namespace app
{
    class ApplicationCommon;
}

namespace app
{
    constexpr std::uint32_t maxSnoozeCount = 3;

    class AlarmModel : public AbstractAlarmModel, public AsyncCallbackReceiver
    {
      public:
        explicit AlarmModel(ApplicationCommon *app);

        bool isActive() const override;
        void setAlarmTime(time_t time) override;
        time_t getAlarmTime() const override;
        void activate(bool value) override;
        void update(AlarmModelReadyHandler callback) override;
        std::uint32_t getSnoozeDuration() override;
        bool isSnoozeAllowed() override;
        void turnOff() override;
        void snooze() override;

      private:
        enum class State
        {
            Invalid,
            InitInProgress,
            Valid
        };

        void updateAlarm(AlarmEventRecord &alarm);
        AlarmEventRecord generateDefaultAlarm() const;
        std::shared_ptr<AlarmEventRecord> getAlarmPtr() const;

        ApplicationCommon *app{};
        State state{State::Invalid};
        SingleEventRecord cachedRecord;
        std::uint32_t snoozeCount = 0;

        std::function<bool(sys::ResponseMessage *)> responseCallback;

        mutable settings::Settings settings;
    };
} // namespace app