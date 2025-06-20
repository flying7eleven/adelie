// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_MOUSEMOVEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_MOUSEMOVEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API MouseMovedEvent : public Event {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::MouseInput)
            EVENT_CLASS_TYPE(MouseMoved)
        public:
            MouseMovedEvent(const float& x, const float& y);

            virtual ~MouseMovedEvent();

            MouseMovedEvent(const MouseMovedEvent&) = delete;

            auto operator=(MouseMovedEvent const&) -> MouseMovedEvent& = delete;

            MouseMovedEvent(MouseMovedEvent&&) = delete;

            auto operator=(MouseMovedEvent&&) -> MouseMovedEvent& = delete;

            [[nodiscard]] auto getX() const -> float;

            [[nodiscard]] auto getY() const -> float;

            [[nodiscard]] auto toString() const -> std::string override;

        private:
            float mX;
            float mY;

    }; /* class MouseMovedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_MOUSEMOVEDEVENT_HXX__) */
