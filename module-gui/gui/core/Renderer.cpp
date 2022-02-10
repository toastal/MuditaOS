// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

// for memset
#include "Renderer.hpp"
#include "Context.hpp"

// renderer
#include "renderers/PixelRenderer.hpp"
#include <chrono>
#include "log/log.hpp"
#include "FreeRTOS.h"
#include "task.h"
namespace gui
{
    void Renderer::changeColorScheme(const std::unique_ptr<ColorScheme> &scheme)
    {
        renderer::PixelRenderer::updateColorScheme(scheme);
    }

    void Renderer::render(Context *ctx, std::list<std::unique_ptr<DrawCommand>> &commands)
    {
        if (ctx == nullptr) {
            return;
        }
        auto initial_time = xTaskGetTickCount();

        for (auto &cmd : commands) {
            if (cmd == nullptr) {
                continue;
            }

            cmd->draw(ctx);
        }
        auto end_time = xTaskGetTickCount();
        auto result   = (end_time - initial_time) / portTICK_RATE_MS;
        //        auto result = std::chrono::duration_cast<std::chrono::milliseconds>( stop - start ).count();
        LOG_ERROR("Rendering time: %lu ms", result);
    }

} /* namespace gui */
