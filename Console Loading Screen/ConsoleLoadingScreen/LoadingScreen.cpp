#include "LoadingScreen.h"



std::wstring LoadingScreen::FontFile = L"Resources\\CustomConsola.ttf";

void LoadingScreen::InitilizeFont()
{
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
}

void LoadingScreen::TerminateFont()
{
#pragma region Remove Font Resource
	if (RemoveFontResourceEx(FontFile.c_str(), NULL, NULL) == 0)
	{
		throw std::exception("Font remove fails");
	}
#pragma endregion
}

bool LoadingScreen::FileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}


void LoadingScreen::StartLoading()
{
	switch (BarType)
	{
	case Unknown:
		UnknownProgressLoad();
		break;
	case Known:
		KnownProgressLoad();
		break;
	}
}

void LoadingScreen::UpdateKnownProgressBar(float percentageDone)
{
	PercentageDone = percentageDone;
}

void LoadingScreen::KnownProgressLoad()
{
	std::thread FunctionThread([this] { this->ThreadingFunction(); });

	std::wstring bar = L"";
	int Lenght = 10;


	while (PercentageDone < 1 && !CrossThreadFinishBoolean)
	{

		float left = PercentageDone * Lenght;

		bar += std::wstring(floor(left), L'█');

		left -= floor(left);
		bar += std::wstring(floor(left / 0.5), L'▌');

		std::wcout << bar << std::endl;

		Sleep(100);
		LoadingScreen::ClearCurrentLine(0);
		bar = L"";
	}

	FunctionThread.join();
}


void LoadingScreen::UnknownProgressLoad()
{
	std::thread FunctionThread ([this] { this->ThreadingFunction(); });


	std::wstring bar = L"▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁";

	int MidPosition = std::ceil((float)bar.length() / 2); /* For tracking the middle character character */
	int TrueMid = std::ceil((float)bar.length() / 2); /* Middle absolute position */
	bool GoingRight = true; /* Tracking the direction in which the bar is going in */

	while (!CrossThreadFinishBoolean)
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
		if (MidPosition >= TrueMid)
			sleepTime = ((float)(MidPosition + 1) / 15) * 50;
		else
		{
			int Difference = TrueMid - MidPosition;
			sleepTime = ((float)(TrueMid + Difference + 1) / 15) * 50;
		}
		Sleep(sleepTime);
		ClearCurrentLine(0);
	}

	FunctionThread.join();
}

void LoadingScreen::ThreadingFunction()
{
	(*LoadingFunction)(this);
	(CrossThreadFinishBoolean) = !(CrossThreadFinishBoolean);
}

void LoadingScreen::ClearCurrentLine(int Position)
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

std::wstring LoadingScreen::MoveRight(std::wstring* string)
{
	wchar_t LastChar = (*string)[string->length() - 1];
	for (int ii = string->length() - 1; ii >= 0; ii--)
	{
		if (ii == 0)
			(*string)[0] = LastChar;
		else
			(*string)[ii] = (*string)[ii - 1];
	}

	return *string;
}

std::wstring LoadingScreen::MoveLeft(std::wstring* string)
{
	wchar_t LastChar = (*string)[0];
	for (int ii = 0; ii <= string->length() - 1; ii++)
	{
		if (ii == string->length() - 1)
		{
			(*string)[ii] = LastChar;
		}
		else
			(*string)[ii] = (*string)[ii + 1];
	}

	return *string;
}
