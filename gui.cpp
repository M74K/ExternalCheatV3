#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "settings.h"

#include <filesystem>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter
);

LRESULT CALLBACK WindowProcess(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(
        window,
        message,
        wideParameter,
        longParameter))
    {
        return TRUE;
    }

    switch (message)
    {
    case WM_SIZE:
    {
        if (gui::device && wideParameter != SIZE_MINIMIZED)
        {
            gui::presentParameters.BackBufferWidth =
                LOWORD(longParameter);

            gui::presentParameters.BackBufferHeight =
                HIWORD(longParameter);

            gui::ResetDevice();
        }

        return 0;
    }

    case WM_SYSCOMMAND:
    {
        if ((wideParameter & 0xfff0) == SC_KEYMENU)
            return 0;

        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        gui::position = MAKEPOINTS(longParameter);
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        if (wideParameter == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = RECT{};

            GetWindowRect(gui::window, &rect);

            rect.left += points.x - gui::position.x;
            rect.top += points.y - gui::position.y;

            if (
                gui::position.x >= 0 &&
                gui::position.x <= gui::WIDTH &&
                gui::position.y >= 0 &&
                gui::position.y <= 19)
            {
                SetWindowPos(
                    gui::window,
                    HWND_TOPMOST,
                    rect.left,
                    rect.top,
                    0,
                    0,
                    SWP_SHOWWINDOW |
                    SWP_NOSIZE |
                    SWP_NOZORDER
                );
            }
        }

        return 0;
    }
    }

    return DefWindowProcA(
        window,
        message,
        wideParameter,
        longParameter
    );
}

void gui::CreateHWindow(
    const char* windowName,
    const char* className) noexcept
{
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.style = CS_CLASSDC;

    windowClass.lpfnWndProc = WindowProcess;

    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;

    windowClass.hInstance = GetModuleHandleA(nullptr);

    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;

    windowClass.lpszClassName = className;

    windowClass.hIconSm = nullptr;

    RegisterClassExA(&windowClass);

    window = CreateWindowA(
        className,
        windowName,
        WS_POPUP,
        100,
        100,
        WIDTH,
        HEIGHT,
        nullptr,
        nullptr,
        windowClass.hInstance,
        nullptr
    );

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

void gui::DestroyWindow() noexcept
{
    ::DestroyWindow(window);

    UnregisterClassA(
        windowClass.lpszClassName,
        windowClass.hInstance
    );

    window = nullptr;
}
