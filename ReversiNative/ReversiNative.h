#pragma once

#define REVERSI_PRINTBUFFERSIZE 253
#define REVERSI_SAVEBUFFERSIZE 8000

#ifdef __cplusplus

#ifdef _REVERSI_EXPORT_
#define REVERSI_API __declspec(dllexport)
#define REVERSI_API_C extern "C" __declspec(dllexport)
#else
#define REVERSI_API __declspec(dllimport)
#define REVERSI_API_C extern "C" __declspec(dllimport)
#endif

#include <iostream>

namespace Freiyer
{
	namespace Sandbox
	{
		class REVERSI_API Reversi
		{
		public:
			enum class PieceState : unsigned char
			{
				BlankPiece = 0,
				LightPiece,
				DarkPiece,
				PieceAvailable,
			};

			enum class State : unsigned char
			{
				InProgress = 1,
				DarkWon = 2,
				LightWon = 4,
				Draw = 6,
				NoAvailableSpaces = 9,
			};

			enum class Player : unsigned char
			{
				Dark = 1,
				Light = 2,
			};
		public:
			Reversi();
			Reversi(const unsigned char src[8000]);
			Reversi(const Reversi &);
			Reversi(Reversi &&);
			~Reversi();
		private:
			Player _CurrentPlayer; // Init: Dark
			unsigned char _CurrentTurn; // Init: 1
			unsigned char _BlankCount; // Init: 60
			unsigned char _LightCount; // Init: 2
			unsigned char _DarkCount; // Init: 2
			unsigned char _AvailableCount; // Init: 4
			unsigned char _StackSize; // Init: 1
			PieceState * _CurrentPieceState;
			PieceState * _PieceState;
			struct { unsigned char i, j; } *_PlaceHistory;
		public:
			// Returns which player won
			State GetState();
			// Prints the board
			void PrintState(std::wostream & o);
			// Copy current state to
			void CopyState(PieceState dst[8][8]);
			// Get current Player
			Player GetCurrentPlayer();
			// Get current turn
			int GetCurrentTurn();
			// Get blank count
			int GetBlankCount();
			// Get light count
			int GetLightCount();
			// Get dark count
			int GetDarkCount();
			// Get available spaces count
			int GetAvailableCount();
			// Get piece at
			PieceState GetPieceAt(int pi, int pj);
			// Save the state
			bool SaveState(unsigned char dst[8000]);
			// Load the state
			bool LoadState(const unsigned char src[8000]);
			// Undo
			bool Undo();
			// is able to undo
			bool IsUndoPossible();
			// Redo
			bool Redo();
			// is able to redo
			bool IsRedoPossible();
			// Places a piece
			bool PlacePiece(int pi, int pj);
			// Skip the turn
			bool SkipTurn();
		private:
			// Count the number of pieces
			void _CountPieces();
			// Toggle the current player
			void _TogglePlayer();
			// Check available spaces
			void _CheckAvailability();
			//
			void _InitializeAvailability();
			// Turn the pieces over
			void _TurnPiecesOver(int pi, int pj);
		};

	}
}

#else

#ifdef _REVERSI_EXPORT_
#define REVERSI_API_C extern __declspec(dllexport)
#else
#define REVERSI_API_C extern __declspec(dllimport)
#endif

typedef void * pReversi;

typedef unsigned char ReversiPieceState;
#define REVERSI_BLANKPIECE ((ReversiPieceState)0)
#define REVERSI_LIGHTPIECE ((ReversiPieceState)1)
#define REVERSI_DARKPIECE  ((ReversiPieceState)2)
#define REVERSI_PIECEAVAILABLE  ((ReversiPieceState)3)

typedef unsigned char ReversiState;
#define REVERSI_INPROGRESS ((ReversiState)1)
#define REVERSI_DARKWON ((ReversiState)2)
#define REVERSI_LIGHTWON ((ReversiState)4)
#define REVERSI_DRAW ((ReversiState)6)
#define REVERSI_NOAVAILABLESPACES ((ReversiState)9)

typedef unsigned char ReversiPlayer;
#define REVERSI_DARK ((ReversiState)1)
#define REVERSI_LIGHT ((ReversiState)2)

REVERSI_API_C pReversi NewReversiDefault();

REVERSI_API_C pReversi NewReversiExplicit(const unsigned char * src);

REVERSI_API_C pReversi NewReversiCopy(pReversi other);

REVERSI_API_C void DeleteReversi(pReversi instance);

REVERSI_API_C ReversiState GetState(pReversi instance);

REVERSI_API_C void PrintState(pReversi instance, char * dst, int resultLen);

REVERSI_API_C void CopyState(pReversi instance, ReversiPieceState * dst);

REVERSI_API_C ReversiPlayer GetCurrentPlayer(pReversi instance);

REVERSI_API_C int GetCurrentTurn(pReversi instance);

REVERSI_API_C int GetBlankCount(pReversi instance);

REVERSI_API_C int GetLightCount(pReversi instance);

REVERSI_API_C int GetAvailableCount(pReversi instance);

REVERSI_API_C int GetDarkCount(pReversi instance);

REVERSI_API_C ReversiPieceState GetPieceAt(pReversi instance, int pi, int pj);

REVERSI_API_C unsigned char SaveState(pReversi instance, unsigned char * dst);

REVERSI_API_C unsigned char LoadState(pReversi instance, const unsigned char * src);

REVERSI_API_C unsigned char Undo(pReversi instance);

REVERSI_API_C unsigned char IsUndoPossible(pReversi instance);

REVERSI_API_C unsigned char Redo(pReversi instance);

REVERSI_API_C unsigned char IsRedoPossible(pReversi instance);

REVERSI_API_C unsigned char PlacePiece(pReversi instance, int pi, int pj);

REVERSI_API_C unsigned char SkipTurn(pReversi instance);

#endif