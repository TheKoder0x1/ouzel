// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <GLES2/gl2.h>

#include "RendererOGLEm.h"
#include "core/Engine.h"
#include "utils/Utils.h"

namespace ouzel
{
    namespace graphics
    {
        RendererOGLEm::~RendererOGLEm()
        {
            emscripten_webgl_destroy_context(webGLContext);
        }

        void RendererOGLEm::free()
        {
            if (webGLContext)
            {
                emscripten_webgl_destroy_context(webGLContext);
                webGLContext = 0;
            }
        }

        bool RendererOGLEm::init(const WindowPtr& window,
                                 uint32_t newSampleCount,
                                 TextureFilter newTextureFilter,
                                 PixelFormat newBackBufferFormat,
                                 bool newVerticalSync)
        {
            free();

            apiMajorVersion = 2;
            apiMinorVersion = 0;

            EmscriptenWebGLContextAttributes attrs;
            emscripten_webgl_init_context_attributes(&attrs);

            webGLContext = emscripten_webgl_create_context(0, &attrs);

            if (!webGLContext)
            {
                log(LOG_LEVEL_ERROR, "Failed to create WebGL context");
                return false;
            }

            return RendererOGL::init(window, newSampleCount, newTextureFilter, newBackBufferFormat, newVerticalSync);
        }

        bool RendererOGLEm::present()
        {
            EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(webGLContext);

            if (res != EMSCRIPTEN_RESULT_SUCCESS)
            {
                return false;
            }

            if (!RendererOGL::present())
            {
                return false;
            }

            return true;
        }
    } // namespace graphics
} // namespace ouzel