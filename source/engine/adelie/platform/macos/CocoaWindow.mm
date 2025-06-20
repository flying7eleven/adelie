#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#import <adelie/platform/macos/CocoaWindow.hxx>

using adelie::platform::CocoaWindow;

@interface WindowDelegate : NSObject <NSWindowDelegate>
@property(nonatomic, assign) bool shouldClose;
@end

@implementation WindowDelegate
- (instancetype)init {
    self = [super init];
    if (self) {
        _shouldClose = false;
    }
    return self;
}

- (BOOL)windowShouldClose:(NSWindow*)sender {
    _shouldClose = true;
    return YES;
}
@end

CocoaWindow::CocoaWindow() : window(nullptr), view(nullptr), delegate(nullptr), shouldCloseFlag(false) {
    // Ensure NSApplication is initialized
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
}

CocoaWindow::~CocoaWindow() {
    destroyWindow();
}

void CocoaWindow::createWindow(const int& width, const int& height, const char* title) {
    @autoreleasepool {
        NSRect screenRect = [[NSScreen mainScreen] frame];
        CGFloat x = NSMidX(screenRect) - static_cast<CGFloat>(width) / 2.f;
        CGFloat y = NSMidY(screenRect) - static_cast<CGFloat>(height) / 2.f;

        NSRect windowRect = NSMakeRect(x, y, width, height);
        window = [[NSWindow alloc] initWithContentRect:windowRect
                                             styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];

        delegate = [[WindowDelegate alloc] init];
        [(NSWindow*)window setDelegate:(WindowDelegate*)delegate];
        [(NSWindow*)window setTitle:@(title)];

        view = [[NSView alloc] initWithFrame:windowRect];
        [(NSWindow*)window setContentView:(NSView*)view];
        [(NSWindow*)window makeKeyAndOrderFront:nullptr];

        // Create a metal layer for MoltenVK
        CAMetalLayer* metalLayer = [CAMetalLayer layer];
        [(NSView*)view setWantsLayer:YES];
        [(NSView*)view setLayer:metalLayer];
    }
}

void CocoaWindow::destroyWindow() {
    if (window) {
        [(NSWindow*)window close];
        [(NSWindow*)window setDelegate:nullptr];
        window = nullptr;
    }

    if (delegate) {
        delegate = nullptr;
    }

    if (view) {
        view = nullptr;
    }
}

void* CocoaWindow::getNativeWindowHandle() const {
    return view;
}

void* CocoaWindow::getNativeDisplayHandle() const {
    return nullptr;  // Not needed for macOS
}

void CocoaWindow::getWindowSize(int& width, int& height) const {
    NSRect frame = [(NSView*)view frame];
    width = static_cast<int>(frame.size.width);
    height = static_cast<int>(frame.size.height);
}

bool CocoaWindow::shouldClose() const {
    return [(WindowDelegate*)delegate shouldClose];
}

void CocoaWindow::pollEvents() {
    @autoreleasepool {
        NSEvent* event;
        do {
            event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
            if (event) {
                [NSApp sendEvent:event];
            }
        } while (event);

        [NSApp updateWindows];
    }
}