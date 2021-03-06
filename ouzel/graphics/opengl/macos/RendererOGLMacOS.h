// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#if defined(__OBJC__)
#import <CoreVideo/CoreVideo.h>
#import <AppKit/NSOpenGL.h>
typedef NSOpenGLContext* NSOpenGLContextPtr;
typedef NSOpenGLPixelFormat* NSOpenGLPixelFormatPtr;
#else
#include <objc/objc.h>
typedef void* CVDisplayLinkRef;
typedef id NSOpenGLContextPtr;
typedef id NSOpenGLPixelFormatPtr;
#endif

#include "graphics/opengl/RendererOGL.h"
#include "events/EventHandler.h"

namespace ouzel
{
    class Engine;

    namespace graphics
    {
        class RendererOGLMacOS: public RendererOGL
        {
            friend Engine;
        public:
            virtual ~RendererOGLMacOS();

            NSOpenGLContextPtr getOpenGLContext() const { return openGLContext; }

        protected:
            virtual bool init(Window* newWindow,
                              const Size2& newSize,
                              uint32_t newSampleCount,
                              Texture::Filter newTextureFilter,
                              uint32_t newMaxAnisotropy,
                              PixelFormat newBackBufferFormat,
                              bool newVerticalSync,
                              bool newDepth) override;

            virtual bool upload() override;
            virtual bool lockContext() override;
            virtual bool swapBuffers() override;

            bool handleWindow(Event::Type type, const WindowEvent& event);

            NSOpenGLContextPtr openGLContext = Nil;
            NSOpenGLPixelFormatPtr pixelFormat = Nil;

            CVDisplayLinkRef displayLink = nullptr;
            EventHandler eventHandler;
        };
    } // namespace graphics
} // namespace ouzel
