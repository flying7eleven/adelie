// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/io/Logger.hxx>
#include <iostream>
#include <sstream>

using adelie::io::Logger;

Logger::Logger() {
    mStartTime = std::chrono::steady_clock::now();
    mColorEnabled = false;
    mLogfileStream.open("adelie.log");
}

Logger::~Logger() {
    mLogfileStream.flush();
    mLogfileStream.close();
}

auto Logger::getInstance() -> Logger* {
    static Logger staticInstance;
    return &staticInstance;
}

auto Logger::printWithPrefix(const LoggingLevel& level, const std::string_view& message) noexcept -> void {
    std::array<char, MAX_LOGGING_TIME_LINE_SIZE> timeMessageBuffer{};
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedSeconds = currentTime - mStartTime;

    snprintf(timeMessageBuffer.data(), MAX_LOGGING_TIME_LINE_SIZE, "%10.4f", elapsedSeconds.count());

    std::stringstream loggingMessage;
    loggingMessage << getLoggingString(timeMessageBuffer, level, message);

    if (mLogfileStream.is_open()) {
        mLogfileStream << loggingMessage.str() << std::endl;
    }
    std::cout << getColorControlSequence(level) << loggingMessage.str() << getColorResetControlSequence() << std::endl;
}

auto Logger::getLoggingString(const std::array<char, MAX_LOGGING_TIME_LINE_SIZE>& timeAsString, const LoggingLevel& level, const std::string_view& message) noexcept -> std::string {
    std::string messageString;

    if (LoggingLevel::LevelNothing == level) {
        return message.data();  // NOLINT(fuchsia-default-arguments-calls)
    }

    messageString.append("[");
    messageString.append(timeAsString.data());
    messageString.append("]");
    messageString.append("[");

    switch (level) {
        case LoggingLevel::LevelTrace:
            messageString.append("TRACE");
            break;
        case LoggingLevel::LevelDebug:
            messageString.append("DEBUG");
            break;
        case LoggingLevel::LevelInformation:
            messageString.append("INFO ");
            break;
        case LoggingLevel::LevelWarning:
            messageString.append("WARN ");
            break;
        case LoggingLevel::LevelError:
            messageString.append("ERROR");
            break;
        case LoggingLevel::LevelFatal:
            messageString.append("FATAL");
            break;
        case LoggingLevel::LevelNothing:
            messageString.append("?????");
            break;
    }

    messageString.append("] ");
    messageString.append(message.data());

    return messageString;
}

auto Logger::getColorControlSequence(const LoggingLevel& level) const noexcept -> std::string {
    if (!mColorEnabled) {
        return "";  // NOLINT(fuchsia-default-arguments-calls)
    }

    std::stringstream controlSequence;
    switch (level) {
        case LoggingLevel::LevelNothing:
            break;
        case LoggingLevel::LevelTrace:
            controlSequence << "\033[" << static_cast<unsigned short int>(ForegroundColor::ForegroundBlue) << "m";
            break;
        case LoggingLevel::LevelDebug:
            controlSequence << "\033[" << static_cast<unsigned short int>(ForegroundColor::ForegroundGreen) << "m";
            break;
        case LoggingLevel::LevelInformation:
            // do NOT set the color here since sometimes it has to be white and sometimes black
            // controlSequence << "\033[" << static_cast<unsigned short int>( ForegroundColor::ForegroundWhite ) << "m";
            break;
        case LoggingLevel::LevelWarning:
            controlSequence << "\033[" << static_cast<unsigned short int>(ForegroundColor::ForegroundYellow) << "m";
            break;
        case LoggingLevel::LevelError:
            controlSequence << "\033[" << static_cast<unsigned short int>(ForegroundColor::ForegroundRed) << "m";
            break;
        case LoggingLevel::LevelFatal:
            controlSequence << "\033[" << static_cast<unsigned short int>(ConsoleFlags::FlagRapidBlink) << ";" << static_cast<unsigned short int>(ConsoleFlags::FlagBoldBright) << ";"
                            << static_cast<unsigned short int>(BackgroundColor::BackgroundRed) << ";" << static_cast<unsigned short int>(ForegroundColor::ForegroundWhite) << "m";
            break;
    }
    return controlSequence.str();
}

auto Logger::getColorResetControlSequence() const noexcept -> std::string {
    if (!mColorEnabled) {
        return "";  // NOLINT(fuchsia-default-arguments-calls)
    }

    return "\033[0m";  // NOLINT(fuchsia-default-arguments-calls)
}

[[maybe_unused]] void Logger::enableColor() noexcept {
    mColorEnabled = true;
}

[[maybe_unused]] void Logger::disableColor() noexcept {
    mColorEnabled = false;
}

// TODO: raise-condition, it could happen that a thread tries to write while the file is closed
[[maybe_unused]] void Logger::changeOutputFile(const char* filename) noexcept {
    if (mLogfileStream.is_open()) {
        mLogfileStream.flush();
        mLogfileStream.close();
    }
    mLogfileStream.open(filename);  // NOLINT(fuchsia-default-arguments-calls)
}