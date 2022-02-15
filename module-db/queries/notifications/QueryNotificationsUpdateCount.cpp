// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QueryNotificationsUpdateCount.hpp"

namespace db::query::notifications
{
    UpdateCount::UpdateCount(NotificationsRecord::Key key, const utils::PhoneNumber::View &number, const ssize_t count)
        : Query(Query::Type::Update), key(key), number(number), count(count)
    {}

    auto UpdateCount::getKey() const noexcept -> NotificationsRecord::Key
    {
        return key;
    }
    auto UpdateCount::getNumber() const noexcept -> const utils::PhoneNumber::View &
    {
        return number;
    }
    auto UpdateCount::getCount() const noexcept -> ssize_t
    {
        return count;
    }

    auto UpdateCount::debugInfo() const -> std::string
    {
        return "UpdateCount";
    }

    UpdateCountResult::UpdateCountResult(bool ret) : ret(ret)
    {}

    auto UpdateCountResult::getResult() const noexcept -> bool
    {
        return ret;
    }

    auto UpdateCountResult::debugInfo() const -> std::string
    {
        return "UpdateCountResult";
    }
} // namespace db::query::notifications
