// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Interface/NotificationsRecord.hpp>
#include <Common/Query.hpp>
#include <string>
#include <PhoneNumber.hpp>
#include <cstdint>

namespace db::query::notifications
{
    class UpdateCount : public Query
    {
        const NotificationsRecord::Key key;
        const utils::PhoneNumber::View number;
        const ssize_t count;

      public:
        UpdateCount(NotificationsRecord::Key key, const utils::PhoneNumber::View &number, const ssize_t count = 1);

        [[nodiscard]] auto getKey() const noexcept -> NotificationsRecord::Key;
        [[nodiscard]] auto getNumber() const noexcept -> const utils::PhoneNumber::View &;
        [[nodiscard]] auto getCount() const noexcept -> ssize_t;

        [[nodiscard]] auto debugInfo() const -> std::string override;
    };

    class UpdateCountResult : public QueryResult
    {
        bool ret;

      public:
        explicit UpdateCountResult(bool ret);
        [[nodiscard]] auto getResult() const noexcept -> bool;

        [[nodiscard]] auto debugInfo() const -> std::string override;
    };

} // namespace db::query::notifications
