// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <service-fileindexer/ServiceFileIndexer.hpp>

#include <service-fileindexer/Constants.hpp>
#include <service-appmgr/messages/UserPowerDownRequest.hpp>

#include <log/log.hpp>
#include <purefs/filesystem_paths.hpp>

namespace
{
    inline auto getMusicPath()
    {
        return purefs::createPath(purefs::dir::getUserDiskPath(), "music").string();
    }
} // namespace

namespace {
    void xPowerOff(std::shared_ptr<sys::Service> svc)
    {
        auto msg = std::make_shared<app::UserPowerDownRequest>();
        svc->bus.sendUnicast(std::move(msg), service::name::system_manager);
    }
}


namespace service
{

    ServiceFileIndexer::ServiceFileIndexer(const std::vector<std::string> &paths)
        : sys::Service{service::name::file_indexer}, mStartupIndexer{paths}
    {
        LOG_DEBUG("[%s] Initializing", service::name::file_indexer);
    }

    sys::MessagePointer ServiceFileIndexer::DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp)
    {
        return sys::msgNotHandled();
    }

    // Initialize data notification handler
    sys::ReturnCodes ServiceFileIndexer::InitHandler()
    {
        if (mInotifyHandler.init(shared_from_this())) {
            mInotifyHandler.addWatch(getMusicPath());
            // -- //
            xPowerOff(shared_from_this());
            // Start the initial indexer
            mStartupIndexer.start(shared_from_this(), service::name::file_indexer);
            return sys::ReturnCodes::Success;
        }

        return sys::ReturnCodes::Failure;
    }

    sys::ReturnCodes ServiceFileIndexer::DeinitHandler()
    {
        return sys::ReturnCodes::Success;
    }

    sys::ReturnCodes ServiceFileIndexer::SwitchPowerModeHandler(const sys::ServicePowerMode mode)
    {
        LOG_DEBUG("Switch to power Mode %s", c_str(mode));
        return sys::ReturnCodes::Success;
    }

    void ServiceFileIndexer::ProcessCloseReason(sys::CloseReason closeReason)
    {
        if (closeReason == sys::CloseReason::FactoryReset) {
            mStartupIndexer.reset();
        }
        sendCloseReadyMessage(this);
    }
} // namespace service
