// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/KeyPressedEvent.hxx>
#include <sstream>

using adelie::core::events::KeyPressedEvent;

KeyPressedEvent::KeyPressedEvent(const KeyCode& keyCode, const uint32_t& repeatCount) : KeyEvent(keyCode) {
    mRepeatCount = repeatCount;
}

KeyPressedEvent::~KeyPressedEvent() = default;

auto KeyPressedEvent::getRepeatCount() const -> uint32_t {
    return mRepeatCount;
}

auto KeyPressedEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "KeyPressedEvent: " << static_cast<uint64_t>(mKeyCode) << " (" << mRepeatCount << " times)";
    return sstream.str();
}