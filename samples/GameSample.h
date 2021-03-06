// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class GameSample: public ouzel::scene::Scene
{
public:
    GameSample(Samples& aSamples);

private:
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event);
    bool handleMouse(ouzel::Event::Type type, const ouzel::MouseEvent& event);
    bool handleTouch(ouzel::Event::Type type, const ouzel::TouchEvent& event);
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    
    Samples& samples;

    std::shared_ptr<ouzel::scene::Layer> layer;
    std::shared_ptr<ouzel::scene::Camera> camera;

    ouzel::EventHandler eventHandler;
};
