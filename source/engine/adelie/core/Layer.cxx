// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/Layer.hxx>
using adelie::core::Layer;
using adelie::core::events::Event;

Layer::Layer(const std::string& name) {
    mLayerName = name;
}

Layer::~Layer() = default;

void Layer::onAttach() {}

void Layer::onDetach() {}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Layer::onUpdate(const Timestep& deltaTime) {}
#pragma clang diagnostic pop

void Layer::onUIRendering() {}

void Layer::onEvent(Event& /*event*/) {}

auto Layer::getName() const -> const std::string& {
    return mLayerName;
}
