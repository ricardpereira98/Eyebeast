#ifndef _wxTiny_
#define _wxTiny_

/* wxTiny.h version 5.0
   Artur Miguel Dias, 2022
   Dept. de Informatica, FCT, Universidade Nova de Lisboa.

   wxTiny is a small framework that supports simple 2D arcade games.
   It has been created for pedagogical purposes.

   wxTiny works on Windows, Linux and MacOS X and is compatible with C and C++.
   wxTiny runs on top of the (wonderful) wxWidgets library.
*/

typedef char *tyStr;
typedef const char *tyConstStr;
typedef const char *tyImage[];

typedef void (*Fun)();
typedef void (*FunD)(int dx, int dy);
typedef void (*MenuFun)(tyStr command);

#ifdef __cplusplus
extern "C" {
#endif

/* How does the wxTiny framework work?

   The wxTiny framework owns the 'main' function. Therefore, it is wxTiny
   that is in charge of the execution of the program.

   The 'main' function of wxTiny keeps running an event loop that waits
   for events coming from the user and the timer. Then, it dispatches
   those event to the application side using several event handlers
   that are provided by the application,
*/

// REQUIRED FUNCTIONS: Your application must supply a number of
// functions that will be automatically called by wxTiny.

tyStr tyAppName(void);					// title of the game window
int tyWidth(void);						// width of the game window in pixels
int tyHeight(void);						// height of the game window in pixels
int tyStatusItems(void);				// number of slots in the status bar
tyStr tyMenus(void);					// description of the menus
void tyHandleStart(void);				// handler of the start event
void tyHandleMenuCommand(tyStr command); // handler of the menu events
void tyHandleRedraw(void);				// handler of the window redraw events
void tyHandleTime(void);				// handler of the time events

// Reads keypresses.
int tyKeyDeltaX(void);
int tyKeyDeltaY(void);
char tyKey(void);

// Creates an image from a description in XPM format. Returns an
// integer handle that can be used later in the DrawImage function.
int tyCreateImage(tyImage xpm);

// Creates an image from a generic description in XPM format plus the name of
// a color. In the generic description there is a color placeholder represented
// by $1.
int tyCreateImage2(tyImage xpm, tyConstStr color);

// Draws an image at coordinates (x, y).
void tyDrawImage(int image, int x, int y);

// Displays a message in a dialog.
void tyAlertDialog(tyConstStr title, tyConstStr fmt, ...);

// Displays an error message in a dialog but does not quit the application.
void tyError(tyConstStr fmt, ...);

// Displays an error message in a dialog and then quits the application.
void tyFatalError(tyConstStr fmt, ...);

// Asks a question to the user through a dialog.
tyStr tyQueryDialog(tyConstStr title, tyConstStr question);

// Asks the user to select an existing file.
// The dialog window supports navigating the filesystem.
tyStr tyOpenDialog(void);

// Asks the user to select an existing file or create a new file.
// The dialog window supports navigating the filesystem.
tyStr tySaveDialog(void);

// If 'paused' is true, wxTiny stops emitting events.
void tyPaused(bool paused);

// Sets the speed of the game.
#define TY_MAX_SPEED	7
void tySetSpeed(int speed);

// Specifies the text that should appear in the status bar at the bottom of the
// window. The status bar has several parts, referenced by position: 0, 1, 2...
void tySetStatusText(int pos, tyConstStr txt);

// Exits the application
void tyQuit(void);

// Time in seconds, since program start or since the last reset
int tySeconds(void);

// Resets the seconds counter
void tySecondsSetZero(void);

// Cartesian distance between two points, rounded up to an integer.
int tyDistance(int x1, int y1, int x2, int y2);

// Cartesian distance on a circular world with a certain width and height .
int tyDistanceCircular(int x1, int y1, int x2, int y2, int width, int height);

// Generates random integer between 0 and n-1 inclusive.
int tyRand(int n);

#ifdef __cplusplus
}
#endif

#endif
