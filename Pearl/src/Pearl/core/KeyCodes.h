#pragma once

namespace Pearl
{
    typedef int KeyCode;
    
    // From imgui.h
    enum KeyCode_
    {
        // Keyboard
        None = 0,
        Tab = 512,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        PageUp,
        PageDown,
        Home,
        End,
        Insert,
        Delete,
        Backspace,
        Space,
        Enter,
        Escape,
        LeftCtrl, LeftShift, LeftAlt, LeftSuper,
        RightCtrl, RightShift, RightAlt, RightSuper,
        Menu,
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
        A, B, C, D, E, F, G, H, I, J,
        K, L, M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z,
        F1, F2, F3, F4, F5, F6,
        F7, F8, F9, F10, F11, F12,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        Semicolon,
        Equal,
        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4,
        Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,

        // Gamepad (some of those are analog values, 0.0f to 1.0f)
        GamepadStart,          // Menu (Xbox)          + (Switch)   Start/Options (PS)
        GamepadBack,           // View (Xbox)          - (Switch)   Share (PS)
        GamepadFaceUp,         // Y (Xbox)             X (Switch)   Triangle (PS)
        GamepadFaceDown,       // A (Xbox)             B (Switch)   Cross (PS)
        GamepadFaceLeft,       // X (Xbox)             Y (Switch)   Square (PS)
        GamepadFaceRight,      // B (Xbox)             A (Switch)   Circle (PS)
        GamepadDpadUp,         // D-pad Up
        GamepadDpadDown,       // D-pad Down
        GamepadDpadLeft,       // D-pad Left
        GamepadDpadRight,      // D-pad Right
        GamepadL1,             // L Bumper (Xbox)      L (Switch)   L1 (PS)
        GamepadR1,             // R Bumper (Xbox)      R (Switch)   R1 (PS)
        GamepadL2,             // L Trigger (Xbox)     ZL (Switch)  L2 (PS) [Analog]
        GamepadR2,             // R Trigger (Xbox)     ZR (Switch)  R2 (PS) [Analog]
        GamepadL3,             // L Thumbstick (Xbox)  L3 (Switch)  L3 (PS)
        GamepadR3,             // R Thumbstick (Xbox)  R3 (Switch)  R3 (PS)
        GamepadLStickUp,       // [Analog]
        GamepadLStickDown,     // [Analog]
        GamepadLStickLeft,     // [Analog]
        GamepadLStickRight,    // [Analog]
        GamepadRStickUp,       // [Analog]
        GamepadRStickDown,     // [Analog]
        GamepadRStickLeft,     // [Analog]
        GamepadRStickRight,    // [Analog]

        ModCtrl, ModShift, ModAlt, ModSuper,

        // End of list
        COUNT
    };
}