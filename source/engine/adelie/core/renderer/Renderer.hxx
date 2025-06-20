// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_RENDERER_RENDERER_HXX__)
    #define __ADELIE_CORE_RENDERER_RENDERER_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/renderer/WindowInterface.hxx>

namespace adelie::core::renderer {

    class ADELIE_API Renderer {
        public:
            enum class API : unsigned char { None, Vulkan };

            static auto initialize(const std::unique_ptr<WindowInterface>& windowInterface) -> void;

            static auto getAPI() -> const API& { return sAPI; }

            static auto setAPI(const API& selectedAPI) -> void { sAPI = selectedAPI; }

        private:
            static API sAPI;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    }; /* class Renderer */

} /* namespace adelie::core::renderer */

#endif /* if !defined(__ADELIE_CORE_RENDERER_RENDERER_HXX__) */
