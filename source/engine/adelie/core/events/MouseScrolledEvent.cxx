// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/MouseScrolledEvent.hxx>
#include <sstream>

using adelie::core::events::MouseScrolledEvent;

MouseScrolledEvent::MouseScrolledEvent(const float& xOffset, const float& yOffset) {
    mOffsetX = xOffset;
    mOffsetY = yOffset;
}

MouseScrolledEvent::~MouseScrolledEvent() = default;

auto MouseScrolledEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "MouseScrolledEvent: " << mOffsetX << ", " << mOffsetY;
    return sstream.str();
}

auto MouseScrolledEvent::getOffsetX() const -> float {
    return mOffsetX;
}

auto MouseScrolledEvent::getOffsetY() const -> float {
    return mOffsetY;
}
