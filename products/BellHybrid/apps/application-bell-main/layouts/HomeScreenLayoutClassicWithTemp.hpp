#pragma once

#include "layouts/HomeScreenLayoutClassic.hpp"

namespace style::homescreen_classic
{
    constexpr inline auto temperature_w = 85U;
    constexpr inline auto temperature_h = 102U;
} // namespace style::homescreen_classic

namespace gui
{

    class HomeScreenLayoutClassicWithTemp : public HomeScreenLayoutClassic
    {
      public:
        HomeScreenLayoutClassicWithTemp(std::string name);

        auto setTemperature(utils::temperature::Temperature newTemp) -> void override;

      protected:
        auto buildInterface() -> void override;

        TextFixedSize *tempText{};
    };
}; // namespace gui
