#include <iostream>
#include <thread>

#include <fcntl.h>
#include <io.h>
#include <stdint.h>
#include <iostream>
#include <string>

#include <fstream>

// SetConsoleOutputCP
#include <Windows.h>

#include "resource.h"

#include <filesystem>

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

void ClearCurrentLine(int Position)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD tl = { 0, (SHORT)(Position) };
	GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
	DWORD written, cells = csbi.dwSize.X;
	FillConsoleOutputCharacter(ConsoleHandle, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(ConsoleHandle, tl);
}

std::wstring MoveRight(std::wstring *string)
{
	wchar_t LastChar = (*string)[string->length() - 1];
	for (int ii = string->length() - 1; ii >= 0; ii--)
	{
		if (ii == 0)
			(*string)[0] = LastChar;
		else
			(*string)[ii] = (*string)[ii -1];
	}

	return *string;
}

std::wstring MoveLeft(std::wstring *string)
{
	wchar_t LastChar = (*string)[0];
	for (int ii = 0; ii <= string->length() - 1; ii++)
	{
		if (ii == string->length() - 1)
		{
			(*string)[ii] = LastChar;
		}
		else
			(*string)[ii] = (*string)[ii+1];
	}

	return *string;
}

bool FileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wstring FontFile = L"Resources\\CustomConsola.ttf";

#pragma region Extract font from resource
	std::filesystem::create_directory("Resources"); /* Make Resources Direcory */

	if (!FileExists("Resources\\CustomConsola.ttf")) /* Check if Font file already exists*/
	{
		HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);
		HRSRC ResourceHandle = FindResource(hResInstance, MAKEINTRESOURCE(IDR_CUSTOM_CONSOLA_FONT), L"XFONT");

		HGLOBAL ResourceMemory = LoadResource(hResInstance, ResourceHandle);

		void* ResourceData = LockResource(ResourceMemory);

		size_t ResourceLenght = SizeofResource(hResInstance, ResourceHandle);
		HANDLE hFile = CreateFile(L"Resources\\CustomConsola.ttf", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		DWORD bytesWritten = ResourceLenght;

		WriteFile(hFile, ResourceData, ResourceLenght, &bytesWritten, NULL);
		CloseHandle(hFile);
	}
#pragma endregion

#pragma region Add Font Resource
	if (AddFontResourceEx(FontFile.c_str(), NULL, NULL) == 0)
	{
		throw std::exception("Font add fails");
	}
#pragma endregion

#pragma region Make console use font
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

	//std::wstring bar = L"▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁";
	std::wstring bar = L"▁ ▂ ▃ ▄ ▅ ▆ ☐ ╳ ☐ ▆ ▅ ▄ ▃ ▂ ▁";

	int MidPosition = std::ceil((float)bar.length()/2); /* For tracking the middle character character */
	int TrueMid = std::ceil((float)bar.length() / 2); /* Middle absolute position */
	bool GoingRight = true; /* Tracking the direction in which the bar is going in */

	for (int i = 0; i < 100; i++)
	{
		if (MidPosition == 1 || MidPosition == bar.length())
			GoingRight = !GoingRight;
		if (GoingRight)
		{
			std::wcout << MoveRight(&bar) << std::endl;
			MidPosition++;
		}
		else
		{
			std::wcout << MoveLeft(&bar) << std::endl;
			MidPosition--;
		}

		int sleepTime = 0;
		if(MidPosition >= TrueMid)
			sleepTime = ((float)(MidPosition+1)/15)*50;
		else
		{
			int Difference = TrueMid - MidPosition;
			sleepTime = ((float)(TrueMid+Difference+1) / 15) * 50;
		}
		Sleep(sleepTime);
		ClearCurrentLine(0);
	}
		
	system("Pause");

#pragma region Remove Font Resource
	if (RemoveFontResourceEx(FontFile.c_str(), NULL, NULL) == 0)
	{
		throw std::exception("Font remove fails");
	}
#pragma endregion
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
*/