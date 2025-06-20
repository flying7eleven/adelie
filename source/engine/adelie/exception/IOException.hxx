// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_EXCEPTION_IOEXCEPTION_HXX__)
    #define __ADELIE_EXCEPTION_IOEXCEPTION_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/exception/RuntimeException.hxx>

namespace adelie::exception {

    class ADELIE_API IOException : public RuntimeException {
        public:
            explicit IOException(const std::string& message) noexcept;

            ~IOException() noexcept override;

    }; /* class IOException */

} /* namespace adelie::exception */

#endif /* if !defined(__ADELIE_EXCEPTION_IOEXCEPTION_HXX__) */
