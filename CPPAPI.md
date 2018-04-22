Writing on this file is in progress.

#### Other version of this file
[ja-jp](CPPAPI.ja-jp.md): 日本語\
[ko-kr](CPPAPI.ko-kr.md): 한국어

# Reversi C++ API
## namespace Freiyer::Sandbox
### class Reversi
#### Nested
| Definition | Description |
| -- | -- |
| ```enum class PieceState : unsigned char``` | For indicating types of pieces. |
| ```enum class State : unsigned char``` | For indicating the state of the game. |
| ```enum class Player : unsigned char``` | For indicating whose turn it is. |
##### PieceState
| Name | Description |
| -- | -- |
##### State
| Name | Description |
| -- | -- |
##### Player
| Name | Description |
| -- | -- |
#### Constructors
| Definition | Description |
| -- | -- |
| ```Reversi();``` | Default constructor. Starts new game. |
| ```Reversi(const unsigned char * src);``` | Loads the state from pre-generated data by ```SaveState``` |
| ```Reversi(const Reversi &);``` | Basic copy constructor. Copies the state from an instance. |
| ```Reversi(Reveri &&);``` | Basic move constructor. |
#### Methods
##### GetState
Returns the state of the game.
| Return Type |
| -- |
| ```State``` |
##### PrintState
Print the current state to the given stream.
| Parameters | Description |
| -- | -- |
| ```std::wostream & o``` | The stream to print the current state. |
##### CopyState
Copies the current piece state to where the given pointer is pointing.
| Parameters | Description |
| ```PieceState * dst``` | The pointer indicating address where the state will be copied. |
##### GetCurrentPlayer
##### GetCurrentTurn
##### GetBlankCount
##### GetLightCount
##### GetDarkCount
##### GetAvailableCount
##### GetPieceAt
##### SaveState
##### LoadState
##### Undo
##### IsUndoPossible
##### Redo
##### IsRedoPossible
##### PlacePiece
##### SkipTurn
