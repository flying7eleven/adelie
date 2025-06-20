// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/KeyTypedEvent.hxx>
#include <sstream>

using adelie::core::events::KeyTypedEvent;

KeyTypedEvent::KeyTypedEvent(const uint16_t& typedKey) {
    mTypedKey = typedKey;
}

KeyTypedEvent::~KeyTypedEvent() = default;

auto KeyTypedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "KeyTypedEvent: " << mTypedKey;
    return sstream.str();
}

auto KeyTypedEvent::getTypedKey() const -> uint16_t {
    return mTypedKey;
}
