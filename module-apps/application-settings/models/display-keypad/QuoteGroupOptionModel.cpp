// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QuoteGroupOptionModel.hpp"

namespace gui
{
    QuoteGroupOptionModel::QuoteGroupOptionModel(app::settingsInterface::WallpaperSettings *settings)
        : settings(settings)
    {}

    void QuoteGroupOptionModel::update()
    {
        quoteGroupOption = settings->getQuoteGroupOption();
    }

    void QuoteGroupOptionModel::saveValue(QuoteGroupOption option)
    {
        settings->setQuoteGroupOption(option);
        quoteGroupOption = option;
    }

    auto QuoteGroupOptionModel::isCurrentOption(QuoteGroupOption option) -> bool
    {
        return quoteGroupOption == option;
    }

    auto QuoteGroupOptionModel::isOurFavouritesOptionSelected() -> bool
    {
        return quoteGroupOption == QuoteGroupOption::OurFavourites;
    }

    auto QuoteGroupOptionModel::isCustomOptionSelected() -> bool
    {
        return quoteGroupOption == QuoteGroupOption::Custom;
    }
} // namespace gui
