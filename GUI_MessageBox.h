#ifndef GUI_MESSAGEBOX_H_
#define GUI_MESSAGEBOX_H_

/** Displays a message box with the specified title and message
 *
 * @param - title - the title of the message box
 * @param message - the message of the message box
 *
 * */
void showMessageBox(char *title, char *message);

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
int createMessageBoxForQuit(char* title, char* message);

#endif /* GUI_MESSAGEBOX_H_ */
