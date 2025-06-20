// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONPRESSEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_MOUSEBUTTONPRESSEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Input.hxx>
    #include <adelie/core/events/MouseButtonEvent.hxx>

namespace adelie::core::events {

    class ADELIE_API MouseButtonPressedEvent : public MouseButtonEvent {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::MouseInput)
            EVENT_CLASS_TYPE(MouseButtonPressed)
        public:
            MouseButtonPressedEvent(const adelie::core::MouseButtonCode& button);

            virtual ~MouseButtonPressedEvent();

            MouseButtonPressedEvent(const MouseButtonPressedEvent&) = delete;

            auto operator=(MouseButtonPressedEvent const&) -> MouseButtonPressedEvent& = delete;

            MouseButtonPressedEvent(MouseButtonPressedEvent&&) = delete;

            auto operator=(MouseButtonPressedEvent&&) -> MouseButtonPressedEvent& = delete;

            [[nodiscard]] auto toString() const -> std::string override;

    }; /* class MouseButtonPressedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONPRESSEDEVENT_HXX__) */
