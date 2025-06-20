// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONRELEASEDEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_MOUSEBUTTONRELEASEDEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Input.hxx>
    #include <adelie/core/events/MouseButtonEvent.hxx>

namespace adelie::core::events {

    class ADELIE_API MouseButtonReleasedEvent : public MouseButtonEvent {
            EVENT_CLASS_CATEGORY(EventCategory::GenericInput | EventCategory::MouseInput)
            EVENT_CLASS_TYPE(MouseButtonReleased)
        public:
            MouseButtonReleasedEvent(const adelie::core::MouseButtonCode& button);

            virtual ~MouseButtonReleasedEvent();

            MouseButtonReleasedEvent(const MouseButtonReleasedEvent&) = delete;

            auto operator=(MouseButtonReleasedEvent const&) -> MouseButtonReleasedEvent& = delete;

            MouseButtonReleasedEvent(MouseButtonReleasedEvent&&) = delete;

            auto operator=(MouseButtonReleasedEvent&&) -> MouseButtonReleasedEvent& = delete;

            [[nodiscard]] auto toString() const -> std::string override;

    }; /* class MouseButtonReleasedEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_MOUSEBUTTONRELEASEDEVENT_HXX__) */
