// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_TIMESTEP_HXX__)
    #define __ADELIE_CORE_TIMESTEP_HXX__

    #include <adelie/adelie.hxx>

namespace adelie::core {

    static inline constexpr float SECONDS_TO_MILLISECONDS = 1000.0F;

    class ADELIE_API Timestep {
        public:
            explicit Timestep(float time) : mTime(time) {};

            [[nodiscard]] auto getSeconds() const -> const float& { return mTime; }

            [[nodiscard]] auto getMilliseconds() const -> float { return mTime * SECONDS_TO_MILLISECONDS; }

            operator const float&() const { return mTime; }

            static auto getCurrentTime() -> float;

        private:
            float mTime;
    }; /* class Timestep */

} /* namespace adelie::core */

#endif /* if !defined(__ADELIE_CORE_TIMESTEP_HXX__) */
