// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/KeyEvent.hxx>

using adelie::core::events::KeyEvent;

KeyEvent::KeyEvent(const KeyCode& keyCode) {
    mKeyCode = keyCode;
}

KeyEvent::~KeyEvent() = default;

auto KeyEvent::getKeyCode() const -> const KeyCode& {
    return mKeyCode;
}