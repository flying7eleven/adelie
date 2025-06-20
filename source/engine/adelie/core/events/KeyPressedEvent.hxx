// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_KEYPRESSEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_KEYPRESSEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/KeyEvent.hxx>

namespace adelie::core::events {

    class ADELIE_API KeyPressedEvent : public KeyEvent {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::KeyboardInput)
            EVENT_CLASS_TYPE(KeyPressed)
        public:
            KeyPressedEvent(const KeyCode& keyCode, const uint32_t& repeatCount);

            virtual ~KeyPressedEvent();

            KeyPressedEvent(const KeyPressedEvent&) = delete;

            auto operator=(KeyPressedEvent const&) -> KeyPressedEvent& = delete;

            KeyPressedEvent(KeyPressedEvent&&) = delete;

            auto operator=(KeyPressedEvent&&) -> KeyPressedEvent& = delete;

            [[nodiscard]] auto getRepeatCount() const -> uint32_t;

            [[nodiscard]] auto toString() const -> std::string override;

        private:
            uint32_t mRepeatCount;

    }; /* class KeyPressedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_KEYPRESSEDEVENT_HXX__) */
