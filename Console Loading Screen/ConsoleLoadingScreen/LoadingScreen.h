#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <functional>
#include <thread>

#include "../resource.h"

class LoadingScreen
{
private:
	static std::wstring FontFile;
public:
	static void InitilizeFont();
	static void TerminateFont();
	static bool FileExists(const std::string& name);
	static void ClearCurrentLine(int Position);

	enum LoadType
	{
		Unknown = 0,
		Known = 1,
	};

	LoadType BarType;
	void (*LoadingFunction)(LoadingScreen*);

	bool CrossThreadFinishBoolean;

	LoadingScreen(LoadType barType, void (*Function)(LoadingScreen*))
	{
		CrossThreadFinishBoolean = false;
		BarType = barType;
		LoadingFunction = Function;
	}

	void StartLoading();

	void KnownProgressLoad(float percentageDone);

	void UnknownProgressLoad();

	void ThreadingFunction();


	std::wstring MoveRight(std::wstring* string);

	std::wstring MoveLeft(std::wstring* string);

};