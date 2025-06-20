// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/exception/IOException.hxx>

using adelie::exception::IOException;

IOException::IOException(const std::string& message) noexcept : RuntimeException(message) {}

IOException::~IOException() noexcept = default;
