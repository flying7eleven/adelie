// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_EVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_EVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <functional>
    #include <iostream>

namespace adelie::core::events {

    enum class EventType {
        NoEventType = 0,
        WindowClose,
        WindowResize,
        WindowGainFocus,
        WindowsLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseMoved,
        MouseScrolled,
        MouseButtonPressed,
        MouseButtonReleased
    };

    enum EventCategory { NoEventCategory = 0, Application = 1 << 0, GenericInput = 1 << 1, KeyboardInput = 1 << 2, MouseInput = 1 << 3 };

    #define EVENT_CLASS_TYPE(type)                                  \
    public:                                                         \
        static auto getStaticType() -> EventType {                  \
            return EventType::type;                                 \
        }                                                           \
        virtual auto getEventType() const -> EventType override {   \
            return getStaticType();                                 \
        }                                                           \
        virtual auto getEventName() const -> const char* override { \
            return #type;                                           \
        }

    #define EVENT_CLASS_CATEGORY(category)                           \
    public:                                                          \
        virtual auto getCategoryFlags() const -> uint32_t override { \
            return category;                                         \
        }

    class ADELIE_API Event {
        public:
            Event();
            virtual ~Event();
            [[nodiscard]] virtual auto getEventType() const -> EventType = 0;
            [[nodiscard]] virtual auto getEventName() const -> const char* = 0;
            [[nodiscard]] virtual auto getCategoryFlags() const -> uint32_t = 0;
            [[nodiscard]] virtual auto toString() const -> std::string;
            [[nodiscard]] auto isHandled() const -> bool;
            void markHandled();

        private:
            bool mHandled;
    }; /* class Event */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_EVENT_HXX__) */
