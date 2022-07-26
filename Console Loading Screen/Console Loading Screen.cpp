#include <iostream>
#include <thread>

#include <fcntl.h>
#include <io.h>
#include <stdint.h>
#include <iostream>
#include <string>

// SetConsoleOutputCP
#include <Windows.h>

#include "resource.h"

std::wstring GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
    {
        return std::wstring(); //No error message has been recorded
    }

    LPWSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::wstring message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

#pragma region Font to memory
    HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);

    HANDLE m_fonthandle = HANDLE();

    HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(IDR_CUSTOM_CONSOLA_FONT), RT_FONT);
    if (res)
    {
        HGLOBAL mem = LoadResource(hResInstance, res);
        void* data = LockResource(mem);
        size_t len = SizeofResource(hResInstance, res);

        DWORD nFonts;
        m_fonthandle = AddFontMemResourceEx(
            data,       // font resource
            len,       // number of bytes in font resource
            NULL,          // Reserved. Must be 0.
            &nFonts      // number of fonts installed
        );

        if (m_fonthandle == 0)
        {
            throw std::exception("Font add fails");
        }
    }
#pragma endregion

#pragma region Change Font
    CONSOLE_FONT_INFOEX cfi = { 0 };

    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Custom Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &cfi);
#pragma endregion

    for (int i = 0; i < 30; i++)
    {
        std::wcout << L"▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁" << std::endl;

    }


    system("pause");


#pragma region Remove font from memory
    if (m_fonthandle)
    {
        BOOL b = RemoveFontMemResourceEx(m_fonthandle);
        if (b == 0)
        {
            throw std::exception("Font remove fails");
        }
    }
#pragma endregion
    return 0;
}

/*▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁
█
▉
▊
▋
▌
▍
▎
▏
*/