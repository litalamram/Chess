#include "GUI_MessageBox.h"
#include <SDL.h>
#include <stdio.h>

/** Displays a message box with the specified title and message
 *
 * @param - title - the title of the message box
 * @param message - the message of the message box
 *
 * */
void showMessageBox(char *title, char *message){
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL) == -1){
		printf("ERROR: error displaying message box\n");
	}
}

/**Displays a message box with the specifies title and message
 * Enables 3 options: Yes, No, Cancel
 *
 * @param - title - the title of the message box
 * @param message - the message of the message box
 *
 * @return
 * 1, if chose "Yes"
 * 2, if chose "No"
 * 3, if chose "Cancel
 * -1, on error
 * */
int createMessageBoxForQuit(char* title, char* message){
	const SDL_MessageBoxButtonData buttons[] = {
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "No" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 3, "Cancel" },
	};
	const SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION, /* .flags */
			NULL, /* .window */
			title, /* .title */
			message, /* .message */
			SDL_arraysize(buttons), /* .numbuttons */
			buttons, /* .buttons */
			NULL /* .colorScheme */
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		printf("ERROR: error displaying message box\n");
		return -1;
	}
	return buttonid;
}



