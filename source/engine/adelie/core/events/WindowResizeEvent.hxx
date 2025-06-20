// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_WINDOWRESIZEEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_WINDOWRESIZEEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API WindowResizeEvent : public Event {
            EVENT_CLASS_TYPE(WindowResize)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
        public:
            WindowResizeEvent(const uint16_t& newWidth, const uint16_t& newHeight);

            [[nodiscard]] auto getWidth() const -> uint16_t;

            [[nodiscard]] auto getHeight() const -> uint16_t;

            [[nodiscard]] auto toString() const -> std::string override;

        private:
            uint16_t mWidth;
            uint16_t mHeight;
    }; /* class WindowResizeEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_WINDOWRESIZEEVENT_HXX__) */
