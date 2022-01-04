#pragma once

#include "layouts/HomeScreenLayoutClassic.hpp"

namespace gui
{
    class HomeScreenLayoutClassicWithAmPm : public HomeScreenLayoutClassic
    {
      public:
        HomeScreenLayoutClassicWithAmPm(std::string name);

        auto setTime(std::time_t newTime) -> void override;

      protected:
        auto buildInterface() -> void override;

        TextFixedSize *fmt{};
    };
}; // namespace gui
