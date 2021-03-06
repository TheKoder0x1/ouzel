// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SpritesSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

SpritesSample::SpritesSample(Samples& aSamples):
    samples(aSamples),
    backButton(std::make_shared<ouzel::gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK))
{
    eventHandler.gamepadHandler = bind(&SpritesSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&SpritesSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SpritesSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    camera = std::make_shared<scene::Camera>();
    camera->setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera->setTargetContentSize(Size2(800.0f, 600.0f));

    layer = std::make_shared<scene::Layer>();
    layer->addCamera(camera);
    addLayer(layer);

    // character
    characterSprite = std::make_shared<scene::Sprite>();
    characterSprite->initFromFile("run.json");
    characterSprite->play(true);

    character = std::make_shared<scene::Node>();
    character->addComponent(characterSprite);
    layer->addChild(character);
    character->setPosition(Vector2(-300.0f, 0.0f));

    move = std::make_shared<scene::Move>(4.0f, Vector2(300.0f, 0.0f));
    character->addComponent(move);
    move->start();

    // fire
    fireSprite = std::make_shared<scene::Sprite>();
    fireSprite->initFromFile("fire.json");
    fireSprite->setOffset(Vector2(0.0f, 20.0f));
    fireSprite->play(true);

    fireNode = std::make_shared<scene::Node>();
    fireNode->addComponent(fireSprite);
    fireNode->setPosition(Vector2(-100.0f, -140.0f));
    layer->addChild(fireNode);

    // triangle
    triangleSprite = std::make_shared<scene::Sprite>();
    triangleSprite->initFromFile("triangle.json");

    triangleNode = std::make_shared<scene::Node>();
    triangleNode->addComponent(triangleSprite);
    triangleNode->setPosition(Vector2(100.0f, -140.0f));
    layer->addChild(triangleNode);

    guiCamera = std::make_shared<scene::Camera>();
    guiCamera->setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera->setTargetContentSize(Size2(800.0f, 600.0f));

    guiLayer = std::make_shared<scene::Layer>();
    guiLayer->addCamera(guiCamera);
    addLayer(guiLayer);

    menu = std::make_shared<gui::Menu>();
    guiLayer->addChild(menu);

    hideButton = std::make_shared<gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Show/hide", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    hideButton->setPosition(Vector2(-200.0f, 200.0f));
    menu->addWidget(hideButton);

    wireframeButton = std::make_shared<gui::Button>("button.png", "button_selected.png", "button_down.png", "", "Wireframe", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK);
    wireframeButton->setPosition(Vector2(-200.0f, 160.0f));
    menu->addWidget(wireframeButton);

    backButton->setPosition(Vector2(-200.0f, -200.0f));
    menu->addWidget(backButton);
}

bool SpritesSample::handleGamepad(Event::Type type, const GamepadEvent& event)
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

bool SpritesSample::handleUI(Event::Type type, const UIEvent& event)
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == backButton)
        {
            samples.setScene(std::shared_ptr<scene::Scene>(new MainMenu(samples)));
        }
        else if (event.node == hideButton)
        {
            character->setHidden(!character->isHidden());
        }
        else if (event.node == wireframeButton)
        {
            camera->setWireframe(!camera->getWireframe());
        }
    }

    return true;
}

bool SpritesSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
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
