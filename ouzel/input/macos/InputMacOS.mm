// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#import <CoreGraphics/CoreGraphics.h>
#import <GameController/GameController.h>
#import <Carbon/Carbon.h>
#include "InputMacOS.h"
#include "core/macos/WindowMacOS.h"
#include "core/Application.h"
#include "core/Engine.h"
#include "GamepadMacOS.h"
#include "events/EventDispatcher.h"
#include "utils/Log.h"

static void deviceAdded(void* ctx, IOReturn, void*, IOHIDDeviceRef device)
{
    ouzel::input::InputMacOS* inputMacOS = reinterpret_cast<ouzel::input::InputMacOS*>(ctx);
    inputMacOS->handleGamepadConnected(device);
}

static void deviceRemoved(void *ctx, IOReturn, void*, IOHIDDeviceRef device)
{
    ouzel::input::InputMacOS* inputMacOS = reinterpret_cast<ouzel::input::InputMacOS*>(ctx);
    inputMacOS->handleGamepadDisconnected(device);
}

namespace ouzel
{
    namespace input
    {
        KeyboardKey InputMacOS::convertKeyCode(unsigned short keyCode)
        {
            switch (keyCode)
            {
                case kVK_UpArrow: return KeyboardKey::UP;
                case kVK_DownArrow: return KeyboardKey::DOWN;
                case kVK_LeftArrow: return KeyboardKey::LEFT;
                case kVK_RightArrow: return KeyboardKey::RIGHT;
                case kVK_F1: return KeyboardKey::F1;
                case kVK_F2: return KeyboardKey::F2;
                case kVK_F3: return KeyboardKey::F3;
                case kVK_F4: return KeyboardKey::F4;
                case kVK_F5: return KeyboardKey::F5;
                case kVK_F6: return KeyboardKey::F6;
                case kVK_F7: return KeyboardKey::F7;
                case kVK_F8: return KeyboardKey::F8;
                case kVK_F9: return KeyboardKey::F9;
                case kVK_F10: return KeyboardKey::F10;
                case kVK_F11: return KeyboardKey::F11;
                case kVK_F12: return KeyboardKey::F12;
                case kVK_F13: return KeyboardKey::F13;
                case kVK_F14: return KeyboardKey::F14;
                case kVK_F15: return KeyboardKey::F15;
                case kVK_F16: return KeyboardKey::F16;
                case kVK_F17: return KeyboardKey::F17;
                case kVK_F18: return KeyboardKey::F18;
                case kVK_F19: return KeyboardKey::F19;
                case kVK_F20: return KeyboardKey::F20;
                case kVK_Home: return KeyboardKey::HOME;
                case kVK_End: return KeyboardKey::END;
                case NSInsertFunctionKey: return KeyboardKey::INSERT;
                case kVK_ForwardDelete: return KeyboardKey::DEL;
                case kVK_Help: return KeyboardKey::HELP;
                case NSSelectFunctionKey: return KeyboardKey::SELECT;
                case NSPrintFunctionKey: return KeyboardKey::PRINT;
                case NSExecuteFunctionKey: return KeyboardKey::EXECUTE;
                case NSPrintScreenFunctionKey: return KeyboardKey::SNAPSHOT;
                case NSPauseFunctionKey: return KeyboardKey::PAUSE;
                case NSScrollLockFunctionKey: return KeyboardKey::SCROLL;
                case kVK_Delete: return KeyboardKey::BACKSPACE;
                case kVK_Tab: return KeyboardKey::TAB;
                case kVK_Return: return KeyboardKey::RETURN;
                case kVK_Escape: return KeyboardKey::ESCAPE;
                case kVK_Control: return KeyboardKey::CONTROL;
                case kVK_RightControl: return KeyboardKey::RCONTROL;
                case kVK_Command: return KeyboardKey::LSUPER;
                case kVK_Shift: return KeyboardKey::SHIFT;
                case kVK_RightShift: return KeyboardKey::RSHIFT;
                case kVK_Space: return KeyboardKey::SPACE;

                case kVK_ANSI_A: return KeyboardKey::KEY_A;
                case kVK_ANSI_B: return KeyboardKey::KEY_B;
                case kVK_ANSI_C: return KeyboardKey::KEY_C;
                case kVK_ANSI_D: return KeyboardKey::KEY_D;
                case kVK_ANSI_E: return KeyboardKey::KEY_E;
                case kVK_ANSI_F: return KeyboardKey::KEY_F;
                case kVK_ANSI_G: return KeyboardKey::KEY_G;
                case kVK_ANSI_H: return KeyboardKey::KEY_H;
                case kVK_ANSI_I: return KeyboardKey::KEY_I;
                case kVK_ANSI_J: return KeyboardKey::KEY_J;
                case kVK_ANSI_K: return KeyboardKey::KEY_K;
                case kVK_ANSI_L: return KeyboardKey::KEY_L;
                case kVK_ANSI_M: return KeyboardKey::KEY_M;
                case kVK_ANSI_N: return KeyboardKey::KEY_N;
                case kVK_ANSI_O: return KeyboardKey::KEY_O;
                case kVK_ANSI_P: return KeyboardKey::KEY_P;
                case kVK_ANSI_Q: return KeyboardKey::KEY_Q;
                case kVK_ANSI_R: return KeyboardKey::KEY_R;
                case kVK_ANSI_S: return KeyboardKey::KEY_S;
                case kVK_ANSI_T: return KeyboardKey::KEY_T;
                case kVK_ANSI_U: return KeyboardKey::KEY_U;
                case kVK_ANSI_V: return KeyboardKey::KEY_V;
                case kVK_ANSI_W: return KeyboardKey::KEY_W;
                case kVK_ANSI_X: return KeyboardKey::KEY_X;
                case kVK_ANSI_Y: return KeyboardKey::KEY_Y;
                case kVK_ANSI_Z: return KeyboardKey::KEY_Z;

                case kVK_ANSI_0: return KeyboardKey::KEY_0;
                case kVK_ANSI_1: return KeyboardKey::KEY_1;
                case kVK_ANSI_2: return KeyboardKey::KEY_2;
                case kVK_ANSI_3: return KeyboardKey::KEY_3;
                case kVK_ANSI_4: return KeyboardKey::KEY_4;
                case kVK_ANSI_5: return KeyboardKey::KEY_5;
                case kVK_ANSI_6: return KeyboardKey::KEY_6;
                case kVK_ANSI_7: return KeyboardKey::KEY_7;
                case kVK_ANSI_8: return KeyboardKey::KEY_8;
                case kVK_ANSI_9: return KeyboardKey::KEY_9;

                case kVK_ANSI_Comma: return KeyboardKey::COMMA;
                case kVK_ANSI_Period: return KeyboardKey::PERIOD;
                case kVK_PageUp: return KeyboardKey::PRIOR;
                case kVK_PageDown: return KeyboardKey::NEXT;

                case kVK_ANSI_Keypad0: return KeyboardKey::NUMPAD0;
                case kVK_ANSI_Keypad1: return KeyboardKey::NUMPAD1;
                case kVK_ANSI_Keypad2: return KeyboardKey::NUMPAD2;
                case kVK_ANSI_Keypad3: return KeyboardKey::NUMPAD3;
                case kVK_ANSI_Keypad4: return KeyboardKey::NUMPAD4;
                case kVK_ANSI_Keypad5: return KeyboardKey::NUMPAD5;
                case kVK_ANSI_Keypad6: return KeyboardKey::NUMPAD6;
                case kVK_ANSI_Keypad7: return KeyboardKey::NUMPAD7;
                case kVK_ANSI_Keypad8: return KeyboardKey::NUMPAD8;
                case kVK_ANSI_Keypad9: return KeyboardKey::NUMPAD9;

                case kVK_ANSI_KeypadDecimal: return KeyboardKey::DECIMAL;
                case kVK_ANSI_KeypadMultiply: return KeyboardKey::MULTIPLY;
                case kVK_ANSI_KeypadPlus: return KeyboardKey::PLUS;
                case kVK_ANSI_KeypadClear: return KeyboardKey::OEM_CLEAR;
                case kVK_ANSI_KeypadDivide: return KeyboardKey::DIVIDE;
                case kVK_ANSI_KeypadEnter: return KeyboardKey::RETURN;
                case kVK_ANSI_KeypadMinus: return KeyboardKey::SUBTRACT;

                case kVK_ANSI_Semicolon: return KeyboardKey::SEMICOLON;
                case kVK_ANSI_Slash: return KeyboardKey::SLASH;
                case kVK_ANSI_Grave: return KeyboardKey::GRAVE;
                case kVK_ANSI_LeftBracket: return KeyboardKey::BRACKET_LEFT;
                case kVK_ANSI_Backslash: return KeyboardKey::BACKSLASH;
                case kVK_ANSI_RightBracket: return KeyboardKey::BRACKET_RIGHT;
                default: return KeyboardKey::NONE;
            }
        }

        uint32_t InputMacOS::getModifiers(NSUInteger modifierFlags, NSUInteger pressedMouseButtons)
        {
            uint32_t modifiers = 0;

            if (modifierFlags & NSShiftKeyMask) modifiers |= SHIFT_DOWN;
            if (modifierFlags & NSAlternateKeyMask) modifiers |= ALT_DOWN;
            if (modifierFlags & NSControlKeyMask) modifiers |= CONTROL_DOWN;
            if (modifierFlags & NSCommandKeyMask) modifiers |= SUPER_DOWN;
            if (modifierFlags & NSFunctionKeyMask) modifiers |= FUNCTION_DOWN;

            if (pressedMouseButtons)
            {
                if (pressedMouseButtons & (1 << 0)) modifiers |= LEFT_MOUSE_DOWN;
                if (pressedMouseButtons & (1 << 1)) modifiers |= RIGHT_MOUSE_DOWN;
                if (pressedMouseButtons & (1 << 2)) modifiers |= MIDDLE_MOUSE_DOWN;
            }

            return modifiers;
        }

        InputMacOS::InputMacOS()
        {
        }

        InputMacOS::~InputMacOS()
        {
            if (hidManager)
            {
                IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone);
                CFRelease(hidManager);
            }
        }

        bool InputMacOS::init()
        {
            NSArray* criteria = @[
                                  @{@kIOHIDDeviceUsagePageKey: @(kHIDPage_GenericDesktop), @kIOHIDDeviceUsageKey: @(kHIDUsage_GD_Joystick)},
                                  @{@kIOHIDDeviceUsagePageKey: @(kHIDPage_GenericDesktop), @kIOHIDDeviceUsageKey: @(kHIDUsage_GD_GamePad)},
                                  @{@kIOHIDDeviceUsagePageKey: @(kHIDPage_GenericDesktop), @kIOHIDDeviceUsageKey : @(kHIDUsage_GD_MultiAxisController)}
                                  ];

            hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

            IOHIDManagerSetDeviceMatchingMultiple(hidManager, (CFArrayRef)criteria);
            if (IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone) != kIOReturnSuccess)
            {
                Log(Log::Level::ERR) << "Failed to initialize manager";
                return false;
            }
            else
            {
                IOHIDManagerRegisterDeviceMatchingCallback(hidManager, deviceAdded, this);
                IOHIDManagerRegisterDeviceRemovalCallback(hidManager, deviceRemoved, this);
                IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
            }

            return true;
        }

        void InputMacOS::setCursorVisible(bool visible)
        {
            if (visible != cursorVisible)
            {
                cursorVisible = visible;

                sharedApplication->execute([visible] {
                    if (visible)
                    {
                        [NSCursor unhide];
                    }
                    else
                    {
                        [NSCursor hide];
                    }
                });
            }
        }

        bool InputMacOS::isCursorVisible() const
        {
            return cursorVisible;
        }

        void InputMacOS::setCursorPosition(const Vector2& position)
        {
            Input::setCursorPosition(position);

            ouzel::Vector2 windowLocation = ouzel::sharedEngine->getWindow()->convertNormalizedToWindowLocation(position);

            sharedApplication->execute([windowLocation] {
                CGPoint screenOrigin = [[NSScreen mainScreen] visibleFrame].origin;

                NSWindow* window = static_cast<WindowMacOS*>(sharedEngine->getWindow())->getNativeWindow();
                CGPoint windowOrigin = [window frame].origin;

                CGWarpMouseCursorPosition(CGPointMake(screenOrigin.x + windowOrigin.x + windowLocation.v[0],
                                                      screenOrigin.y + windowOrigin.y + windowLocation.v[1]));
            });
        }

        void InputMacOS::setCursorLocked(bool locked)
        {
            sharedApplication->execute([locked] {
                CGAssociateMouseAndMouseCursorPosition(!locked);
            });
            cursorLocked = locked;
        }

        bool InputMacOS::isCursorLocked() const
        {
            return cursorLocked;
        }

        void InputMacOS::handleGamepadConnected(IOHIDDeviceRef device)
        {
            Event event;
            event.type = Event::Type::GAMEPAD_CONNECT;

            std::unique_ptr<GamepadMacOS> gamepad(new GamepadMacOS(device));

            event.gamepadEvent.gamepad = gamepad.get();

            gamepads.push_back(std::move(gamepad));

            sharedEngine->getEventDispatcher()->postEvent(event);
        }

        void InputMacOS::handleGamepadDisconnected(IOHIDDeviceRef device)
        {
            std::vector<std::unique_ptr<Gamepad>>::iterator i = std::find_if(gamepads.begin(), gamepads.end(), [device](const std::unique_ptr<Gamepad>& gamepad) {
                GamepadMacOS* currentGamepad = static_cast<GamepadMacOS*>(gamepad.get());
                return currentGamepad->getDevice() == device;
            });

            if (i != gamepads.end())
            {
                Event event;
                event.type = Event::Type::GAMEPAD_DISCONNECT;

                event.gamepadEvent.gamepad = (*i).get();

                sharedEngine->getEventDispatcher()->postEvent(event);

                gamepads.erase(i);
            }
        }
    } // namespace input
} // namespace ouzel
