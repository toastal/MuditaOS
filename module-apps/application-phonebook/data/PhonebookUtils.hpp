// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <ContactRecord.hpp>
#include <Utils.hpp>

namespace phonebookUtils
{
    void fillContactData(std::string &data, ContactRecord &contact);
} // namespace phonebookUtils