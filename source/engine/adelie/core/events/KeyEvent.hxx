// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_KEYEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_KEYEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Input.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API KeyEvent : public Event {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::KeyboardInput)
        public:
            virtual ~KeyEvent();

            KeyEvent(const KeyEvent&) = delete;

            auto operator=(KeyEvent const&) -> KeyEvent& = delete;

            KeyEvent(KeyEvent&&) = delete;

            auto operator=(KeyEvent&&) -> KeyEvent& = delete;

            [[nodiscard]] auto getKeyCode() const -> const KeyCode&;

        protected:
            KeyEvent(const KeyCode& keyCode);
            KeyCode mKeyCode;
    }; /* class KeyEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_KEYEVENT_HXX__) */
