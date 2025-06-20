// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_VERSIONINFORMATION_HXX__)
    #define __ADELIE_VERSIONINFORMATION_HXX__

    // required includes
    #include <adelie/adelie.hxx>
    #include <string>

namespace adelie {

    [[maybe_unused]] auto ADELIE_API getVersionMajor() -> unsigned short int;

    [[maybe_unused]] auto ADELIE_API getVersionMinor() -> unsigned short int;

    [[maybe_unused]] auto ADELIE_API getVersionPatch() -> unsigned short int;

    [[maybe_unused]] auto ADELIE_API getVersionSHA1() -> std::string;

    [[maybe_unused]] auto ADELIE_API getVersionString() -> std::string;

    [[maybe_unused]] auto ADELIE_API getBuildDate() -> std::string;

    [[maybe_unused]] auto ADELIE_API getBuildTime() -> std::string;

    [[maybe_unused]] auto ADELIE_API isDirty() -> bool;

}  // namespace adelie

#endif /* if !defined( __ADELIE_VERSIONINFORMATION_HXX__ ) */