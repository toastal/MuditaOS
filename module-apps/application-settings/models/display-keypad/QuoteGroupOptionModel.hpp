// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <application-settings/ApplicationSettings.hpp>

namespace gui
{
    class QuoteGroupOptionModel
    {
      public:
        explicit QuoteGroupOptionModel(app::settingsInterface::WallpaperSettings *settings);
        void update();
        void saveValue(QuoteGroupOption option);
        auto isCurrentOption(QuoteGroupOption option) -> bool;
        auto isOurFavouritesOptionSelected() -> bool;
        auto isCustomOptionSelected() -> bool;

      private:
        app::settingsInterface::WallpaperSettings *settings = nullptr;
        QuoteGroupOption quoteGroupOption                   = QuoteGroupOption::OurFavourites;
    };
} // namespace gui
