#include "GUI_GameWindow.h"
#include "GUI_Manager.h"
#include "SPChessGame.h"
#include "Parser.h"
#include "ConsoleMode.h"
#include "GUI_MessageBox.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Panel.h"
#include "UI_Label.h"


#define NO_PIECE_SELECTED 0
#define PIECE_SELECTED 1
#define PIECE_MOVING 2
#define PIECE_DROPPED 3

extern SPChessGame *game;


int setComputerMove(SPChessGame *game);
void checkGameStatus(SPChessGame *game);
int fillSquaresArray(SPChessGame *game, int row, int col, HighlightSquare *highlightedSqueres);
void mainMenuClick();
void saveClick();
void undoClick();
void resetClick();
int handleMove(SPMove move);
void handleRightClick(Widget *src, SDL_Event *e);
int handleDragPiece (SPChessGame *game, SDL_Event *event, gameWindowData *windowData);
int initGameWindowData(gameWindowData *windowData, SDL_Renderer *rend);
SDL_Texture *getPieceTexture(char piece, gameWindowData *windowData);
int drawPieces(SPChessGame *src, SDL_Renderer* renderer, gameWindowData *windowData);
int drawHighlightedSquares(HighlightSquare *highlightedSqueres, int len, SDL_Renderer* render);
void destroyGameWindData (void *data);


/**Creates a new window that enables the user to play the chess game
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window otherwise
 *
 * */
Widget *createGameWindow(){

	Widget *gameWindow = createWindow("Game Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			GAME_WINDOW_WIDTH,
			GAME_WINDOW_HEIGHT,
			destroyGameWindData);
	if (gameWindow == NULL){
		return NULL;
	}

	Window *window = gameWindow->data;

	gameWindowData *windowData = (gameWindowData*)malloc(sizeof(gameWindowData));
	if (windowData == NULL){
		destroyWidget(gameWindow);
		return NULL;
	}
	if (initGameWindowData(windowData, window->rend) == -1){
		destroyWidget(gameWindow);
		return NULL;
	}
	window->data = windowData;

	Widget *gamePanel = createPanel(gameWindow, 0, 0, GAME_WINDOW_HEIGHT, GAME_WINDOW_HEIGHT);
	if (gamePanel == NULL){
		destroyWidget(gameWindow);
		return NULL;
	}
	addFirstToWindow(gameWindow, gamePanel);

	Widget *board = createLabel(gameWindow, window->rend, boardImg, (SDL_Rect){0,0,600,600});
	if (board == NULL){
		destroyWidget(gameWindow);
		return NULL;
	}
	addFirstToPanel(gamePanel, board);

	Widget *buttonsPanel = createPanel(gameWindow, 600, 0, 200, 600);
	if (buttonsPanel==NULL){
		destroyWidget(gameWindow);
		return NULL;
	}
	addFirstToWindow(gameWindow, buttonsPanel);


	Widget *buttons[6];
	char *images[6] = {mainMenuImg, saveImg, loadGameImg, undoImg, resetImg, quitImg};
	char *disabledImages[6] = {NULL, saveDisabledImg, NULL, undoDisabledImg, NULL, NULL};
	SDL_Rect locations[6] = {{25,10,150,50}, {25,70,150,50}, {25,130,150,50}, {25,190,150,50}, {25,250,150,50}, {25,540,150,50}};
	void (*actions[6])(Widget*, SDL_Event*) = {mainMenuClick, saveClick, goToLoadWindow, undoClick, resetClick, quitClick};
	for (int i=0;i<6;i++){
		buttons[i] = createButton(gameWindow, window->rend, images[i], disabledImages[i], NULL, locations[i], actions[i], 0);
		if (buttons[i]==NULL){
			destroyWidget(gameWindow);
			return NULL;
		}
		if (i==1) windowData->btnSave=buttons[i];
		if (i==3) windowData->btnUndo=buttons[i];
		addFirstToPanel(buttonsPanel, buttons[i]);
	}
	return gameWindow;
}

/** Inits the game window data
 * and creates the pieces textures
 *
 * @return
 * 0 on success,
 * -1, on error */
int initGameWindowData(gameWindowData *windowData, SDL_Renderer *rend){
	windowData->highlightedSquaresLen = 0;
	windowData->isMoving = false;
	windowData->toDrag = NULL;
	windowData->movement = (SDL_Rect){.x=0, .y=0, .w=SQUARE_SIZE, .h=SQUARE_SIZE};
	windowData->xDiff = 0;
	windowData->yDiff = 0;

	//create bleck pieces textures
	windowData->black_pawnTexture = getTexture(rend, bPawnImg);
	windowData->black_knightTexture = getTexture(rend, bKnightImg);
	windowData->black_rookTexture = getTexture(rend, bRookImg);
	windowData->black_bishopTexture = getTexture(rend, bBishopImg);
	windowData->black_queenTexture = getTexture(rend, bQueenImg);
	windowData->black_kingTexture = getTexture(rend, bKingImg);

	//create white pieces textures
	windowData->white_pawnTexture = getTexture(rend, wPawnImg);
	windowData->white_knightTexture = getTexture(rend, wKnightImg);
	windowData->white_rookTexture = getTexture(rend, wRookImg);
	windowData->white_bishopTexture = getTexture(rend, wBishopImg);
	windowData->white_queenTexture = getTexture(rend, wQueenImg);
	windowData->white_kingTexture = getTexture(rend, wKingImg);

	if (windowData->black_pawnTexture == NULL || windowData->black_knightTexture == NULL || windowData->black_rookTexture == NULL ||
			windowData->black_bishopTexture == NULL || windowData->black_queenTexture == NULL || windowData->black_kingTexture == NULL ||
			windowData->white_pawnTexture == NULL || windowData->white_knightTexture == NULL || windowData->white_rookTexture == NULL ||
			windowData->white_bishopTexture == NULL || windowData->white_queenTexture == NULL || windowData->white_kingTexture == NULL){
		return -1;
	}
	return 0;

}

/** Check if there is a check, checkmate or draw,
 * and represents a message box accordingly.
 * */
void checkGameStatus(SPChessGame *game){
	if (game == NULL){
		return;
	}
	game->isOver = false;
	char *title = NULL, *message = NULL;
	int player = spChessGameGetCurrentPlayer(game);
	int c = spChessGameGetState(game);
	if (c == SP_CHESS_GAME_WHITE || c == SP_CHESS_GAME_BLACK || c == SP_CHESS_GAME_DRAW){
		game->isOver = true;
		title = (c == SP_CHESS_GAME_DRAW) ? "Draw" : "Checkmate";
		message = "The game ended with a draw";
		if (c == SP_CHESS_GAME_BLACK){
			message = "Checkmate! Black player wins";
		}
		else if (c == SP_CHESS_GAME_WHITE){
			message = "Checkmate! White player wins";
		}
	}
	else if (c == SP_CHESS_GAME_CHECK){
		title = "Check";
		if (player == SP_CHESS_GAME_BLACK){
			message = "Check: black king is threatened\n";
		}
		else {
			message = "Check: white king is threatened\n";
		}

	}
	if (title != NULL && message != NULL)
		showMessageBox(title, message);
	return;
}

/** Called when the main menu button was clicked.
 * If the game is not saved, asks the user if he wants to save it,
 * if so, go to save window,
 * otherwise, go to main menu window*/
void mainMenuClick(){
	if (!game->isOver && !game->isSaved){ //ask the user if he wants to save
		int choice = createMessageBoxForQuit("Game not saved", "Do you want to save the game?");
		if (choice == 1){ //yes
			goToSaveWindow(false, true);
		}
		else if (choice == 2 || choice == -1){ //no or error displaying message box
			goToMainMenu();
		}
	}
	else {
		goToMainMenu();
	}
}

/** Called when the quit button was clicked.
 * If the game wasn't saved,
 * asks the user if he wants to save it and acting according to his choice.
 * Otherwise, quits the game
 * */
void quitClick(){
	if (!game->isOver && !game->isSaved){
		int choice = createMessageBoxForQuit("Game not saved", "Do you want to save the game?");
		if (choice == 1){ //yes
			goToSaveWindow(true, false);
		}
		else if (choice == 2 || choice == -1){ //no or error displaying message box
			quitGame();
		}
	}
	else {
		quitGame();
	}
}

/** Called when the save button was clicked.
 * goes to the save window
 * */
void saveClick(){
	goToSaveWindow(false, false);
}

/** Called when the undo button was clicked.
 *  If has 2 valid moves in the history, undoes the 2 recent moves,
 *  If there is only one move to undo,
 *  then the program will undo the previous move only.
 * */
void undoClick(){
	CCommand command;
	command.cmd = UNDO;
	handlePlayerCommand(game, command, false);

	game->isSaved = 0;
	checkGameStatus(game); //check if the current player is in check

	// if the computer has to play next, make its turn
	setComputerMove(game);
}

/**Called when the reset button was clicked
 * Restarts the game with the current game settings.
 *
 * If it's the computer turn, plays it's turn*/
void resetClick(){
	CCommand command;
	command.cmd = RESET;
	handlePlayerCommand(game, command, false);

	// if the computer has to play first, make its turn
	setComputerMove(game);
}

/** Checks if it's the computer turn and if so plays it's turn.
 * If it's failed due to malloc error, present an error message to the user.
 * Otherwise, checks if the move cause a check, checkmate or draw,
 * and present a relevant message to the user accordingly
 *
 * @return
 * -1, if an error occured
 *  0, otherwise
 *
 * @param game - the game src*/
int setComputerMove(SPChessGame *game){
	if (!game->isOver && game->game_mode == 1 && game->currentPlayer != game->userColor){ //is it computer turn

		if (computerTurn(game, false) == SP_GAME_STATUS_MALLOC_ERR){ //error occurred
			showMessageBox("ERROR", "Failed allocate memory for computer move");
			printf("ERROR: failed allocate memory for computer move\n");
			return -1;
		}

		else { //move set successfully
			game->isSaved = 0;
			//check if the move cause check,checkmate or draw
			checkGameStatus(game);
		}

	}
	return 0;
}

/** Called when the user drop a piece.
 * If the requested move is valid, makes the move,
 * and checks if it caused check, checkmate or draw.
 *
 * If the current game mode is one player,
 * plays the computer' turn afterwards
 *
 * @return
 *  -1, on error
 *  0, otherwise
 * */
int handleMove(SPMove move){
	if (spChessGameisValidMove(game, move) == SP_CHESS_GAME_LEGAL_MOVE){
		spChessGameSetMove(game, move);
		game->isSaved = 0;
		checkGameStatus(game);
		//if it's computer turn make it's turn
		return setComputerMove(game);
	}
	return 0;
}

/** If the user right clicked the board,
 *  highlights the available moves of the position was clicked
 *
 * @param src - the game window src
 * @param e - the event*/
void handleRightClick(Widget *src, SDL_Event *e){
	if (!src || !e){
		return;
	}
	if (e->type != SDL_MOUSEBUTTONUP) {
		return;
	}
	Window *window = (Window*)src->data;
	gameWindowData *windowData = window->data;

	//get the position was clicked
	bool inRect = isInRect(e->button.x, e->button.y, (SDL_Rect){0,0,600,600});

	int row = e->button.y/(SQUARE_SIZE), col = e->button.x/(SQUARE_SIZE);

	if (inRect && e->button.button == SDL_BUTTON_RIGHT){ //right click to get available moves
		windowData->highlightedSquaresLen = fillSquaresArray(game, row, col, windowData->highlightedSqueres);
	}
	else {
		windowData->highlightedSquaresLen = 0; //remove highlighted squares
	}

}

/** Gets the available moves of the piece at location (row, col)
 * and fills the specified array with squares accordingly:
 * If a move captures a piece - it's color set to green
 * If it's threatened - it's color set to red
 * If it's satisfies both - orange
 * Otherwise, it's color set to blue
 *
 * @param game - the game source
 * @param row,col - the location of the piece
 * @param highlightedSqueres - the array to fill
 *
 * @return
 * the size of the filled array
 * */
int fillSquaresArray(SPChessGame *game, int row, int col, HighlightSquare *highlightedSqueres){
	SPMove moves[SP_CHESS_GAME_MAX_NUMBER_OF_MOVES];
	SPCoordinate coord = {row, col};
	int size;
	SP_CHESS_GAME_MESSAGE status = spChessGameGetMoves(game, coord, moves, &size, true);
	if (status != SP_CHESS_GAME_SUCCESS){
		return 0;
	}
	for (int i=0; i<size; i++){
		SPMove move = moves[i];
		int r,g,b;
		if (move.isThreatened && move.doesCapturePiece){
			r = 255; g=128; b=0; //orange
		}
		else if (move.isThreatened){
			r=255;g=0;b=0; //red
		}
		else if (move.doesCapturePiece){
			r=0;g=255;b=0; //green
		}
		else {
			r=0;g=255;b=255; //light blue
		}

		SDL_Rect rect = { .y = (move.dest.row)*(SQUARE_SIZE), .x = (move.dest.col)*(SQUARE_SIZE), .w = SQUARE_SIZE, .h = SQUARE_SIZE };
		HighlightSquare s = {.rect = rect, .r=r,.g=g,.b=b,.a=128};
		highlightedSqueres[i] = s;
	}
	return size;
}

/** Handles dragging piece event.
 * If the user dropped the piece, makes the move he requested if it's valid
 *
 * @param game - the game src
 * @param event - the event
 * @param windowData - the game window data
 *
 * @return
 * -1, if game == NULL || event == NULL
 * NO_PIECE_SELECTED, if no piece was selected
 * PIECE_SELECTED, if piece was selected
 * PIECE_MOVING, if piece is moving
 * PIECE_FROPPED, if piece droped*/
int handleDragPiece (SPChessGame *game, SDL_Event *event, gameWindowData *windowData){
	if (!game || !event){
		return -1;
	}
	//get the position was clicked
	//get the position was clicked
	bool inRect = isInRect(event->button.x, event->button.y, (SDL_Rect){0,0,600,600});
	if (!inRect) {
		return NO_PIECE_SELECTED;
	}


	int row = event->button.y/(SQUARE_SIZE), col = event->button.x/(SQUARE_SIZE);

	SDL_Rect pos = {.x=col*SQUARE_SIZE, .y=row*SQUARE_SIZE, .w=SQUARE_SIZE, .h=SQUARE_SIZE};

	char dragedPiece;
	int state = NO_PIECE_SELECTED;

	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN: //piece selected
		dragedPiece = spChessGameGetPieceAt(game, row, col);
		//disable dragging of empty piece or enemy piece
		if (dragedPiece == SP_CHESS_GAME_EMPTY_ENTRY ||
				spChessGameGetPieceColor(dragedPiece)!=spChessGameGetCurrentPlayer(game) ||
				(game->game_mode == 1 && spChessGameGetPieceColor(dragedPiece)!=game->userColor)){
			state = NO_PIECE_SELECTED;
		}
		else {
			windowData->toDrag = getPieceTexture(dragedPiece, windowData);
			windowData->xDiff = event->button.x - pos.x;
			windowData->yDiff = event->button.y - pos.y;
			windowData->movement.x = event->motion.x - windowData->xDiff;
			windowData->movement.y = event->motion.y - windowData->yDiff;
			windowData->isMoving = true;
			windowData->wantedMove.start.row = row;
			windowData->wantedMove.start.col = col;
			state = PIECE_SELECTED;
		}
		break;
	case SDL_MOUSEBUTTONUP: //piece is dropped
		if (windowData->isMoving){
			windowData->wantedMove.dest.col = (event->motion.x - windowData->xDiff + SQUARE_SIZE/2)/SQUARE_SIZE;
			windowData->wantedMove.dest.row = (event->motion.y - windowData->yDiff + SQUARE_SIZE/2)/SQUARE_SIZE;
			windowData->isMoving = false;
			windowData->xDiff = 0;
			windowData->yDiff = 0;
			windowData->toDrag = NULL;
			state = PIECE_DROPPED;
		}
		break;
	case SDL_MOUSEMOTION: //piece is moving
		if (windowData->isMoving) {
			windowData->movement.x = event->motion.x - windowData->xDiff;
			windowData->movement.y = event->motion.y - windowData->yDiff;
			state = PIECE_MOVING;
		}
		break;
	}
	return state;

}

/** Returns the texture of the specified piece
 * @param piece - the piece
 * @param windowData - the game window data
 *
 * @return the texture of the piece*/
SDL_Texture *getPieceTexture(char piece, gameWindowData *windowData){
	SDL_Texture *tex;
	SPPieceType type = spChessGameGetPieceType(piece);
	int color = spChessGameGetPieceColor(piece);
	switch (type){
	case KING:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_kingTexture : windowData->white_kingTexture;
		break;
	case QUEEN:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_queenTexture : windowData->white_queenTexture;
		break;
	case PAWN:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_pawnTexture : windowData->white_pawnTexture;
		break;
	case ROOK:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_rookTexture : windowData->white_rookTexture;
		break;
	case KNIGHT:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_knightTexture : windowData->white_knightTexture;
		break;
	case BISHOP:
		tex = (color == SP_CHESS_GAME_BLACK) ? windowData->black_bishopTexture : windowData->white_bishopTexture;
		break;
	case BLANK:
		tex = NULL;
		break;

	}
	return tex;
}

/** Draws the pieces on the board
 *
 * @return
 * 0, on success
 * -1, on error*/
int drawPieces(SPChessGame *src, SDL_Renderer* renderer, gameWindowData *windowData){
	char piece;
	SDL_Texture *texture;
	SDL_Rect location = {.x = 0, .y = 0, .w = SQUARE_SIZE, .h = SQUARE_SIZE};
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			piece = spChessGameGetPieceAt(src, i, j);
			texture = getPieceTexture(piece, windowData);
			if (texture != NULL){
				location.x = j*SQUARE_SIZE;
				location.y = i*SQUARE_SIZE;
				if (windowData->toDrag == NULL || j != windowData->wantedMove.start.col || i != windowData->wantedMove.start.row){
					// draw only undraged pieces
					if (SDL_RenderCopy(renderer, texture, NULL, &location) == -1){
						return -1;
					}
				}
			}
		}
	}
	if (windowData->toDrag != NULL){ //draw dragged piece
		if (SDL_RenderCopy(renderer, windowData->toDrag, NULL, &windowData->movement) == -1){
			return -1;
		}
	}
	return 0;
}

/** Draws the highlighted squares to the screen
 *
 * @param highlightedSqueres - the squares to be drawn
 * @param len - the length of the array
 * @param render - the renderer
 * */
int drawHighlightedSquares(HighlightSquare *highlightedSqueres, int len, SDL_Renderer* render){
	int status;
	for (int i=0;i<len;i++){
		status = SDL_SetRenderDrawColor(render, highlightedSqueres[i].r, highlightedSqueres[i].g, highlightedSqueres[i].b, highlightedSqueres[i].a);
		if (status == -1){ //error
			return -1;
		}
		status = SDL_RenderFillRect(render, &highlightedSqueres[i].rect);
		if (status == -1){ //error
			return -1;
		}
	}
	return 0;
}

/** Draws the game window to the screen
 *
 * @param src - the game window src
 * @param render - the renderer
 *
 * @return
 * -1, on error
 * 0, on success
 * */
int drawGameWindow(Widget *src, SDL_Renderer* render){
	//draw window
	if (src->draw(src, render) == -1){
		return -1;
	}

	//draw highlighted squares
	Window *window = (Window*)src->data;
	gameWindowData *windowData = (gameWindowData*)window->data;
	if (drawHighlightedSquares(windowData->highlightedSqueres, windowData->highlightedSquaresLen, render) == -1){
		return -1;
	}

	//draw pieces
	if (drawPieces(game, render, windowData) == -1){
		return -1;
	}

	//disable/ enable save and undo buttons
	if(game->isSaved || game->isOver){
		disableButton(windowData->btnSave, true);
	}
	else {
		disableButton(windowData->btnSave, false);
	}
	if (spArrayListSize(game->recentMoves)==0){
		disableButton(windowData->btnUndo, true);
	}
	else {
		disableButton(windowData->btnUndo, false);
	}

	return 0;
}

/** Presents the game window to the screen.
 * If the computer should play first, play it's turn
 *
 * @param src - the game windoe instance src
 *
 * */
void showGameWindow(Widget *src){
	showWindow(src);
	checkGameStatus(game); //check if the current player is in check

	//if the computer has to play first, make its turn
	setComputerMove(game);

}

/** Handles the specified event
 *
 * @param src - the game window src
 * @param event - the event to be handled
 *
 * */
void handleEventGameWindow (Widget *src, SDL_Event *event){
	int status;
	gameWindowData *windowData = ((Window*)src->data)->data;
	status = handleDragPiece(game, event, windowData);
	if (status == PIECE_DROPPED){ //piece dropped
		handleMove(windowData->wantedMove);
	}
	handleRightClick(src, event);
	src->handleEvent(src, event);
}

/**Destroys the gameWindowData
 *
 * @param data - the gameWindowData src
 *
 * */
void destroyGameWindData (void *data){
	if (data == NULL){
		return;
	}
	gameWindowData *windowData = (gameWindowData*)data;
	//destroy pieced textures
	if (windowData->white_pawnTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_pawnTexture);
	}
	if (windowData->white_knightTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_knightTexture);
	}
	if (windowData->white_rookTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_rookTexture);
	}
	if (windowData->white_bishopTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_bishopTexture);
	}
	if (windowData->white_queenTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_queenTexture);
	}
	if (windowData->white_kingTexture != NULL ) {
		SDL_DestroyTexture(windowData->white_kingTexture);
	}
	if (windowData->black_pawnTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_pawnTexture);
	}
	if (windowData->black_knightTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_knightTexture);
	}
	if (windowData->black_rookTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_rookTexture);
	}
	if (windowData->black_bishopTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_bishopTexture);
	}
	if (windowData->black_queenTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_queenTexture);
	}
	if (windowData->black_kingTexture != NULL ) {
		SDL_DestroyTexture(windowData->black_kingTexture);
	}
	free(windowData);

}
