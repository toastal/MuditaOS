// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "PureTxAudioDeviceFactory.hpp"
#include "PureTxAudioCodec.hpp"
#include "RT1051CellularAudio.hpp"
#include "audio/BluetoothAudioDevice.hpp"

#include <Audio/Profiles/Profile.hpp>

#include <magic_enum.hpp>
#include <log/log.hpp>

using audio::AudioDevice;
using audio::PureTxAudioCodec;
using audio::PureTxAudioDeviceFactory;
using audio::RT1051CellularAudio;

std::shared_ptr<AudioDevice> PureTxAudioDeviceFactory::getDevice(const audio::Profile &profile)
{
    std::shared_ptr<AudioDevice> device;

    auto audioConfiguration = profile.GetAudioConfiguration();

    auto audioDeviceType = profile.GetAudioDeviceType();
    LOG_ERROR("audioDeviceType: %s", magic_enum::enum_name(audioDeviceType).data());
    switch (audioDeviceType) {
    case AudioDevice::Type::Audiocodec: {
        device = std::make_shared<PureTxAudioCodec>(audioConfiguration);
    } break;

    case AudioDevice::Type::BluetoothA2DP: {
        device = std::make_shared<bluetooth::A2DPAudioDevice>();
    } break;

    case AudioDevice::Type::BluetoothHSP: {
        device = std::make_shared<bluetooth::CVSDAudioDevice>(bluetooth::AudioProfile::HSP);
    } break;

    case AudioDevice::Type::BluetoothHFP: {
        device = std::make_shared<bluetooth::CVSDAudioDevice>(bluetooth::AudioProfile::HFP);
    } break;

    case AudioDevice::Type::Cellular: {
        device = std::make_shared<RT1051CellularAudio>();
    } break;

    default:
        break;
    };

    return device;
}

std::shared_ptr<AudioDevice> PureTxAudioDeviceFactory::createCellularAudioDevice()
{
    return std::make_shared<RT1051CellularAudio>();
}
