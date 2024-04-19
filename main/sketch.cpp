// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 Ricardo Quesada
// http://retro.moe/unijoysticle2

#include "sdkconfig.h"

#include <Arduino.h>
#include <Bluepad32.h>

//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default, it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using, "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

void dumpMouse(ControllerPtr ctl) {
    Console.printf("idx=%d, buttons: 0x%04x, scrollWheel=0x%04x, delta X: %4d, delta Y: %4d\n",
                   ctl->index(),        // Controller Index
                   ctl->buttons(),      // bitmask of pressed buttons
                   ctl->scrollWheel(),  // Scroll Wheel
                   ctl->deltaX(),       // (-511 - 512) left X Axis
                   ctl->deltaY()        // (-511 - 512) left Y axis
    );
}

void dumpKeyboard(ControllerPtr ctl) {
    for (int key = Keyboard_A; key <= Keyboard_RightMeta; ++key) {
    if (ctl->isKeyPressed(static_cast<KeyboardKey>(key))) {
      String keyName = getKeyName(static_cast<KeyboardKey>(key));
      Serial.printf("idx=%d -> ", ctl->index());
      Serial.println(keyName);
    }
  }
}

void dumpBalanceBoard(ControllerPtr ctl) {
    Console.printf("idx=%d,  TL=%u, TR=%u, BL=%u, BR=%u, temperature=%d\n",
                   ctl->index(),        // Controller Index
                   ctl->topLeft(),      // top-left scale
                   ctl->topRight(),     // top-right scale
                   ctl->bottomLeft(),   // bottom-left scale
                   ctl->bottomRight(),  // bottom-right scale
                   ctl->temperature()   // temperature: used to adjust the scale value's precision
    );
}

void processGamepad(ControllerPtr ctl) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    if (ctl->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                ctl->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                ctl->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                ctl->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
    }

    if (ctl->b()) {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually indicate
        // the "gamepad seat".
        // It is possible to change them by calling:
        ctl->setPlayerLEDs(led & 0x0f);
    }

    if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
                            0x40 /* strongMagnitude */);
    }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    dumpGamepad(ctl);

    // See ArduinoController.h for all the available functions.
}

void processMouse(ControllerPtr ctl) {
    // This is just an example.
    if (ctl->scrollWheel() > 0) {
        // Do Something
    } else if (ctl->scrollWheel() < 0) {
        // Do something else
    }

    // See "dumpMouse" for possible things to query.
    dumpMouse(ctl);
}

void processKeyboard(ControllerPtr ctl) {
    // This is just an example.
    /*if (ctl->isKeyPressed(Keyboard_A)) {
        // Do Something
        Console.println("Key 'A' pressed");
    }

    // Don't do "else" here.
    // Multiple keys can be pressed at the same time.
    if (ctl->isKeyPressed(Keyboard_LeftShift)) {
        // Do something else
        Console.println("Key 'LEFT SHIFT' pressed");
    }

    // Don't do "else" here.
    // Multiple keys can be pressed at the same time.
    if (ctl->isKeyPressed(Keyboard_LeftArrow)) {
        // Do something else
        Console.println("Key 'Left Arrow' pressed");
    }*/

    // See "dumpKeyboard" for possible things to query.
    dumpKeyboard(ctl);
}

void processBalanceBoard(ControllerPtr ctl) {
    // This is just an example.
    if (ctl->topLeft() > 10000) {
        // Do Something
    }

    // See "dumpBalanceBoard" for possible things to query.
    dumpBalanceBoard(ctl);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else if (myController->isMouse()) {
                processMouse(myController);
            } else if (myController->isKeyboard()) {
                processKeyboard(myController);
            } else if (myController->isBalanceBoard()) {
                processBalanceBoard(myController);
            } else {
                Console.printf("Unsupported controller\n");
            }
        }
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Console.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Console.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);

    // Enables the BLE Service in Bluepad32.
    // This service allows clients, like a mobile app, to setup and see the state of Bluepad32.
    // By default, it is disabled.
    BP32.enableBLEService(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    //     vTaskDelay(1);
    delay(150);
}

String getKeyName(KeyboardKey key) {
  switch (key) {
    case Keyboard_A: return "A";
    case Keyboard_B: return "B";
    case Keyboard_C: return "C";
    case Keyboard_D: return "D";
    case Keyboard_E: return "E";
    case Keyboard_F: return "F";
    case Keyboard_G: return "G";
    case Keyboard_H: return "H";
    case Keyboard_I: return "I";
    case Keyboard_J: return "J";
    case Keyboard_K: return "K";
    case Keyboard_L: return "L";
    case Keyboard_M: return "M";
    case Keyboard_N: return "N";
    case Keyboard_O: return "O";
    case Keyboard_P: return "P";
    case Keyboard_Q: return "Q";
    case Keyboard_R: return "R";
    case Keyboard_S: return "S";
    case Keyboard_T: return "T";
    case Keyboard_U: return "U";
    case Keyboard_V: return "V";
    case Keyboard_W: return "W";
    case Keyboard_X: return "X";
    case Keyboard_Y: return "Y";
    case Keyboard_Z: return "Z";
    case Keyboard_1: return "1";
    case Keyboard_2: return "2";
    case Keyboard_3: return "3";
    case Keyboard_4: return "4";
    case Keyboard_5: return "5";
    case Keyboard_6: return "6";
    case Keyboard_7: return "7";
    case Keyboard_8: return "8";
    case Keyboard_9: return "9";
    case Keyboard_0: return "0";
    case Keyboard_Enter: return "Enter";
    case Keyboard_Escape: return "Escape";
    case Keyboard_Backspace: return "Backspace";
    case Keyboard_Tab: return "Tab";
    case Keyboard_Spacebar: return "Spacebar";
    case Keyboard_Underscore: return "Underscore";
    case Keyboard_Equal: return "Equal";
    case Keyboard_OpenBracket: return "OpenBracket";
    case Keyboard_CloseBracket: return "CloseBracket";
    case Keyboard_Backslash: return "Backslash";
    case Keyboard_Tilde: return "Tilde";
    case Keyboard_SemiColon: return "SemiColon";
    case Keyboard_Quote: return "Quote";
    case Keyboard_GraveAccent: return "GraveAccent";
    case Keyboard_Comma: return "Comma";
    case Keyboard_Dot: return "Dot";
    case Keyboard_Slash: return "Slash";
    case Keyboard_CapsLock: return "CapsLock";
    case Keyboard_F1: return "F1";
    case Keyboard_F2: return "F2";
    case Keyboard_F3: return "F3";
    case Keyboard_F4: return "F4";
    case Keyboard_F5: return "F5";
    case Keyboard_F6: return "F6";
    case Keyboard_F7: return "F7";
    case Keyboard_F8: return "F8";
    case Keyboard_F9: return "F9";
    case Keyboard_F10: return "F10";
    case Keyboard_F11: return "F11";
    case Keyboard_F12: return "F12";
    case Keyboard_PrintScreen: return "PrintScreen";
    case Keyboard_ScrollLock: return "ScrollLock";
    case Keyboard_Pause: return "Pause";
    case Keyboard_Insert: return "Insert";
    case Keyboard_Home: return "Home";
    case Keyboard_PageUp: return "PageUp";
    case Keyboard_Delete: return "Delete";
    case Keyboard_End: return "End";
    case Keyboard_PageDown: return "PageDown";
    case Keyboard_RightArrow: return "RightArrow";
    case Keyboard_LeftArrow: return "LeftArrow";
    case Keyboard_DownArrow: return "DownArrow";
    case Keyboard_UpArrow: return "UpArrow";
    case Keypad_NumLock: return "NumLock";
    case Keypad_Slash: return "KeypadSlash";
    case Keypad_Asterisk: return "KeypadAsterisk";
    case Keypad_Minus: return "KeypadMinus";
    case Keypad_Plus: return "KeypadPlus";
    case Keypad_Enter: return "KeypadEnter";
    case Keypad_1: return "Keypad1";
    case Keypad_2: return "Keypad2";
    case Keypad_3: return "Keypad3";
    case Keypad_4: return "Keypad4";
    case Keypad_5: return "Keypad5";
    case Keypad_6: return "Keypad6";
    case Keypad_7: return "Keypad7";
    case Keypad_8: return "Keypad8";
    case Keypad_9: return "Keypad9";
    case Keypad_0: return "Keypad0";
    case Keypad_Dot: return "KeypadDot";
    case Keyboard_BackSlash: return "BackSlash";
    case Keyboard_Application: return "Application";
    case Keyboard_Power: return "Power";
    case Keypad_Equal: return "KeypadEqual";
    case Keyboard_F13: return "F13";
    case Keyboard_F14: return "F14";
    case Keyboard_F15: return "F15";
    case Keyboard_F16: return "F16";
    case Keyboard_F17: return "F17";
    case Keyboard_F18: return "F18";
    case Keyboard_F19: return "F19";
    case Keyboard_F20: return "F20";
    case Keyboard_F21: return "F21";
    case Keyboard_F22: return "F22";
    case Keyboard_F23: return "F23";
    case Keyboard_F24: return "F24";
    case Keyboard_Execute: return "Execute";
    case Keyboard_Help: return "Help";
    case Keyboard_Menu: return "Menu";
    case Keyboard_Select: return "Select";
    case Keyboard_Stop: return "Stop";
    case Keyboard_Again: return "Again";
    case Keyboard_Undo: return "Undo";
    case Keyboard_Cut: return "Cut";
    case Keyboard_Copy: return "Copy";
    case Keyboard_Paste: return "Paste";
    case Keyboard_Find: return "Find";
    case Keyboard_Mute: return "Mute";
    case Keyboard_VolumeUp: return "VolumeUp";
    case Keyboard_VolumeDown: return "VolumeDown";
    case Keyboard_LockingCapsLock: return "LockingCapsLock";
    case Keyboard_LockingNumLock: return "LockingNumLock";
    case Keyboard_LockingScrollLock: return "LockingScrollLock";
    case Keypad_Comma: return "KeypadComma";
    case Keypad_EqualSign: return "KeypadEqualSign";
    case Keyboard_International1: return "International1";
    case Keyboard_International2: return "International2";
    case Keyboard_International3: return "International3";
    case Keyboard_International4: return "International4";
    case Keyboard_International5: return "International5";
    case Keyboard_International6: return "International6";
    case Keyboard_International7: return "International7";
    case Keyboard_International8: return "International8";
    case Keyboard_International9: return "International9";
    case Keyboard_Lang1: return "Lang1";
    case Keyboard_Lang2: return "Lang2";
    case Keyboard_Lang3: return "Lang3";
    case Keyboard_Lang4: return "Lang4";
    case Keyboard_Lang5: return "Lang5";
    case Keyboard_Lang6: return "Lang6";
    case Keyboard_Lang7: return "Lang7";
    case Keyboard_Lang8: return "Lang8";
    case Keyboard_Lang9: return "Lang9";
    case Keyboard_AltErase: return "AltErase";
    case Keyboard_Sysreq: return "Sysreq";
    case Keyboard_Cancel: return "Cancel";
    case Keyboard_Clear: return "Clear";
    case Keyboard_Prior: return "Prior";
    case Keyboard_Return: return "Return";
    case Keyboard_Separator: return "Separator";
    case Keyboard_Out: return "Out";
    case Keyboard_Oper: return "Oper";
    case Keyboard_ClearAgain: return "ClearAgain";
    case Keyboard_CrSel: return "CrSel";
    case Keyboard_ExSel: return "ExSel";
    case Keypad_00: return "Keypad00";
    case Keypad_000: return "Keypad000";
    case Keyboard_ThousandsSeparator: return "ThousandsSeparator";
    case Keyboard_DecimalSeparator: return "DecimalSeparator";
    case Keyboard_CurrentyUnit: return "CurrencyUnit";
    case Keyboard_CurrencySubUnit: return "CurrencySubUnit";
    case Keypad_OpenParenthesis: return "KeypadOpenParenthesis";
    case Keypad_CloseParenthesis: return "KeypadCloseParenthesis";
    case Keypad_OpenBrace: return "KeypadOpenBrace";
    case Keypad_CloseBrace: return "KeypadCloseBrace";
    case Keypad_Tab: return "KeypadTab";
    case Keypad_Backspace: return "KeypadBackspace";
    case Keypad_A: return "KeypadA";
    case Keypad_B: return "KeypadB";
    case Keypad_C: return "KeypadC";
    case Keypad_D: return "KeypadD";
    case Keypad_E: return "KeypadE";
    case Keypad_F: return "KeypadF";
    case Keypad_Xor: return "KeypadXor";
    case Keypad_Caret: return "KeypadCaret";
    case Keypad_Percent: return "KeypadPercent";
    case Keypad_Less: return "KeypadLess";
    case Keypad_Greater: return "KeypadGreater";
    case Keypad_Ampersand: return "KeypadAmpersand";
    case Keypad_LogicalAnd: return "KeypadLogicalAnd";
    case Keypad_VerticalBar: return "KeypadVerticalBar";
    case Keypad_LogicalOr: return "KeypadLogicalOr";
    case Keypad_Colon: return "KeypadColon";
    case Keypad_NumberSign: return "KeypadNumberSign";
    case Keypad_Space: return "KeypadSpace";
    case Keypad_At: return "KeypadAt";
    case Keypad_ExclamationMark: return "KeypadExclamationMark";
    case Keypad_MemoryStore: return "KeypadMemoryStore";
    case Keypad_MemoryRecall: return "KeypadMemoryRecall";
    case Keypad_MemoryClear: return "KeypadMemoryClear";
    case Keypad_MemoryAdd: return "KeypadMemoryAdd";
    case Keypad_MemorySubtract: return "KeypadMemorySubtract";
    case Keypad_MemoryMultiply: return "KeypadMemoryMultiply";
    case Keypad_MemoryDivide: return "KeypadMemoryDivide";
    case Keypad_PlusMinus: return "KeypadPlusMinus";
    case Keypad_Clear: return "KeypadClear";
    case Keypad_ClearEntry: return "KeypadClearEntry";
    case Keypad_Binary: return "KeypadBinary";
    case Keypad_Octal: return "KeypadOctal";
    case Keypad_Decimal: return "KeypadDecimal";
    case Keypad_Hexadecimal: return "KeypadHexadecimal";
    case Keyboard_LeftControl: return "LeftControl";
    case Keyboard_LeftShift: return "LeftShift";
    case Keyboard_LeftAlt: return "LeftAlt";
    case Keyboard_LeftMeta: return "LeftMeta";
    case Keyboard_RightControl: return "RightControl";
    case Keyboard_RightShift: return "RightShift";
    case Keyboard_RightAlt: return "RightAlt";
    case Keyboard_RightMeta: return "RightMeta";
    default: return "Unknown";
  }
}
