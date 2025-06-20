// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_LAYER_HXX__)
    #define __ADELIE_CORE_LAYER_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/Timestep.hxx>
    #include <adelie/core/events/Event.hxx>
    #include <string>

namespace adelie::core {

    class ADELIE_API Layer {
        public:
            [[maybe_unused]] Layer(const std::string& name);
            virtual ~Layer();

            [[maybe_unused]] virtual void onAttach();
            [[maybe_unused]] virtual void onDetach();
            [[maybe_unused]] virtual void onUpdate(const Timestep& deltaTime);
            [[maybe_unused]] virtual void onUIRendering();
            [[maybe_unused]] virtual void onEvent(adelie::core::events::Event& event);

            [[nodiscard]] auto getName() const -> const std::string&;

        private:
            std::string mLayerName;
    }; /* class Layer */

} /* namespace adelie::core */

#endif /* if !defined(__ADELIE_CORE_LAYER_HXX__) */
