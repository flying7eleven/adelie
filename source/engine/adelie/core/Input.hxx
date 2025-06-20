// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_INPUT_HXX__)
    #define __ADELIE_CORE_INPUT_HXX__

    #include <adelie/adelie.hxx>
    #include <cstdint>
    #include <limits>
    #include <utility>

namespace adelie::core {

    enum class KeyCode : uint8_t {
        CharacterA,
        CharacterB,
        CharacterC,
        CharacterD,
        CharacterE,
        CharacterF,
        CharacterG,
        CharacterH,
        CharacterI,
        CharacterJ,
        CharacterK,
        CharacterL,
        CharacterM,
        CharacterN,
        CharacterO,
        CharacterP,
        CharacterQ,
        CharacterR,
        CharacterS,
        CharacterT,
        CharacterU,
        CharacterV,
        CharacterW,
        CharacterX,
        CharacterY,
        CharacterZ,
        Character0,
        Character1,
        Character2,
        Character3,
        Character4,
        Character5,
        Character6,
        Character7,
        Character8,
        Character9,
        SpecialKeyTab,
        SpecialKeyEscape,
        SpecialKeyReturn,
        SpecialKeySpacebar,
        SpecialKeyLeftArrow,
        SpecialKeyUpArrow,
        SpecialKeyRightArrow,
        SpecialKeyDownArrow,
        SpecialKeyPageUp,
        SpecialKeyPageDown,
        SpecialKeyHome,
        SpecialKeyEnd,
        SpecialKeyInsert,
        SpecialKeyDelete,
        SpecialKeyBackspace,
        SpecialKeyLeftShift,
        SpecialKeyRightShift,
        SpecialKeyLeftCtrl,
        SpecialKeyRightCtrl,
        SpecialKeyLeftSuper,
        SpecialKeyRightSuper,
        SpecialKeyLeftAlt,
        SpecialKeyRightAlt,
        UnknownKey,
    }; /* enum class KeyCode */

    enum class MouseButtonCode : uint8_t {
        MouseButtonLeft,
        MouseButtonRight,
        MouseButtonMiddle,
        MouseButtonExtended1,
        MouseButtonExtended2,
        MouseButtonUnknown,
    }; /* enum class MouseButton */

    class ADELIE_API Input {
        public:
            static auto isKeyPressed(const KeyCode& keycode) -> bool;

            static auto isMouseButtonPressed(const MouseButtonCode& button) -> bool;

            static auto getMouseX() -> float;

            static auto getMouseY() -> float;

            static auto getMousePosition() -> std::pair<float, float>;

    }; /* class Input */

} /* namespace adelie::core */

#endif /* if !defined(__ADELIE_CORE_INPUT_HXX__) */
