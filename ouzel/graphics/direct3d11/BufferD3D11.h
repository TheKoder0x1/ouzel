// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#define NOMINMAX
#include <d3d11.h>
#include "graphics/BufferResource.h"

namespace ouzel
{
    namespace graphics
    {
        class BufferD3D11: public BufferResource
        {
        public:
            BufferD3D11();
            virtual ~BufferD3D11();

            ID3D11Buffer* getBuffer() const { return buffer; }

        protected:
            virtual bool upload() override;

            ID3D11Buffer* buffer = nullptr;
            UINT bufferSize = 0;
        };
    } // namespace graphics
} // namespace ouzel
