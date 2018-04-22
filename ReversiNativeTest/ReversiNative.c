#include "ReversiNative.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
	char i, j;
	puts("");

	char printBuffer[REVERSI_PRINTBUFFERSIZE];
	
	pReversi reversi = NewReversiDefault();
	while ((int)GetState(reversi) & 1)
	{
		PrintState(reversi, printBuffer, sizeof(printBuffer));
		fputs(printBuffer, stdout);
		fflush(stdin);
		while (isspace(i = getc(stdin)));
		while (isspace(j = getc(stdin)));
		system("cls");
		if (islower(i))
			i -= ' ';
		if (islower(j))
			j -= ' ';
		if (i == 'Z' && j == 'Z')
		{
			if (!Undo(reversi))
				puts("Cannot undo.");
			else
				puts("Undid.");
		}
		else if (i == 'Y' && j == 'Y')
		{
			if (!Redo(reversi))
				puts("Cannot redo.");
			else
				puts("Redid.");
		}
		else if (i == 'S' && j == 'S')
		{
			if (!SkipTurn(reversi))
				puts("Cannot skip the turn.");
			else
				puts("Skipped.");
		}
		else if (!isdigit(i) || !isalpha(j))
		{
			puts("Invalid command.");
		}
		else
		{
			if (!PlacePiece(reversi, i - L'1', j - L'A'))
			{
				puts("Cannot place.");
			}
			else
			{
				if (GetState(reversi) == REVERSI_NOAVAILABLESPACES)
					puts("No available spaces.");
				else
					puts("");
			}
		}
	}
	PrintState(reversi, printBuffer, sizeof(printBuffer));
	fputs(printBuffer, stdout);

	DeleteReversi(reversi);
}