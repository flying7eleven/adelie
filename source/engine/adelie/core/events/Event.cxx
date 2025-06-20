// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/Event.hxx>

using adelie::core::events::Event;

Event::Event() {
    mHandled = false;
}

Event::~Event() = default;

[[maybe_unused]] auto Event::toString() const -> std::string {
    return getEventName();
}

auto Event::isHandled() const -> bool {
    return mHandled;
}

void Event::markHandled() {
    mHandled = true;
}
