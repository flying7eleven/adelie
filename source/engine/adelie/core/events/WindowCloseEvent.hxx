// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_WINDOWCLOSEEVENT_HXX__)
    #define __ADELIE_CORE_EVENTS_WINDOWCLOSEEVENT_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>

namespace adelie::core::events {

    class ADELIE_API WindowCloseEvent : public Event {
            EVENT_CLASS_TYPE(WindowClose)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    }; /* class WindowCloseEvent */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_WINDOWCLOSEEVENT_HXX__) */
