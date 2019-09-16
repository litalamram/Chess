#include <SDL.h>
#include "SPChessGame.h"
#include "UI_Widget.h"

//definitions

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600
#define SQUARE_SIZE 75

//paths to images

#define boardImg "images/gameWindow/board.bmp"
#define mainMenuImg "images/gameWindow/mainMenu.bmp"
#define saveImg "images/gameWindow/save.bmp"
#define saveDisabledImg "images/gameWindow/save_inactive.bmp"
#define undoImg "images/gameWindow/undo.bmp"
#define undoDisabledImg "images/gameWindow/undo_inactive.bmp"
#define resetImg "images/gameWindow/reset.bmp"
#define quitImg "images/general/quitGame.bmp"
#define loadGameImg "images/general/loadGame.bmp"


#define bKingImg "images/gameWindow/pieces/bKing.bmp"
#define bQueenImg "images/gameWindow/pieces/bQueen.bmp"
#define bRookImg "images/gameWindow/pieces/bRook.bmp"
#define bKnightImg "images/gameWindow/pieces/bKnight.bmp"
#define bPawnImg "images/gameWindow/pieces/bPawn.bmp"
#define bBishopImg "images/gameWindow/pieces/bBishop.bmp"

#define wKingImg "images/gameWindow/pieces/wKing.bmp"
#define wQueenImg "images/gameWindow/pieces/wQueen.bmp"
#define wRookImg "images/gameWindow/pieces/wRook.bmp"
#define wKnightImg "images/gameWindow/pieces/wKnight.bmp"
#define wPawnImg "images/gameWindow/pieces/wPawn.bmp"
#define wBishopImg "images/gameWindow/pieces/wBishop.bmp"

/**type used to represent an highlighted square*/
typedef struct highlight_square_t{
	SDL_Rect rect;
	int r,g,b,a;
} HighlightSquare;

/**type used to represent relevant data about the window*/
typedef struct game_window_data_t {
	Widget *btnSave, *btnUndo; //pointers to undo and save buttons

	//squares to highlight in the board
	HighlightSquare highlightedSqueres[SP_CHESS_GAME_MAX_NUMBER_OF_MOVES];
	int highlightedSquaresLen;

	//info about draged piece
	SDL_Texture* toDrag;
	SPMove wantedMove;
	SDL_Rect movement;
	bool isMoving;
	int xDiff, yDiff;

	//pieces textures
	SDL_Texture* black_pawnTexture;
	SDL_Texture* black_knightTexture;
	SDL_Texture* black_rookTexture;
	SDL_Texture* black_bishopTexture;
	SDL_Texture* black_queenTexture;
	SDL_Texture* black_kingTexture;
	SDL_Texture* white_pawnTexture;
	SDL_Texture* white_knightTexture;
	SDL_Texture* white_rookTexture;
	SDL_Texture* white_bishopTexture;
	SDL_Texture* white_queenTexture;
	SDL_Texture* white_kingTexture;

} gameWindowData;

/**Creates a new window that enables the user to play the chess game
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window otherwise
 *
 * */
Widget *createGameWindow();

/** Presents the game window to the screen.
 * If the computer should play first, play it's turn
 *
 * @param src - the game windoe instance src
 *
 * */
void showGameWindow(Widget *src);

/** Draws the game window to the screen
 *
 * @param src - the game window src
 * @param render - the renderer
 *
 * @return
 * -1, on error
 * 0, on success
 * */
int drawGameWindow(Widget *src, SDL_Renderer* render);

/** Handles the specified event
 *
 * @param src - the game window src
 * @param event - the event to be handled
 *
 * */
void handleEventGameWindow (Widget *src, SDL_Event *event);

/** Called when the quit button was clicked.
 * If the game wasn't saved,
 * asks the user if he wants to save it and acting according to his choice.
 * Otherwise, quits the game
 * */
void quitClick();
