// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/KeyReleasedEvent.hxx>
#include <sstream>

using adelie::core::events::KeyReleasedEvent;

KeyReleasedEvent::KeyReleasedEvent(const KeyCode& keyCode) : KeyEvent(keyCode) {}

KeyReleasedEvent::~KeyReleasedEvent() = default;

auto KeyReleasedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "KeyReleasedEvent: " << static_cast<uint32_t>(mKeyCode);
    return sstream.str();
}