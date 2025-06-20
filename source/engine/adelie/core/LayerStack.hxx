// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_LAYERSTACK_HXX__)
    #define __ADELIE_CORE_LAYERSTACK_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Layer.hxx>
    #include <vector>

namespace adelie::core {

    class ADELIE_API LayerStack {
        public:
            LayerStack();
            ~LayerStack();

            void pushLayer(Layer* layer);
            void pushOverlay(Layer* overlay);
            void popLayer(Layer* layer);
            void popOverlay(Layer* overlay);

            auto begin() -> std::vector<Layer*>::iterator;
            auto end() -> std::vector<Layer*>::iterator;
            auto rbegin() -> std::vector<Layer*>::reverse_iterator;
            auto rend() -> std::vector<Layer*>::reverse_iterator;

        private:
            std::vector<Layer*> mLayers;
            uint32_t mLayerInsertIndex;
    }; /* class LayerStack */

} /* namespace adelie::core */

#endif /* if !defined(__ADELIE_CORE_LAYERSTACK_HXX__) */
