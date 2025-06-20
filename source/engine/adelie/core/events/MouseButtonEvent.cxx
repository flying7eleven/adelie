// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/MouseButtonEvent.hxx>

using adelie::core::events::MouseButtonEvent;

MouseButtonEvent::MouseButtonEvent(const adelie::core::MouseButtonCode& button) {
    mButton = button;
}

MouseButtonEvent::~MouseButtonEvent() = default;

auto MouseButtonEvent::getMouseButton() -> const adelie::core::MouseButtonCode& {
    return mButton;
}