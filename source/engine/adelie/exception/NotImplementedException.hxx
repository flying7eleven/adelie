// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_EXCEPTION_NOTIMPLEMENTEDEXCEPTION_HXX__)
    #define __ADELIE_EXCEPTION_NOTIMPLEMENTEDEXCEPTION_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/exception/RuntimeException.hxx>

namespace adelie::exception {

    class ADELIE_API NotImplementedException : public RuntimeException {
        public:
            NotImplementedException() noexcept;

            ~NotImplementedException() noexcept override;

    }; /* class adelie::exception */

}  // namespace adelie::exception

#endif /* if !defined(__ADELIE_EXCEPTION_NOTIMPLEMENTEDEXCEPTION_HXX__) */
