#include <iostream>
#include <fcntl.h>
#include <io.h>

#include "ConsoleLoadingScreen/LoadingScreen.hpp"

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

void LongFunctionTing(LoadingScreen* Object)
{
	for (int i = 0; i <= 1000; i++)
	{
		Sleep(10);
		Object->UpdateKnownProgressBar((float)i/1000.0, L"Ting");
	}
}

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	LoadingScreen::InitilizeFont();

	std::wstring splash = LR"(
                          ████████                ███████
                        ▄██▀    ▀██▄ ▄███████▄  ███▀   ▀████████▄
              ▄███████████▌      ██████     ▀█████       ███     ▀▀███▄
         ▄██▀▀         ██▌        ████       ████▌       ███           ▀▀███▄
       ██▀            ███         ███▌       ▐███        ▐██▄               ▀▀███▄
     ██▀       ███    ███         ███▌       ▐███        ▐████▀                  ▀██
    ██▌       ▀███▄▄▄▄███         ███        ▐███        ████▌                     ██
    ██▌               ██▌         ███        ▐███        ███▌          ████▄▄     ▄██
    ▀██▄              ██▌         ███        ▐███        ███          ███    ▀█████▀
      ▀██████████████▄███         ███        ████       ███          ███
        ██▀       ████▀██         ███        ▐██▌      ▐██▌          ██▌
       ███             ██▌        ██▌         ██       ███▌         ███
       ███             ▐██                            █████▄       ███
        ▀██▄▄       ▄▄▄████▄                         ███   ▀███▄▄███▀
           ▀▀▀███▀▀▀▀    ▀██▄         ▄██▄         ▄██▀
                           ▀███▄▄▄▄▄███▀████▄▄▄▄▄███▀
                               ▀▀▀▀▀        ▀▀▀▀▀)";

	LoadingScreen basic(LoadingScreen::LoadType::Known, splash, &LongFunctionTing);
	
		
	system("Pause");
	LoadingScreen::TerminateFont();
	return 0;
}

/*
▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁
█						    █
▉						    ▉
▊						    ▊
▋						    ▋
▌						    ▌
▍						    ▍
▎						    ▎
▏						    ▏
████████████████████████████*/