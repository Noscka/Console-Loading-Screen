#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>
#include <iostream>

#include "../resource.h"

class LoadingScreen
{
private:
	static std::wstring FontFile;
public:
	static void InitilizeFont();
	static void TerminateFont();
	static bool FileExists(const std::string& name);

	enum LoadType
	{
		Unknown = 0,
		Known = 1,
	};

	LoadType BarType;

	LoadingScreen(LoadType barType)
	{
		BarType = barType;
	}

	void StartLoading();

	void Loadingbar();

	void BasicBar();

	void ClearCurrentLine(int Position);

	std::wstring MoveRight(std::wstring* string);

	std::wstring MoveLeft(std::wstring* string);

};