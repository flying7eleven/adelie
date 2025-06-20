// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/exception/RuntimeException.hxx>

using adelie::exception::RuntimeException;

RuntimeException::RuntimeException(const std::string& message) noexcept {
    mExceptionMessage = message;
    mCausingStacktrace = boost::stacktrace::stacktrace();
}

auto RuntimeException::getMessage() const noexcept -> const std::string& {
    return mExceptionMessage;
}
auto RuntimeException::getStacktrace() const noexcept -> const boost::stacktrace::stacktrace& {
    return mCausingStacktrace;
}

RuntimeException::~RuntimeException() noexcept = default;
