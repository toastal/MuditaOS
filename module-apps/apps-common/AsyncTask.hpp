// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <service-db/DBServiceAPI.hpp>
#include <service-db/QueryMessage.hpp>

#include <cstdint>

namespace app
{
    class Application; // Forward declaration

    using RequestId = std::uint64_t;
    class AsyncCallbackReceiver;

    class AsyncCallbacksDeleter
    {
      public:
        virtual ~AsyncCallbacksDeleter() noexcept = default;

        virtual void cancelCallbacks(AsyncCallbackReceiver *receiver) = 0;
    };

    class AsyncCallbackReceiver
    {
      public:
        using Ptr = AsyncCallbackReceiver *;

        explicit AsyncCallbackReceiver(AsyncCallbacksDeleter *deleter) noexcept;
        virtual ~AsyncCallbackReceiver() = 0;

      private:
        AsyncCallbacksDeleter *deleter;
    };

    /**
     * Executes an operation on the sender's thread and saves the info about the receiver context for callback use.
     * Intended to use in order to avoid callbacks called on invalid receiver contexts.
     */
    class AsyncTask
    {
      public:
        virtual ~AsyncTask() noexcept = default;

        /**
         * Executes a task in application's thread and saves the receiver object.
         * @param application       Application
         * @param receiverObject    The context of receiver
         */
        void execute(Application *application, AsyncCallbackReceiver::Ptr receiverObject);

      private:
        /**
         * The specific operation that is to be executed by a task.
         * @param application   Application
         * @return Request identifier, to be matched with a response.
         */
        [[nodiscard]] virtual auto onExecute(Application *application) -> RequestId = 0;
    };

    /**
     * A database query that is using the mechanism of AsyncTask.
     */
    class AsyncQuery : public AsyncTask
    {
      public:
        [[nodiscard]] static auto createFromQuery(std::unique_ptr<db::Query> &&query, db::Interface::Name target)
            -> std::unique_ptr<AsyncQuery>;

        AsyncQuery(std::unique_ptr<db::Query> &&query, db::Interface::Name target) noexcept;

        void setCallback(db::QueryCallbackFunction &&callback) noexcept;
        void setCallback(std::unique_ptr<db::QueryListener> &&listener) noexcept;

      private:
        [[nodiscard]] auto onExecute(Application *application) -> RequestId override;

        std::unique_ptr<db::Query> query;
        db::Interface::Name target;
    };

    class AsyncCallback
    {
      public:
        virtual ~AsyncCallback() noexcept = default;

        [[nodiscard]] virtual auto execute() -> bool = 0;
    };

    class NullCallback : public AsyncCallback
    {
      public:
        [[nodiscard]] auto execute() -> bool override;
    };

    class QueryCallback : public AsyncCallback
    {
      public:
        explicit QueryCallback(db::QueryResponse *response);

        [[nodiscard]] auto execute() -> bool override;

      private:
        db::QueryResponse *response;
    };
} // namespace app