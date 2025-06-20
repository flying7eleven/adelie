// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/exception/NotImplementedException.hxx>

using adelie::exception::NotImplementedException;

NotImplementedException::NotImplementedException() noexcept : RuntimeException("The called method was not implemented.") {}

NotImplementedException::~NotImplementedException() noexcept = default;
