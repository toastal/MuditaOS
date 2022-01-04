#include "layouts/HomeScreenLayoutClassicWithBattery.hpp"
#include "widgets/BellBattery.hpp"

namespace gui
{
    HomeScreenLayoutClassicWithBattery::HomeScreenLayoutClassicWithBattery(std::string name)
        : HomeScreenLayoutClassic(std::move(name))
    {
        buildInterface();
    }
    auto HomeScreenLayoutClassicWithBattery::buildInterface() -> void
    {
        battery->setBatteryPercentMode(BatteryPercentMode::Static);
    }

    bool HomeScreenLayoutClassicWithBattery::isBatteryVisibilityAllowed(const Store::Battery &batteryContext)
    {
        return true;
    }
}; // namespace gui
