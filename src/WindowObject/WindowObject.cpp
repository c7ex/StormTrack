#include "WindowObject.hpp"

const wchar_t* WindowObject::CLASS_NAME = L"FlowerStorm";

LRESULT CALLBACK WindowObject::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowObject* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<WindowObject*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<WindowObject*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->WindowProc(hwnd, msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool WindowObject::RegisterWindowClass() {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcStatic;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    return RegisterClassEx(&wc) != 0;
}

bool WindowObject::Create(int nCmdShow) {
    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (!hwnd) {
        return false;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    windowCreated = true;
    return true;
}

void WindowObject::ThreadProc(int nCmdShow) {
    if (!Create(nCmdShow)) {
        windowCreated = false;
        return;
    }

    SetTimer(hwnd, TimerId, TimerInterval, NULL);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

WindowObject::WindowObject(HINSTANCE hInst, const wchar_t* title)
    : hInstance(hInst), hwnd(nullptr), windowTitle(title),
    windowCreated(false), windowClosed(false) {

    RegisterWindowClass();

    graphState.InitializeWindowSize(800, 600);
    graphState.InitializeVisibleArea(1000.0, 2000.0);
    graphState.InitializeReferencePosition(0.0, -1000.0);
}

WindowObject::~WindowObject() {
    Close();
    if (windowThread.joinable()) {
        windowThread.join();
    }
}

bool WindowObject::Show(int nCmdShow) {
    if (windowThread.joinable()) {
        return false;
    }

    windowThread = std::thread(&WindowObject::ThreadProc, this, nCmdShow);

    while (!windowCreated && !windowClosed) {
        Sleep(10);
    }

    return windowCreated;
}

void WindowObject::Close() {
    if (hwnd && !windowClosed) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}

void WindowObject::WaitForClose() {
    if (windowThread.joinable()) {
        windowThread.join();
    }
}

bool WindowObject::IsValid() const {
    return (hwnd != nullptr) && (!windowClosed);
}

HWND WindowObject::GetHandle() const {
    return hwnd;
}