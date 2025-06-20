// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_IO_LOGGER_HXX__)
    #define __ADELIE_IO_LOGGER_HXX__

    #include <adelie/adelie.hxx>
    #include <array>
    #include <chrono>
    #include <format>
    #include <fstream>
    #include <string_view>

    #define AdelieLogTrace(...) adelie::io::Logger::getInstance()->trace(__VA_ARGS__)
    #define AdelieLogDebug(...) adelie::io::Logger::getInstance()->debug(__VA_ARGS__)
    #define AdelieLogInformation(...) adelie::io::Logger::getInstance()->info(__VA_ARGS__)
    #define AdelieLogWarning(...) adelie::io::Logger::getInstance()->warn(__VA_ARGS__)
    #define AdelieLogError(...) adelie::io::Logger::getInstance()->error(__VA_ARGS__)

namespace adelie::io {

    constexpr auto MAX_LOGGING_LINE_SIZE = 4096;
    constexpr auto MAX_LOGGING_TIME_LINE_SIZE = 32;

    enum class LoggingLevel : unsigned char { LevelNothing, LevelTrace, LevelDebug, LevelInformation, LevelWarning, LevelError, LevelFatal }; /* enum class LoggingLevel */

    enum class ForegroundColor : unsigned char {
        ForegroundBlack [[maybe_unused]] = 30,
        ForegroundRed = 31,
        ForegroundGreen = 32,
        ForegroundYellow = 33,
        ForegroundBlue = 34,
        ForegroundMagenta [[maybe_unused]] = 35,
        ForegroundCyan [[maybe_unused]] = 36,
        ForegroundWhite = 37
    }; /* enum class ForegroundColor */

    enum class BackgroundColor : unsigned char {
        BackgroundBlack [[maybe_unused]] = 40,
        BackgroundRed = 41,
        BackgroundGreen [[maybe_unused]] = 42,
        BackgroundYellow [[maybe_unused]] = 43,
        BackgroundBlue [[maybe_unused]] = 44,
        BackgroundMagenta [[maybe_unused]] = 45,
        BackgroundCyan [[maybe_unused]] = 46,
        BackgroundWhite [[maybe_unused]] = 47
    }; /* enum class BackgroundColor */

    enum class ConsoleFlags : unsigned char {
        FlagReset [[maybe_unused]] = 0,
        FlagBoldBright = 1,
        FlagUnderline [[maybe_unused]] = 4,
        FlagSlowBlink [[maybe_unused]] = 5,
        FlagRapidBlink = 6,
        FlagInverse [[maybe_unused]] = 7,
        FlagBoldBrightOff [[maybe_unused]] = 21,
        FlagUnderlineOff [[maybe_unused]] = 24,
        FlagBlinkOff [[maybe_unused]] = 25,
        FlagInverseOff [[maybe_unused]] = 27
    }; /* enum class ConsoleFlags */

    class ADELIE_API Logger {
        public:
            static auto getInstance() -> Logger*;

            Logger(const Logger&) = delete;

            auto operator=(Logger const&) -> Logger& = delete;

            Logger(Logger&&) = delete;

            auto operator=(Logger&&) -> Logger& = delete;

            ~Logger();

            [[maybe_unused]] void changeOutputFile(const char* filename) noexcept;

            [[maybe_unused]] void enableColor() noexcept;

            [[maybe_unused]] void disableColor() noexcept;

            template <typename... Args>
            void trace(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelTrace, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

            template <typename... Args>
            void debug(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelDebug, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

            template <typename... Args>
            void info(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelInformation, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

            template <typename... Args>
            void warn(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelWarning, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

            template <typename... Args>
            void error(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelError, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

            template <typename... Args>
            void fatal(std::format_string<Args...> fmt, Args&&... args) noexcept {
                try {
                    auto formatted = std::format(fmt, std::forward<Args>(args)...);
                    this->printWithPrefix(LoggingLevel::LevelFatal, formatted);
                } catch (...) {
                    // nothing to do here, we stay silent; it is just the logging call that will fail silently
                }
            }

        protected:
            Logger();

            auto printWithPrefix(const LoggingLevel& level, const std::string_view& message) noexcept -> void;

            static auto getLoggingString(const std::array<char, MAX_LOGGING_TIME_LINE_SIZE>& timeAsString, const LoggingLevel& level, const std::string_view& message) noexcept -> std::string;

            auto getColorControlSequence(const LoggingLevel& level) const noexcept -> std::string;

            auto getColorResetControlSequence() const noexcept -> std::string;

        private:
            bool mColorEnabled;
            std::ofstream mLogfileStream;
            std::chrono::time_point<std::chrono::steady_clock> mStartTime;

    }; /* class Logger */

} /* namespace adelie::io */

#endif /* if !defined(__ADELIE_IO_LOGGER_HXX__) */