#pragma once
#include <windows.h>
#include <thread>
#include <atomic>
#include <windowsx.h>

#include"GraphState.hpp"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

/*
    <StormTrack>
    description: 
        A Windows window wrapper class that runs in a separate thread. 
        This class encapsulates Windows API window creation and management,
        allowing windows to be created and controlled from console applications
        without blocking the main thread.

    Usage example:
        HINSTANCE hInstance = GetModuleHandle(nullptr);
        StormTrack window(hInstance, L"My Window");
        window.Show(); // Window runs in background thread
        window.Close(); // Close window
        window.WaitForClose(); // Wait for thread to finish

    Console remains interactive!

    note: The window runs in its own thread. Always call WaitForClose()
    before destroying the object if the window is still open.

    C++11 or later
 */

class StormTrack {
private:
    HWND hwnd;
    HINSTANCE hInstance;
    static const wchar_t* CLASS_NAME;
    const wchar_t* windowTitle;
    std::thread windowThread;
    std::atomic<bool> windowCreated;
    std::atomic<bool> windowClosed;

private:
    GraphState graphState;

private:
    const UINT_PTR TimerId = 1;
    UINT TimerInterval = 24;
    HBITMAP hBackBuffer = nullptr;
    int bufferWidth = 0, bufferHeight = 0;
    
private:
    static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool RegisterWindowClass();
    bool Create(int nCmdShow);
    void ThreadProc(int nCmdShow);

public:
    StormTrack(HINSTANCE hInst, const wchar_t* title = L"StormTrack");
    ~StormTrack();
    
    bool Show(int nCmdShow = SW_SHOWDEFAULT);
    void Close();
    void WaitForClose();
    bool IsValid() const;

    HWND GetHandle() const;

public:
    void StaticData(std::vector<double>& load_data, std::wstring caption, COLORREF color, double step = 1, double offset = 0) {
        graphState.AddData(load_data, caption, color, step, offset);
    }

    size_t AddStreamingTrace(std::wstring caption, COLORREF color, double step = 1, double offset = 0) {
        return graphState.CreateTrace(caption, color, step, offset);
    }

    bool Streaming(std::vector<double>& load_data, size_t trace_index) {
        return graphState.StreamUpdate(load_data, trace_index);
    }
};