// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <PhoneNumber.hpp>

namespace sys
{
    class Service;
}

namespace call::api
{
    class Multicast
    {
      public:
        virtual void notifyIncommingCall()                                        = 0;
        virtual void notifyIdentifiedCall(const utils::PhoneNumber::View &number) = 0;
        virtual void notifyCallActive()                                           = 0;
        virtual void notifyCallAborted()                                          = 0;
        virtual ~Multicast()                                                      = default;
    };
} // namespace call::api

class CallMulticast : public call::api::Multicast
{
    sys::Service *owner;

  public:
    explicit CallMulticast(sys::Service *owner) : owner(owner)
    {}
    void notifyIncommingCall() override;
    void notifyIdentifiedCall(const utils::PhoneNumber::View &number) override;
    void notifyCallActive() override;
    void notifyCallAborted() override;
};
