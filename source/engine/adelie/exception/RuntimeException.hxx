// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_EXCEPTION_RUNTIMEEXCEPTION_HXX__)
    #define __ADELIE_EXCEPTION_RUNTIMEEXCEPTION_HXX__

    #include <adelie/adelie.hxx>
    #include <boost/stacktrace.hpp>
    #include <string>

namespace adelie::exception {

    class ADELIE_API RuntimeException : std::exception {
        public:
            explicit RuntimeException(const std::string& message) noexcept;

            ~RuntimeException() noexcept override;

            [[nodiscard]] auto getMessage() const noexcept -> const std::string&;

            [[nodiscard]] auto getStacktrace() const noexcept -> const boost::stacktrace::stacktrace&;

        private:
            std::string mExceptionMessage;
            boost::stacktrace::stacktrace mCausingStacktrace;

    }; /* class RuntimeException */

} /* namespace adelie::exception */

#endif /* if !defined(__ADELIE_EXCEPTION_RUNTIMEEXCEPTION_HXX__) */
