#include "ReversiNative.h"

#include <cstring>
#include <sstream>
#include <iomanip>

#define SIZE 8
#define MAXTURN 121

namespace Freiyer
{
	namespace Sandbox
	{
		Reversi::Reversi()
			:
			_CurrentPlayer(Player::Dark),
			_CurrentTurn(1),
			_BlankCount(60),
			_DarkCount(2),
			_LightCount(2),
			_AvailableCount(4),
			_StackSize(1),
			_PieceState(new PieceState[MAXTURN * SIZE * SIZE]),
			_PlaceHistory(
				new std::remove_reference<decltype(*_PlaceHistory)>::type[MAXTURN]
			)
		{
			_CurrentPieceState = _PieceState;
			_PlaceHistory[0].i = 0;
			_PlaceHistory[0].j = 0;
			memset(_PieceState, 0, sizeof(PieceState) * MAXTURN * SIZE * SIZE);
			_CurrentPieceState[2 * SIZE + 3] = PieceState::PieceAvailable;
			_CurrentPieceState[3 * SIZE + 2] = PieceState::PieceAvailable;
			_CurrentPieceState[3 * SIZE + 3] = PieceState::LightPiece;
			_CurrentPieceState[3 * SIZE + 4] = PieceState::DarkPiece;
			_CurrentPieceState[4 * SIZE + 3] = PieceState::DarkPiece;
			_CurrentPieceState[4 * SIZE + 4] = PieceState::LightPiece;
			_CurrentPieceState[4 * SIZE + 5] = PieceState::PieceAvailable;
			_CurrentPieceState[5 * SIZE + 4] = PieceState::PieceAvailable;
		}

		Reversi::Reversi(const unsigned char src[8000])
			:
			_CurrentPlayer(Player::Dark),
			_CurrentTurn(1),
			_BlankCount(60),
			_DarkCount(2),
			_LightCount(2),
			_AvailableCount(4),
			_StackSize(1),
			_PieceState(new PieceState[MAXTURN * SIZE * SIZE])
		{
			LoadState(src);
		}

		Reversi::Reversi(const Reversi & other)
			:
			_CurrentPlayer(other._CurrentPlayer),
			_CurrentTurn(other._CurrentTurn),
			_BlankCount(other._BlankCount),
			_DarkCount(other._DarkCount),
			_LightCount(other._LightCount),
			_AvailableCount(other._AvailableCount),
			_StackSize(other._StackSize),
			_PieceState(new PieceState[MAXTURN * SIZE * SIZE])
		{
			memcpy(_PieceState, other._PieceState, sizeof(PieceState) * MAXTURN * SIZE * SIZE);
			_CurrentPieceState = _PieceState + (other._CurrentPieceState - other._PieceState);
		}

		Reversi::Reversi(Reversi && other)
			:
			_CurrentPlayer(other._CurrentPlayer),
			_CurrentTurn(other._CurrentTurn),
			_BlankCount(other._BlankCount),
			_DarkCount(other._DarkCount),
			_LightCount(other._LightCount),
			_AvailableCount(other._AvailableCount),
			_StackSize(other._StackSize),
			_PieceState(other._PieceState),
			_CurrentPieceState(other._CurrentPieceState)
		{
			memset(&other, 0, sizeof(Reversi));
		}


		Reversi::~Reversi()
		{
			delete[] _PieceState;
		}

		Reversi::State Reversi::GetState()
		{
			if (_BlankCount)
			{
				if (_AvailableCount)
					return State::InProgress;
				else
					return State::NoAvailableSpaces;
			}
			else
			{
				if (_LightCount == _DarkCount)
					return State::Draw;
				else if (_LightCount > _DarkCount)
					return State::LightWon;
				else
					return State::DarkWon;
			}
		}

		// Get light count

		int Reversi::GetLightCount()
		{
			return _LightCount;
		}

		// Get dark count

		int Reversi::GetDarkCount()
		{
			return _DarkCount;
		}

		// Get available spaces count

		int Reversi::GetAvailableCount()
		{
			return _AvailableCount;
		}

		Reversi::PieceState Reversi::GetPieceAt(int pi, int pj)
		{
			return _CurrentPieceState[pi * SIZE + pj];
		}

		bool Reversi::SaveState(unsigned char dst[8000])
		{
			const unsigned char header[] = "REVERSI";
			strcpy_s((char *)dst, sizeof(header), (const char *)header);
			dst[7] = unsigned char(_CurrentPlayer);
			dst[8] = _CurrentTurn;
			dst[9] = _BlankCount;
			dst[10] = _LightCount;
			dst[11] = _DarkCount;
			dst[12] = _AvailableCount;
			dst[13] = _StackSize;
			memcpy(dst + 14, _PieceState, sizeof(PieceState) * MAXTURN * SIZE * SIZE);
			memcpy(dst + 14 + sizeof(PieceState) * MAXTURN * SIZE * SIZE, _PlaceHistory, sizeof(*_PlaceHistory) * MAXTURN);
			return true;
		}

		bool Reversi::LoadState(const unsigned char src[8000])
		{
			const unsigned char header[] = "REVERSI";
			for (unsigned char i = 0; i < 7; i++)
				if (src[i] != header[i])
					return false;
			_CurrentPlayer = Player(src[7]);
			_CurrentTurn = src[8];
			_BlankCount = src[9];
			_LightCount = src[10];
			_DarkCount = src[11];
			_AvailableCount = src[12];
			_StackSize = src[13];
			memcpy(_PieceState, src + 14, sizeof(PieceState) * MAXTURN * SIZE * SIZE);
			memcpy(_PlaceHistory, src + 14 + sizeof(PieceState) * MAXTURN * SIZE * SIZE, sizeof(*_PlaceHistory) * MAXTURN);
			_CurrentPieceState = _PieceState + _CurrentTurn - 1;
			return true;
		}

		// Prints the board

		void Reversi::PrintState(std::wostream & o)
		{
			o << L"Turn " << std::setw(2) << _CurrentTurn
				<< std::setw(8) << (_CurrentPlayer == Player::Dark ? L"  Dark  " : L"  Light ")
				<< std::setw(2) << _DarkCount << L':'
				<< std::setw(2) << _LightCount << L'\n'
				<< L"....................\n"
				<< L".  A B C D E F G H .\n";
			for (int i = 0; i < 8; i++)
			{
				o << L'.' << i + 1 << L' ';
				for (int j = 0; j < 8; j++)
				{
					switch (_CurrentPieceState[i * SIZE + j])
					{
					case PieceState::BlankPiece:
						if (j == SIZE - 1)
							o << L"+ ";
						else
							o << L"+-";
						break;
					case PieceState::LightPiece:
						o << L"o ";
						break;
					case PieceState::DarkPiece:
						o << L"* ";
						break;
					case PieceState::PieceAvailable:
						o << L"##";
						break;
					}
				}
				o << L".\n";
			}
			o << L"...................." << std::endl;
		}

		// Copy current state to

		void Reversi::CopyState(PieceState dst[8][8])
		{
			memcpy(dst, _CurrentPieceState, sizeof(PieceState) * SIZE * SIZE);
		}

		// Get current Player

		Reversi::Player Reversi::GetCurrentPlayer()
		{
			return _CurrentPlayer;
		}

		// Get current turn

		int Reversi::GetCurrentTurn()
		{
			return _CurrentTurn;
		}

		// Get blank count

		int Reversi::GetBlankCount()
		{
			return _BlankCount;
		}

		bool Reversi::Undo()
		{
			if (_CurrentTurn > 1)
			{
				_TogglePlayer();
				_CurrentTurn--;
				_CurrentPieceState = _CurrentPieceState - SIZE * SIZE;
				_CountPieces();
				return true;
			}
			else
				return false;
		}

		bool Reversi::IsUndoPossible()
		{
			return _CurrentTurn > 1;
		}

		bool Reversi::Redo()
		{
			if (_CurrentTurn < _StackSize)
			{
				_TogglePlayer();
				_CurrentTurn++;
				_CurrentPieceState = _CurrentPieceState + SIZE * SIZE;
				_CountPieces();
				return true;
			}
			else
				return false;
		}

		bool Reversi::IsRedoPossible()
		{
			return _CurrentTurn < _StackSize;
		}

		void Reversi::_InitializeAvailability()
		{
			// Initializing the piece-spacing validity.
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					if (_CurrentPieceState[i * SIZE + j] == PieceState::PieceAvailable)
						_CurrentPieceState[i * SIZE + j] = PieceState::BlankPiece;
				}
			}
		}

		void Reversi::_TurnPiecesOver(int pi, int pj)
		{
			// Place a piece
			_CurrentPieceState[pi * SIZE + pj] = (_CurrentPlayer == Player::Dark ?
				PieceState::DarkPiece : PieceState::LightPiece
				);

			_InitializeAvailability();

			// Turning over the pieces
			// A variable that indicates the position of the piece on the other side
			int otherSidePiece = -1;
			// Check the up
			for (int i = pi - 1; i >= 0; i--) {
				if (_CurrentPieceState[i * SIZE + pj] == PieceState::BlankPiece)
				{
					otherSidePiece = -1;
					break;
				}
				if (_CurrentPieceState[i * SIZE + pj] == (_CurrentPlayer == Player::Dark ?
					PieceState::DarkPiece : PieceState::LightPiece))
				{
					otherSidePiece = i;
					break;
				}
			}
			if (otherSidePiece != -1) {
				for (int i = otherSidePiece + 1; i < pi; i++)
					_CurrentPieceState[i * SIZE + pj] = (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece);
				otherSidePiece = -1;
			}
			// Check the down
			for (int i = pi + 1; i < SIZE; i++) {
				if (_CurrentPieceState[i * SIZE + pj] == PieceState::BlankPiece) {
					otherSidePiece = -1;
					break;
				}
				if (_CurrentPieceState[i * SIZE + pj] == (_CurrentPlayer == Player::Dark ?
					PieceState::DarkPiece : PieceState::LightPiece)) {
					otherSidePiece = i;
					break;
				}
			}
			if (otherSidePiece != -1) {
				for (int i = otherSidePiece - 1; i > pi; i--)
					_CurrentPieceState[i * SIZE + pj] = (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece);
				otherSidePiece = -1;
			}
			// Check the left
			for (int j = pj - 1; j >= 0; j--) {
				if (_CurrentPieceState[pi * SIZE + j] == PieceState::BlankPiece) {
					otherSidePiece = -1;
					break;
				}
				if (_CurrentPieceState[pi * SIZE + j] == (_CurrentPlayer == Player::Dark ?
					PieceState::DarkPiece : PieceState::LightPiece)) {
					otherSidePiece = j;
					break;
				}
			}
			if (otherSidePiece != -1) {
				for (int j = otherSidePiece + 1; j < pj; j++)
					_CurrentPieceState[pi * SIZE + j] = (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece);
				otherSidePiece = -1;
			}
			// Check the right
			for (int j = pj + 1; j < SIZE; j++) {
				if (_CurrentPieceState[pi * SIZE + j] == PieceState::BlankPiece) {
					otherSidePiece = -1;
					break;
				}
				if (_CurrentPieceState[pi * SIZE + j] == (_CurrentPlayer == Player::Dark ?
					PieceState::DarkPiece : PieceState::LightPiece)) {
					otherSidePiece = j;
					break;
				}
			}
			if (otherSidePiece != -1) {
				for (int j = otherSidePiece - 1; j > pj; j--)
					_CurrentPieceState[pi * SIZE + j] = (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece);
				otherSidePiece = -1;
			}
			// Check the up-left
			{
				int i = 1;
				while (true) {
					if ((pi - i) < 0 || pj - i < 0)
						break;
					if (_CurrentPieceState[(pi - i) * SIZE + pj - i] == PieceState::BlankPiece) {
						otherSidePiece = -1;
						break;
					}
					if (_CurrentPieceState[(pi - i) * SIZE + pj - i] == (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece)) {
						otherSidePiece = i;
						break;
					}
					i++;
				}
				if (otherSidePiece != -1) {
					for (int j = i - 1; j > 0; j--)
						_CurrentPieceState[(pi - j) * SIZE + pj - j] = (_CurrentPlayer == Player::Dark ?
							PieceState::DarkPiece : PieceState::LightPiece);
					otherSidePiece = -1;
				}
			}
			// Check the up-right
			{
				int i = 1;
				while (true) {
					if ((pi - i) < 0 || pj + i >= SIZE)
						break;
					if (_CurrentPieceState[(pi - i) * SIZE + pj + i] == PieceState::BlankPiece) {
						otherSidePiece = -1;
						break;
					}
					if (_CurrentPieceState[(pi - i) * SIZE + pj + i] == (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece)) {
						otherSidePiece = i;
						break;
					}
					i++;
				}
				if (otherSidePiece != -1) {
					for (int j = i - 1; j > 0; j--)
						_CurrentPieceState[(pi - j) * SIZE + pj + j] = (_CurrentPlayer == Player::Dark ?
							PieceState::DarkPiece : PieceState::LightPiece);
					otherSidePiece = -1;
				}
			}
			// Check the down-left
			{
				int i = 1;
				while (true) {
					if ((pi + i) >= SIZE || pj - i < 0)
						break;
					if (_CurrentPieceState[(pi + i) * SIZE + pj - i] == PieceState::BlankPiece) {
						otherSidePiece = -1;
						break;
					}
					if (_CurrentPieceState[(pi + i) * SIZE + pj - i] == (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece)) {
						otherSidePiece = i;
						break;
					}
					i++;
				}
				if (otherSidePiece != -1) {
					for (int j = i - 1; j > 0; j--)
						_CurrentPieceState[(pi + j) * SIZE + pj - j] = (_CurrentPlayer == Player::Dark ?
							PieceState::DarkPiece : PieceState::LightPiece);
					otherSidePiece = -1;
				}
			}
			// Check the down-right
			{
				int i = 1;
				while (true) {
					if ((pi + i) >= SIZE || pj + i >= SIZE)
						break;
					if (_CurrentPieceState[((pi + i)) * SIZE + pj + i] == PieceState::BlankPiece) {
						otherSidePiece = -1;
						break;
					}
					if (_CurrentPieceState[((pi + i)) * SIZE + pj + i] == (_CurrentPlayer == Player::Dark ?
						PieceState::DarkPiece : PieceState::LightPiece)) {
						otherSidePiece = i;
						break;
					}
					i++;
				}
				if (otherSidePiece != -1) {
					for (int j = i - 1; j > 0; j--)
						_CurrentPieceState[(pi + j) * SIZE + pj + j] = (_CurrentPlayer == Player::Dark ?
							PieceState::DarkPiece : PieceState::LightPiece);
					otherSidePiece = -1;
				}
			}
		}

		bool Reversi::PlacePiece(int pi, int pj)
		{
			// If the program attempts to place a piece on the outside of the board,
			// the function throws an exception.
			if (pi >= SIZE || pj >= SIZE || pi < 0 || pj < 0 || _BlankCount == 0)
			{
				return false;
			}

			// If the program attempts to place a piece on an invalid square,
			// the function throws an exception.
			if (_CurrentPieceState[pi * SIZE + pj] != PieceState::PieceAvailable) {
				return false;
			}

			memcpy(_CurrentPieceState + SIZE * SIZE, _CurrentPieceState, sizeof(PieceState) * SIZE * SIZE);
			_CurrentPieceState = _CurrentPieceState + SIZE * SIZE;

			_TurnPiecesOver(pi, pj);
			_CheckAvailability();

			// Count the number of pieces
			_CountPieces();

			// Toggle Player
			_TogglePlayer();

			_CurrentTurn++;
			_StackSize = _CurrentTurn;

			return true;
		}

		bool Reversi::SkipTurn()
		{
			if (GetState() == State::NoAvailableSpaces && _BlankCount > 0)
			{
				_CheckAvailability();
				_CountPieces();
				_TogglePlayer();
				_CurrentTurn++;
				_StackSize = _CurrentTurn;
				if (_AvailableCount == 0)
					_BlankCount = 0;
				return true;
			}
			else
				return false;
		}

		void Reversi::_CheckAvailability()
		{
			int otherSidePiece = -1;

			// Check valid square
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					if (_CurrentPieceState[i * SIZE + j] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
						// Check the up
						for (int k = i - 1; k >= 0; k--) {
							if (_CurrentPieceState[k * SIZE + j] == PieceState::BlankPiece)
								break;
							else if (_CurrentPieceState[k * SIZE + j] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
								otherSidePiece = k - 1;
							else if (_CurrentPieceState[k * SIZE + j] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
								otherSidePiece = -1;
								break;
							}
						}
						if (otherSidePiece != -1) {
							_CurrentPieceState[otherSidePiece * SIZE + j] = PieceState::PieceAvailable;
							otherSidePiece = -1;
						}
						// Check the down
						for (int k = i + 1; k < SIZE; k++) {
							if (_CurrentPieceState[k * SIZE + j] == PieceState::BlankPiece)
								break;
							else if (_CurrentPieceState[k * SIZE + j] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
								otherSidePiece = k + 1;
							else if (_CurrentPieceState[k * SIZE + j] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
								otherSidePiece = -1;
								break;
							}
						}
						if (otherSidePiece != -1) {
							_CurrentPieceState[otherSidePiece * SIZE + j] = PieceState::PieceAvailable;
							otherSidePiece = -1;
						}
						// Check the left
						for (int k = i - 1; k >= 0; k--) {
							if (_CurrentPieceState[i * SIZE + k] == PieceState::BlankPiece)
								break;
							else if (_CurrentPieceState[i * SIZE + k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
								otherSidePiece = k - 1;
							else if (_CurrentPieceState[i * SIZE + k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
								otherSidePiece = -1;
								break;
							}
						}
						if (otherSidePiece != -1) {
							_CurrentPieceState[i * SIZE + otherSidePiece] = PieceState::PieceAvailable;
							otherSidePiece = -1;
						}
						// Check the right
						for (int k = i + 1; k < SIZE; k++) {
							if (_CurrentPieceState[i * SIZE + k] == PieceState::BlankPiece)
								break;
							else if (_CurrentPieceState[i * SIZE + k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
								otherSidePiece = k + 1;
							else if (_CurrentPieceState[i * SIZE + k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
								otherSidePiece = -1;
								break;
							}
						}
						if (otherSidePiece != -1) {
							_CurrentPieceState[i * SIZE + otherSidePiece] = PieceState::PieceAvailable;
							otherSidePiece = -1;
						}
						// Check the up-left
						{
							int k = 1;
							while (true) {
								if (i - k < 0 || j - k < 0) {
									otherSidePiece = -1;
									break;
								}
								else if (_CurrentPieceState[(i - k) * SIZE + j - k] == PieceState::BlankPiece)
									break;
								else if (_CurrentPieceState[(i - k) * SIZE + j - k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
									otherSidePiece = k + 1;
								else if (_CurrentPieceState[(i - k) * SIZE + j - k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
									otherSidePiece = -1;
									break;
								}
								k++;
							}
							if (otherSidePiece != -1) {
								_CurrentPieceState[(i - otherSidePiece) * SIZE + j - otherSidePiece] = PieceState::PieceAvailable;
								otherSidePiece = -1;
							}
						}
						// Check the up-right
						{
							int k = 1;
							while (true) {
								if (i - k < 0 || j + k >= SIZE) {
									otherSidePiece = -1;
									break;
								}
								else if (_CurrentPieceState[(i - k) * SIZE + j + k] == PieceState::BlankPiece)
									break;
								else if (_CurrentPieceState[(i - k) * SIZE + j + k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
									otherSidePiece = k + 1;
								else if (_CurrentPieceState[(i - k) * SIZE + j + k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
									otherSidePiece = -1;
									break;
								}
								k++;
							}
							if (otherSidePiece != -1) {
								_CurrentPieceState[(i - otherSidePiece) * SIZE + j + otherSidePiece] = PieceState::PieceAvailable;
								otherSidePiece = -1;
							}
						}
						// Check the down-left
						{
							int k = 1;
							while (true) {
								if (i + k >= SIZE || j - k < 0) {
									otherSidePiece = -1;
									break;
								}
								else if (_CurrentPieceState[(i + k) * SIZE + j - k] == PieceState::BlankPiece)
									break;
								else if (_CurrentPieceState[(i + k) * SIZE + j - k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
									otherSidePiece = k + 1;
								else if (_CurrentPieceState[(i + k) * SIZE + j - k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
									otherSidePiece = -1;
									break;
								}
								k++;
							}
							if (otherSidePiece != -1) {
								_CurrentPieceState[(i + otherSidePiece) * SIZE + j - otherSidePiece] = PieceState::PieceAvailable;
								otherSidePiece = -1;
							}
						}
						// Check the down-right
						{
							int k = 1;
							while (true) {
								if (i + k >= SIZE || j + k >= SIZE) {
									otherSidePiece = -1;
									break;
								}
								else if (_CurrentPieceState[(i + k) * SIZE + j + k] == PieceState::BlankPiece)
									break;
								else if (_CurrentPieceState[(i + k) * SIZE + j + k] == (_CurrentPlayer == Player::Dark ? PieceState::DarkPiece : PieceState::LightPiece))
									otherSidePiece = k + 1;
								else if (_CurrentPieceState[(i + k) * SIZE + j + k] == (_CurrentPlayer == Player::Dark ? PieceState::LightPiece : PieceState::DarkPiece)) {
									otherSidePiece = -1;
									break;
								}
								k++;
							}
							if (otherSidePiece != -1) {
								_CurrentPieceState[(i + otherSidePiece) * SIZE + j + otherSidePiece] = PieceState::PieceAvailable;
								otherSidePiece = -1;
							}
						}
					}
				}
			}
		}

		void Reversi::_CountPieces()
		{
			_LightCount = 0;
			_DarkCount = 0;
			_AvailableCount = 0;
			_BlankCount = 0;

			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					if (_CurrentPieceState[i * SIZE + j] == PieceState::LightPiece)
						_LightCount++;
					else if (_CurrentPieceState[i * SIZE + j] == PieceState::DarkPiece)
						_DarkCount++;
					else if (_CurrentPieceState[i * SIZE + j] == PieceState::PieceAvailable)
					{
						_AvailableCount++;
						_BlankCount++;
					}
					else
						_BlankCount++;
				}
			}
		}

		void Reversi::_TogglePlayer()
		{
			// Turn is over.
			if (_CurrentPlayer == Player::Dark)
				_CurrentPlayer = Player::Light;
			else
				_CurrentPlayer = Player::Dark;
		}

	}
}

using namespace Freiyer::Sandbox;

REVERSI_API_C Reversi * NewReversiDefault()
{
	return new Reversi;
}

REVERSI_API_C Reversi * NewReversiExplicit(const unsigned char * src)
{
	return new Reversi(src);
}

REVERSI_API_C Reversi * NewReversiCopy(Reversi * other)
{
	if (other)
		return new Reversi(*other);
	else
		return nullptr;
}

REVERSI_API_C void DeleteReversi(Reversi * instance)
{
	delete instance;
}

REVERSI_API_C Reversi::State GetState(Reversi * instance)
{
	return instance->GetState();
}

REVERSI_API_C void PrintState(Reversi * instance, char dst[253], int resultLen)
{
	if (resultLen < 253)
		return;
	std::wostringstream wo;
	instance->PrintState(wo);
	auto string = wo.str();
	for (size_t i = 0; i < 253; i++)
	{
		dst[i] = (char)string[i];
	}
	dst[252] = 0;
}

REVERSI_API_C void CopyState(Reversi * instance, Reversi::PieceState dst[8][8])
{
	return instance->CopyState(dst);
}

REVERSI_API_C Reversi::Player GetCurrentPlayer(Reversi * instance)
{
	return instance->GetCurrentPlayer();
}

REVERSI_API_C int GetCurrentTurn(Reversi * instance)
{
	return instance->GetCurrentTurn();
}

REVERSI_API_C int GetBlankCount(Reversi * instance)
{
	return instance->GetBlankCount();
}

REVERSI_API_C int GetLightCount(Reversi * instance)
{
	return instance->GetLightCount();
}

REVERSI_API_C int GetAvailableCount(Reversi * instance)
{
	return instance->GetAvailableCount();
}

REVERSI_API_C int GetDarkCount(Reversi * instance)
{
	return instance->GetDarkCount();
}

REVERSI_API_C Reversi::PieceState GetPieceAt(Reversi * instance, int pi, int pj)
{
	return instance->GetPieceAt(pi, pj);
}

REVERSI_API_C bool SaveState(Reversi * instance, unsigned char * dst)
{
	return instance->SaveState(dst);
}

REVERSI_API_C bool LoadState(Reversi * instance, const unsigned char * src)
{
	return instance->LoadState(src);
}

REVERSI_API_C bool Undo(Reversi * instance)
{
	return instance->Undo();
}

REVERSI_API_C bool IsUndoPossible(Reversi * instance)
{
	return instance->IsUndoPossible();
}

REVERSI_API_C bool PlacePiece(Reversi * instance, int pi, int pj)
{
	return instance->PlacePiece(pi, pj);
}

REVERSI_API_C bool Redo(Reversi * instance)
{
	return instance->Redo();
}

REVERSI_API_C bool IsRedoPossible(Reversi * instance)
{
	return instance->IsRedoPossible();
}

REVERSI_API_C bool SkipTurn(Reversi * instance)
{
	return instance->SkipTurn();
}
