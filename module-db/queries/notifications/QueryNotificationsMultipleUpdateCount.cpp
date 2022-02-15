// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QueryNotificationsMultipleUpdateCount.hpp"

namespace db::query::notifications
{
    MultipleUpdateCount::MultipleUpdateCount(NotificationsRecord::Key key,
                                             const std::vector<utils::PhoneNumber::View> &numbers)
        : Query(Query::Type::Update), key(key), numbers(numbers)
    {}

    auto MultipleUpdateCount::getKey() const noexcept -> NotificationsRecord::Key
    {
        return key;
    }
    auto MultipleUpdateCount::getNumbers() const noexcept -> const std::vector<utils::PhoneNumber::View> &
    {
        return numbers;
    }

    auto MultipleUpdateCount::debugInfo() const -> std::string
    {
        return "MultipleUpdateCount";
    }

    MultipleUpdateCountResult::MultipleUpdateCountResult(bool ret) : ret(ret)
    {}

    auto MultipleUpdateCountResult::getResult() const noexcept -> bool
    {
        return ret;
    }

    auto MultipleUpdateCountResult::debugInfo() const -> std::string
    {
        return "MultipleUpdateCountResult";
    }
} // namespace db::query::notifications
