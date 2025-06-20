// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/MouseButtonReleasedEvent.hxx>
#include <sstream>

using adelie::core::events::MouseButtonReleasedEvent;

MouseButtonReleasedEvent::MouseButtonReleasedEvent(const adelie::core::MouseButtonCode& button) : MouseButtonEvent(button) {}

MouseButtonReleasedEvent::~MouseButtonReleasedEvent() = default;

auto MouseButtonReleasedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "MouseButtonReleasedEvent: " << static_cast<uint8_t>(mButton);
    return sstream.str();
}