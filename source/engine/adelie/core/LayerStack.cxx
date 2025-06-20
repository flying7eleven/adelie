// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/LayerStack.hxx>
#include <adelie/io/Logger.hxx>
using adelie::core::Layer;
using adelie::core::LayerStack;

LayerStack::LayerStack() {
    mLayerInsertIndex = 0;
}

LayerStack::~LayerStack() {
    for (auto* currentLayer : mLayers) {
        AdelieLogDebug("Destroying layer '%s'...", currentLayer->getName().c_str());
        currentLayer->onDetach();
        delete currentLayer;
    }
    mLayers.clear();
}

void LayerStack::pushLayer(Layer* layer) {
    mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
    mLayerInsertIndex++;
    AdelieLogDebug("Added layer '%s' to the layer stack", layer->getName().c_str());
}

void LayerStack::pushOverlay(Layer* overlay) {
    mLayers.emplace_back(overlay);
    AdelieLogDebug("Added overlay '%s' to the layer stack", overlay->getName().c_str());
}

void LayerStack::popLayer(Layer* layer) {
    auto it = std::find(mLayers.begin(), mLayers.end(), layer);
    if (mLayers.end() != it) {
        mLayers.erase(it);
        mLayerInsertIndex--;
    }
}

void LayerStack::popOverlay(Layer* overlay) {
    auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
    if (mLayers.end() != it) {
        mLayers.erase(it);
    }
}

auto LayerStack::begin() -> std::vector<Layer*>::iterator {
    return mLayers.begin();
}

auto LayerStack::end() -> std::vector<Layer*>::iterator {
    return mLayers.end();
}

auto LayerStack::rbegin() -> std::vector<Layer*>::reverse_iterator {
    return mLayers.rbegin();
}

auto LayerStack::rend() -> std::vector<Layer*>::reverse_iterator {
    return mLayers.rend();
}
