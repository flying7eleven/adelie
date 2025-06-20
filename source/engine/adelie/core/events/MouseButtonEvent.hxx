// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_MOUSEBUTTONEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Input.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API MouseButtonEvent : public Event {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::MouseInput)
        public:
            virtual ~MouseButtonEvent();

            MouseButtonEvent(const MouseButtonEvent&) = delete;

            auto operator=(MouseButtonEvent const&) -> MouseButtonEvent& = delete;

            MouseButtonEvent(MouseButtonEvent&&) = delete;

            auto operator=(MouseButtonEvent&&) -> MouseButtonEvent& = delete;

            auto getMouseButton() -> const adelie::core::MouseButtonCode&;

        protected:
            MouseButtonEvent(const adelie::core::MouseButtonCode& button);
            adelie::core::MouseButtonCode mButton;
    }; /* class MouseButtonEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONEVENT_HXX__) */
