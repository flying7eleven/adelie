// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/MouseMovedEvent.hxx>
#include <sstream>

using adelie::core::events::MouseMovedEvent;

MouseMovedEvent::MouseMovedEvent(const float& x, const float& y) {
    mX = x;
    mY = y;
}

MouseMovedEvent::~MouseMovedEvent() = default;

auto MouseMovedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "MouseMovedEvent: " << mX << ", " << mY;
    return sstream.str();
}

auto MouseMovedEvent::getX() const -> float {
    return mX;
}

auto MouseMovedEvent::getY() const -> float {
    return mY;
}
