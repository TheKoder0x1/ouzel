// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <cstdint>
#include <vector>
#include "scene/NodeContainer.h"
#include "math/Vector2.h"

namespace ouzel
{
    namespace scene
    {
        class Scene;
        class Node;
        class Camera;

        class Layer: public NodeContainer
        {
            friend Scene;
        public:
            Layer();
            virtual ~Layer();

            virtual void draw();

            virtual void addChild(const std::shared_ptr<Node>& node) override;

            void addCamera(const std::shared_ptr<Camera>& camera);
            void removeCamera(const std::shared_ptr<Camera>& camera);
            const std::vector<std::shared_ptr<Camera>>& getCameras() const { return cameras; }

            std::shared_ptr<Node> pickNode(const Vector2& position) const;
            std::vector<std::shared_ptr<Node>> pickNodes(const Vector2& position) const;
            std::vector<std::shared_ptr<Node>> pickNodes(const std::vector<Vector2>& edges) const;

            int32_t getOrder() const { return order; }
            void setOrder(int32_t newOrder);

        protected:
            virtual void recalculateProjection();
            virtual void enter() override;

            Scene* scene = nullptr;

            std::vector<std::shared_ptr<Camera>> cameras;

            int32_t order = 0;
        };
    } // namespace scene
} // namespace ouzel
