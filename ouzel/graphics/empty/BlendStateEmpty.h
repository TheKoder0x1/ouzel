// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "graphics/BlendStateResource.h"

#pragma once

namespace ouzel
{
    namespace graphics
    {
        class BlendStateEmpty: public BlendStateResource
        {
        public:
            BlendStateEmpty();

            virtual bool upload() override;
        };
    } // namespace graphics
} // namespace ouzel
