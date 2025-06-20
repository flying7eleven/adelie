// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__WATSCHEL_WATSCHELAPP_HXX__)
    #define __WATSCHEL_WATSCHELAPP_HXX__

    #include <Adelie.hxx>

namespace watschel {

    class WatschelApp {
        public:
            WatschelApp();

            WatschelApp(const WatschelApp&) = delete;

            WatschelApp(WatschelApp&&) = delete;

            ~WatschelApp() noexcept;

            auto operator=(WatschelApp const&) -> WatschelApp& = delete;

            auto operator=(WatschelApp&&) -> WatschelApp& = delete;

            void initialize();

            void run();

    }; /* class WatschelApp */

} /* namespace watschel */

#endif /* if !defined(__WATSCHEL_WATSCHELAPP_HXX__) */
