// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SoundSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

SoundSample::SoundSample(Samples& aSamples):
    samples(aSamples)
{
    eventHandler.gamepadHandler = bind(&SoundSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&SoundSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SoundSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    std::shared_ptr<audio::SoundDataWave> test8BitData(new audio::SoundDataWave());
    test8BitData->initFromFile("8-bit.wav");

    test8BitSound = std::make_shared<audio::Sound>();
    test8BitSound->init(test8BitData);

    std::shared_ptr<audio::SoundDataWave> test24BitData(new audio::SoundDataWave());
    test24BitData->initFromFile("24-bit.wav");

    test24BitSound = std::make_shared<audio::Sound>();
    test24BitSound->init(test24BitData);

    std::shared_ptr<audio::SoundDataWave> jumpData(new audio::SoundDataWave());
    jumpData->initFromFile("jump.wav");

    jumpSound = std::make_shared<audio::Sound>();
    jumpSound->init(jumpData);

    std::shared_ptr<audio::SoundDataWave> ambientData(new audio::SoundDataWave());
    ambientData->initFromFile("ambient.wav");

    ambientSound = std::make_shared<audio::Sound>();
    ambientSound->init(ambientData);

    guiCamera = std::make_shared<scene::Camera>();
    guiCamera->setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera->setTargetContentSize(Size2(800.0f, 600.0f));

    guiLayer = std::make_shared<scene::Layer>();
    guiLayer->addCamera(guiCamera);
    addLayer(guiLayer);

    menu = std::make_shared<gui::Menu>();
    guiLayer->addChild(menu);

    test8BitButton = std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "8-bit", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    test8BitButton->setPosition(Vector2(0.0f, 80.0f));
    menu->addWidget(test8BitButton);

    test24BitButton = std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "24-bit", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    test24BitButton->setPosition(Vector2(0.0f, 40.0f));
    menu->addWidget(test24BitButton);

    jumpButton = std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Jump", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    jumpButton->setPosition(Vector2(0.0f, 0.0f));
    menu->addWidget(jumpButton);

    ambientButton = std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Ambient", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    ambientButton->setPosition(Vector2(0.0f, -40.0f));
    menu->addWidget(ambientButton);

    backButton = std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    backButton->setPosition(Vector2(-200.0f, -200.0f));
    menu->addWidget(backButton);
}

bool SoundSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    if (type == Event::Type::GAMEPAD_BUTTON_CHANGE)
    {
        if (event.pressed &&
            event.button == input::GamepadButton::FACE2)
        {
            samples.setScene(std::shared_ptr<scene::Scene>(new MainMenu(samples)));
        }
    }

    return true;
}

bool SoundSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == backButton)
        {
            samples.setScene(std::shared_ptr<scene::Scene>(new MainMenu(samples)));
        }
        else if (event.node == test8BitButton)
        {
            test8BitSound->play();
        }
        else if (event.node == test24BitButton)
        {
            test24BitSound->play();
        }
        else if (event.node == jumpButton)
        {
            jumpSound->play();
        }
        else if (event.node == ambientButton)
        {
            ambientSound->play();
        }
    }

    return true;
}

bool SoundSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (type == Event::Type::KEY_DOWN)
    {
        switch (event.key)
        {
            case input::KeyboardKey::ESCAPE:
                samples.setScene(std::shared_ptr<scene::Scene>(new MainMenu(samples)));
                break;
            default:
                break;
        }
    }

    return true;
}
