// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <service-cellular/Constans.hpp>
#include "PhoneInterface.hpp"
#include "service-audio/AudioServiceAPI.hpp"
#include "service-cellular/CellularServiceAPI.hpp"
namespace bluetooth
{
    bool CellularInterfaceImpl::answerIncomingCall(sys::Service *service)
    {
        return CellularServiceAPI::AnswerIncomingCall(service);
    }

    bool CellularInterfaceImpl::hangupCall(sys::Service *service)
    {
        return CellularServiceAPI::HangupCall(service);
    }
    bool CellularInterfaceImpl::sendDTMFCode(sys::Service *service, DTMFCode code)
    {
        auto msg = std::make_shared<CellularDtmfRequestMessage>(code);
        service->bus.sendUnicast(std::move(msg), service::name::cellular);
        return true;
    }

    bool CellularInterfaceImpl::dialNumber(sys::Service *service, const std::string &number)
    {
        return CellularServiceAPI::DialNumber(service, number);
    }

    bool AudioInterfaceImpl::startAudioRouting(sys::Service *service)
    {
        return AudioServiceAPI::RoutingStart(service);
    }
    bool AudioInterfaceImpl::stopAudioRouting(sys::Service *service)
    {
        return AudioServiceAPI::StopAll(service);
    }
} // namespace bluetooth
