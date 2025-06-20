// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_EVENTS_EVENTDISPATCHER_HXX__)
    #define __ADELIE_CORE_EVENTS_EVENTDISPATCHER_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/events/Event.hxx>
    #include <functional>
    #include <iostream>

namespace adelie::core::events {

    class EventDispatcher {
        public:
            EventDispatcher(Event& event) : mEvent(event) {}

            template <typename T, typename F>
            auto dispatch(const F& func) -> bool {
                if (mEvent.getEventType() == T::getStaticType()) {
                    bool wasHandled = func(*(T*)&mEvent);
                    if (wasHandled) {
                        mEvent.markHandled();
                    }
                    return true;
                }
                return false;
            }

        private:
            Event& mEvent;
    }; /* class EventDispatcher */

} /* namespace adelie::core::events */

#endif /* if !defined(__ADELIE_CORE_EVENTS_EVENTDISPATCHER_HXX__) */
