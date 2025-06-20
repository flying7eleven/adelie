// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/events/WindowResizeEvent.hxx>
#include <sstream>

using adelie::core::events::WindowResizeEvent;

WindowResizeEvent::WindowResizeEvent(const uint16_t& newWidth, const uint16_t& newHeight) {
    mWidth = newWidth;
    mHeight = newHeight;
}

auto WindowResizeEvent::getWidth() const -> uint16_t {
    return mWidth;
}

auto WindowResizeEvent::getHeight() const -> uint16_t {
    return mHeight;
}

auto WindowResizeEvent::toString() const -> std::string {
    std::stringstream sstream;
    sstream << "WindowResizeEvent (" << mWidth << "x" << mHeight << ")";
    return sstream.str();
}
