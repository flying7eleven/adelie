// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/MouseButtonPressedEvent.hxx>
#include <sstream>

using adelie::core::events::MouseButtonPressedEvent;

MouseButtonPressedEvent::MouseButtonPressedEvent(const adelie::core::MouseButtonCode& button) : MouseButtonEvent(button) {}

MouseButtonPressedEvent::~MouseButtonPressedEvent() = default;

auto MouseButtonPressedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "MouseButtonPressedEvent: " << static_cast<uint8_t>(mButton);
    return sstream.str();
}