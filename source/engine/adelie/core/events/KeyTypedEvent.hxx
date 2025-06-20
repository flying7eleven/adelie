// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_KEYTYPEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_KEYTYPEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API KeyTypedEvent : public Event {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::KeyboardInput)
            EVENT_CLASS_TYPE(KeyTyped)
        public:
            KeyTypedEvent(const uint16_t& typedKey);

            virtual ~KeyTypedEvent();

            KeyTypedEvent(const KeyTypedEvent&) = delete;

            auto operator=(KeyTypedEvent const&) -> KeyTypedEvent& = delete;

            KeyTypedEvent(KeyTypedEvent&&) = delete;

            auto operator=(KeyTypedEvent&&) -> KeyTypedEvent& = delete;

            [[nodiscard]] auto getTypedKey() const -> uint16_t;

            [[nodiscard]] auto toString() const -> std::string override;

        private:
            uint16_t mTypedKey;

    }; /* class KeyTypedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_KEYTYPEDEVENT_HXX__) */
