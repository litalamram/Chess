CC = gcc
OBJS = Settings.o SPChessGameSettings.o  SPMinimaxNode.o SPMinimax.o SPChessGame.o SPArrayList.o Parser.o ConsoleMode.o main.o \
UI_Button.o UI_Label.o UI_Panel.o UI_vScrollbar.o UI_Widget.o UI_Window.o \
GUI_ColorWindow.o GUI_DifficultyWindow.o GUI_GameModeWindow.o GUI_GameWindow.o GUI_LoadSaveWindow.o GUI_MainMenuWindow.o GUI_Manager.o GUI_MessageBox.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main


all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.c ConsoleMode.h GUI_Manager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) -c $*.c
SPChessGame.o: SPChessGame.c SPChessGame.h SPArrayList.h SPChessGameSettings.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMinimaxNode.o: SPMinimaxNode.c SPMinimaxNode.h SPChessGame.h
	$(CC) $(COMP_FLAG) -c $*.c 
SPMinimax.o: SPMinimax.c SPMinimax.h SPMinimaxNode.h SPChessGame.h
	$(CC) $(COMP_FLAG) -c $*.c
ConsoleMode.o: ConsoleMode.c ConsoleMode.h SPChessGame.h SPMinimax.h  SPArrayList.h Settings.h SPChessGameSettings.h
	$(CC) $(COMP_FLAG) -c $*.c
Settings.o: Settings.c Settings.h Parser.h SPChessGame.h ConsoleMode.h
	$(CC) $(COMP_FLAG) -c $*.c
SPChessGameSettings.o: SPChessGameSettings.c SPChessGameSettings.h SPChessGame.h
	$(CC) $(COMP_FLAG) -c $*.c
UI_Button.o: UI_Button.c UI_Button.h UI_Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Label.o: UI_Label.c UI_Label.h UI_Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Panel.o: UI_Panel.c UI_Panel.h UI_Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_vScrollbar.o: UI_vScrollbar.c UI_vScrollbar.h UI_Widget.h UI_Panel.h UI_Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Widget.o: UI_Widget.c UI_Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Window.o: UI_Window.c UI_Window.h UI_Widget.h UI_Panel.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_ColorWindow.o: GUI_ColorWindow.c GUI_ColorWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_DifficultyWindow.o: GUI_DifficultyWindow.c GUI_DifficultyWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h  UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_GameModeWindow.o: GUI_GameModeWindow.c GUI_DifficultyWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_GameWindow.o: GUI_GameWindow.c GUI_GameWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_LoadSaveWindow.o: GUI_LoadSaveWindow.c GUI_LoadSaveWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_MainMenuWindow.o: GUI_MainMenuWindow.c GUI_MainMenuWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_Manager.o: GUI_MainMenuWindow.c GUI_MainMenuWindow.h GUI_LoadSaveWindow.h GUI_GameWindow.h GUI_GameModeWindow.h GUI_DifficultyWindow.h GUI_ColorWindow.h SPChessGame.h  Settings.h UI_Window.h UI_Button.h UI_Label.h UI_Panel.h UI_vScrollbar.h UI_Widget.o GUI_Manager.h SPChessGame.h Parser.h Settings.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI_MessageBox.o: GUI_MessageBox.c GUI_MessageBox.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c









clean:
	rm -f *.o $(EXEC)
