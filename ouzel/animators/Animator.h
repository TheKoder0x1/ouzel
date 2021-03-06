// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <memory>
#include <functional>
#include "scene/Component.h"
#include "core/UpdateCallback.h"

namespace ouzel
{
    namespace scene
    {
        class Node;

        class Animator: public Component
        {
            friend Node;
        public:
            Animator(float aLength);
            virtual ~Animator();

            virtual void update(float delta);

            virtual void start();
            virtual void play();

            virtual void resume();
            virtual void stop(bool resetAnimation = false);
            virtual void reset();

            bool isRunning() const { return running; }
            bool isDone() const { return done; }

            float getLength() const { return length; }
            float getCurrentTime() const { return currentTime; }

            float getProgress() const { return progress; }
            virtual void setProgress(float newProgress);

            Node* getTargetNode() const { return targetNode; }

            void setFinishHandler(const std::function<void()>& handler) { finishHandler = handler; }

        protected:
            void addAnimator(const std::shared_ptr<Animator>& animator);
            bool removeAnimator(const std::shared_ptr<Animator>& animator);
            virtual void updateProgress();

            float length = 0.0f;
            float currentTime = 0.0f;
            float progress = 0.0f;
            bool done = false;
            bool running = false;

            Animator* parent = nullptr;
            Node* targetNode = nullptr;

            std::function<void()> finishHandler;

            UpdateCallback updateCallback;

            std::vector<std::shared_ptr<Animator>> animators;
        };
    } // namespace scene
} // namespace ouzel
