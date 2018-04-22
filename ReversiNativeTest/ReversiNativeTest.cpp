#include "ReversiNative.h"

#include <iostream>
#include <sstream>
#include <cctype>

#include <Windows.h>

using namespace Freiyer::Sandbox;

unsigned char _SaveBuffer[REVERSI_SAVEBUFFERSIZE];

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
		return TRUE;
	case CTRL_CLOSE_EVENT:
		return TRUE;
	case CTRL_BREAK_EVENT:
		return FALSE;
	case CTRL_LOGOFF_EVENT:
		return FALSE;
	case CTRL_SHUTDOWN_EVENT:
		return FALSE;
	default:
		return FALSE;
	}
}

int main()
{
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
	{
		std::wcout << L"Couldn't add Ctrl Handler." << std::endl;
	}
	else
	{
		std::wcout << std::endl;
	}

	std::ios_base::sync_with_stdio(false);
	wchar_t i, j;

	Reversi * reversi = new Reversi;
	while ((int)reversi->GetState() & 1)
	{
		reversi->PrintState(std::wcout);
		std::wcin >> i >> j;
		system("cls");
		if (islower(i))
			i -= L' ';
		if (islower(j))
			j -= L' ';
		if (i == L'Z' && j == L'Z')
		{
			if (!reversi->Undo())
				std::wcout << L"Cannot undo." << std::endl;
			else
				std::wcout << L"Undid." << std::endl;
		}
		else if (i == L'Y' && j == L'Y')
		{
			if (!reversi->Redo())
				std::wcout << L"Cannot redo." << std::endl;
			else
				std::wcout << L"Redid." << std::endl;
		}
		else if (i == L'S' && j == L'S')
		{
			if (!reversi->SkipTurn())
				std::wcout << L"Cannot skip the turn." << std::endl;
			else
				std::wcout << L"Skipped." << std::endl;
		}
		else if (i == L'Q' && j == L'Q')
		{
			if (!reversi->SaveState(_SaveBuffer))
				std::wcout << L"Cannot save." << std::endl;
			else
				std::wcout << L"Saved." << std::endl;
		}
		else if (i == L'W' && j == L'W')
		{
			if (!reversi->LoadState(_SaveBuffer))
				std::wcout << L"Cannot load." << std::endl;
			else
				std::wcout << L"Loaded" << std::endl;
		}
		else if (!isdigit(i) || !isalpha(j))
		{
			std::wcout << L"Invalid command." << std::endl;
		}
		else
		{
			if (!reversi->PlacePiece(i - L'1', j - L'A'))
			{
				std::wcout << L"Cannot place." << std::endl;
			}
			else
			{
				if (reversi->GetState() == Reversi::State::NoAvailableSpaces)
					std::wcout << L"No available spaces." << std::endl;
				else
					std::wcout << std::endl;
			}
		}
	}
	reversi->PrintState(std::wcout);

	delete reversi;
}