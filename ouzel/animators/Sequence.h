// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#include "animators/Animator.h"

namespace ouzel
{
    namespace scene
    {
        class Sequence: public Animator
        {
        public:
            Sequence(const std::vector<std::shared_ptr<Animator>>& aAnimators);

            virtual void play() override;

        protected:
            virtual void updateProgress() override;

            std::shared_ptr<Animator> currentAnimator;
        };
    } // namespace scene
} // namespace ouzel
