// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include "Win32Window.hxx"

#include <adelie/exception/RuntimeException.hxx>
#include <string>

using adelie::exception::RuntimeException;
using adelie::platform::Win32Window;

Win32Window::Win32Window() : hwnd(nullptr), shouldCloseFlag(false) {
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"VulkanWindowClass";

    if (!RegisterClassEx(&wc)) {
        throw RuntimeException("Failed to register window class");
    }
}

Win32Window::~Win32Window() {
    destroyWindow();
    UnregisterClass(L"VulkanWindowClass", GetModuleHandle(nullptr));
}

void Win32Window::createWindow(const int& width, const int& height, const char* title) {
    // Convert title to wide string
    int titleLength = MultiByteToWideChar(CP_UTF8, 0, title, -1, nullptr, 0);
    std::wstring wideTitle(titleLength, 0);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wideTitle.data(), titleLength);

    // Create window
    hwnd = CreateWindowEx(0, L"VulkanWindowClass", wideTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, GetModuleHandle(nullptr), this);

    if (!hwnd) {
        throw RuntimeException("Failed to create window");
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

void Win32Window::destroyWindow() {
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

void* Win32Window::getNativeWindowHandle() const {
    return hwnd;
}

void* Win32Window::getNativeDisplayHandle() const {
    return GetModuleHandle(nullptr);
}

void Win32Window::getWindowSize(int& width, int& height) const {
    RECT rect;
    GetClientRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

bool Win32Window::shouldClose() const {
    return shouldCloseFlag;
}

void Win32Window::pollEvents() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Win32Window::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Win32Window* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        window = (Win32Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (Win32Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (window) {
        switch (uMsg) {
            case WM_CLOSE:
                window->shouldCloseFlag = true;
                return 0;
            case WM_SIZE:
                // Handle window resize if needed
                break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}