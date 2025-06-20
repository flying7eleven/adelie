#import <AppKit/NSApplication.h>

#include <Adelie.hxx>
#import <watschel/platform/macos/AppDelegate.hxx>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;

int main(int /*unused*/, char** /*unused*/) {
    @autoreleasepool {
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSApplication* application = [NSApplication sharedApplication];
        AppDelegate* appDelegate = [[[AppDelegate alloc] init] autorelease];

        [application setDelegate:appDelegate];

        auto window = WindowFactory::createWindow();
        window->createWindow(1920, 1080, "Watschel");

        [application run];
    }

    return EXIT_SUCCESS;
}