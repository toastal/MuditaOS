// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <catch2/catch.hpp>
#include "btstack_util.h"
#include <command/Command.hpp>
#include <command/DeviceData.hpp>
#include <command/PhoneNumberData.hpp>

using namespace bluetooth;

TEST_CASE("Command data handling test")
{
    utils::PhoneNumber number("123456789", utils::country::getIdByAlpha2Code("PL"));

    bluetooth::Command commandReceivedFromQueue(bluetooth::Command::None);

    auto queueSimulator = [](bluetooth::Command *cmd, bluetooth::Command *targetCmd) {
        memcpy(targetCmd, cmd, sizeof(bluetooth::Command));
    };

    {
        auto data          = std::make_shared<PhoneNumberData>(number);
        auto commandToSend = bluetooth::Command(Command::Type::PowerOn, std::move(data));
        queueSimulator(&commandToSend, &commandReceivedFromQueue);
    }

    auto receivedData = commandReceivedFromQueue.getData();
    REQUIRE(number == std::get<utils::PhoneNumber::View>(receivedData));
    REQUIRE(commandReceivedFromQueue.getType() == bluetooth::Command::PowerOn);
    commandReceivedFromQueue.cleanup();
}

TEST_CASE("DeviceData clone test")
{
    Devicei device("TestDevice");
    bd_addr_t addr;
    sscanf_bd_addr("00:11:22:33:44:55", addr);
    device.deviceState  = DeviceState::ConnectedBoth;
    device.state        = DEVICE_STATE::REMOTE_NAME_FETCHED;
    device.isPairingSSP = true;
    device.setAddress(&addr);
    CommandData *newData;

    {
        auto data    = std::make_shared<DeviceData>(device);
        newData      = data->clone();
        auto device2 = std::get<Devicei>(newData->getData());
        REQUIRE(device == device2);
    }

    auto device2 = std::get<Devicei>(newData->getData());
    delete newData;

    REQUIRE(device == device2);
}
TEST_CASE("PhoneNumberData clone test - number constructor")
{
    utils::PhoneNumber number("123456789", utils::country::getIdByAlpha2Code("PL"));

    CommandData *newData;

    {
        auto data    = std::make_shared<PhoneNumberData>(number);
        newData      = data->clone();
        auto number2 = std::get<utils::PhoneNumber::View>(newData->getData());
        REQUIRE(number == number2);
    }

    auto number2 = std::get<utils::PhoneNumber::View>(newData->getData());
    delete newData;

    REQUIRE(number == number2);
}

TEST_CASE("PhoneNumberData clone test - view constructor")
{
    utils::PhoneNumber number("123456789", utils::country::getIdByAlpha2Code("PL"));
    CommandData *newData;

    {
        auto data    = std::make_shared<PhoneNumberData>(number.getView());
        newData      = data->clone();
        auto number2 = std::get<utils::PhoneNumber::View>(newData->getData());
        REQUIRE(number == number2);
    }

    auto number2 = std::get<utils::PhoneNumber::View>(newData->getData());
    delete newData;

    REQUIRE(number == number2);
}
