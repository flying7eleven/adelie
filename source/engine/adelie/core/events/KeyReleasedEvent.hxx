// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_KEYRELEASEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_KEYRELEASEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/KeyEvent.hxx>

namespace adelie::core::events {

    class ADELIE_API KeyReleasedEvent : public KeyEvent {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::KeyboardInput)
            EVENT_CLASS_TYPE(KeyReleased)
        public:
            KeyReleasedEvent(const KeyCode& keyCode);

            virtual ~KeyReleasedEvent();

            KeyReleasedEvent(const KeyReleasedEvent&) = delete;

            auto operator=(KeyReleasedEvent const&) -> KeyReleasedEvent& = delete;

            KeyReleasedEvent(KeyReleasedEvent&&) = delete;

            auto operator=(KeyReleasedEvent&&) -> KeyReleasedEvent& = delete;

            [[nodiscard]] auto toString() const -> std::string override;

    }; /* class KeyReleasedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_KEYRELEASEDEVENT_HXX__) */
