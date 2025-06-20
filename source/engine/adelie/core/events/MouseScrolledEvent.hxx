// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_MOUSESCROLLEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_MOUSESCROLLEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API MouseScrolledEvent : public Event {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::MouseInput)
            EVENT_CLASS_TYPE(MouseScrolled)
        public:
            MouseScrolledEvent(const float& xOffset, const float& yOffset);

            virtual ~MouseScrolledEvent();

            MouseScrolledEvent(const MouseScrolledEvent&) = delete;

            auto operator=(MouseScrolledEvent const&) -> MouseScrolledEvent& = delete;

            MouseScrolledEvent(MouseScrolledEvent&&) = delete;

            auto operator=(MouseScrolledEvent&&) -> MouseScrolledEvent& = delete;

            [[nodiscard]] auto getOffsetX() const -> float;

            [[nodiscard]] auto getOffsetY() const -> float;

            [[nodiscard]] auto toString() const -> std::string override;

        private:
            float mOffsetX;
            float mOffsetY;

    }; /* class MouseScrolledEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_MOUSESCROLLEDEVENT_HXX__) */
