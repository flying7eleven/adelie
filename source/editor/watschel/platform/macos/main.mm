#import <AppKit/NSApplication.h>

#include <Adelie.hxx>
#include <boost/core/demangle.hpp>
#import <watschel/platform/macos/AppDelegate.hxx>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::exception::VulkanRuntimeException;

int main(int /*unused*/, char** /*unused*/) {
    @autoreleasepool {
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSApplication* application = [NSApplication sharedApplication];
        AppDelegate* appDelegate = [[[AppDelegate alloc] init] autorelease];

        [application setDelegate:appDelegate];

        try {
            auto window = WindowFactory::createWindow();
            window->createWindow(1920, 1080, "Watschel");
        } catch (const VulkanRuntimeException& e) {
            AdelieLogFatal("Exception ({}) [{}]: {}", boost::core::demangle(typeid(e).name()), e.getVulkanErrorCode(), e.getMessage());
            AdelieLogFatal("Stacktrace:");
            for (auto entry : e.getStacktrace()) {
                AdelieLogFatal("    {}", entry.name());
            }
            return EXIT_FAILURE;
        } catch (const RuntimeException& e) {
            AdelieLogFatal("Exception ({}): {}", boost::core::demangle(typeid(e).name()), e.getMessage());
            AdelieLogFatal("Stacktrace:");
            for (auto entry : e.getStacktrace()) {
                AdelieLogFatal("    {}", entry.name());
            }
            return EXIT_FAILURE;
        }

        [application run];
    }

    return EXIT_SUCCESS;
}