/*
 max width = 80 columns
 tab = 4 spaces
 01234567890123456789012345678901234567890123456789012345678901234567890123456789
 */

/*	Linguagens e Ambientes de Programacao - Projeto de 2021/2022

 Eye Beast

 Program written in C/C ++ over the wxWidget platform.
 The wxWidget platform runs on Windows, MacOS and Linux.

 This file is only a starting point fo your work. The entire file can
 be changed, starting with this comment.


 AUTHORS IDENTIFICATION
 Student 1: 55204, Jose Pereira
 Student 2: 58592, Vasco Malta

 Comments:

 We decided to implement the levels extra function, where if the hero manages to trap all the monsters,
 he passes onto the next level with (N_MONSTERS+level) monsters.


 Place here the names and numbers of the authors, plus some comments, as
 Â asked in the listing of the project. Do not deliver an anonymous file with
 unknown authors.
 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* IDENTIFICATION */

#define APP_NAME	"Eye Beast"

#define AUTHOR1		"Jose Pereira (55204)"
#define AUTHOR2		"Vasco Malta (58592)"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* INCLUDES i just wanted to learn how to use git dude*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "wxTiny.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* STRINGS */

#define MAX_STRING	256
#define MAX_LINE	1024

typedef char String[MAX_STRING];
typedef char Line[MAX_LINE];

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* IMAGES */

/******************************************************************************
 * The images are specified in the XPM format [http://en.wikipedia.org/wiki/X_PixMap]
 * After created, each image is represented by an integer in the wxTiny library.
 ******************************************************************************/

typedef int Image;

static Image emptyImg, heroImg, chaserImg, blockImg, boundaryImg, invalidImg;

/* XPM */
static tyImage empty_xpm = { "16 16 2 1", "   c None", ".  c #FFFFFF",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................" };

/* XPM */
static tyImage hero_xpm = { "16 16 3 1", "   c None", ".  c #FFFFFF",
		"+  c #000000", "................", "...+++...+++....",
		"..+...+.+...+...", ".+.....+.....+..", ".+...+++++...+..",
		".+...+++++...+..", ".+...+++++...+..", ".+.....+.....+..",
		"..+...+.+...+...", "...+++...+++....", "................",
		".....+++++......", "....+.....+.....", "................",
		"................", "................" };

/* XPM */
static tyImage chaser_xpm = { "16 16 3 1", "   c None", ".  c #FFFFFF",
		"+  c #000000", "................", "...+++...+++....",
		"..++.++.+.+++...", ".+..+++++++..+..", ".+...+++++...+..",
		".+...+++++...+..", ".+...+++++...+..", "..+...+.+...+...",
		"...+++...+++....", "................", "................",
		"....+++++++.....", "................", "................",
		"................", "................" };

/* XPM */
static tyImage block_xpm = { "16 16 3 1", "   c None", ".  c #000000",
		"+  c #FFFFFF", "................", "................",
		"..............+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
		"....+.+.+.+.+...", "...+.+.+.+.+..+.", "....+.+.+.+.+...",
		"...+.+.+.+.+..+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
		"....+.+.+.+.+...", "...+.+.+.+.+..+.", "................",
		"..+.+.+.+.+.+.+.", "................" };

/* XPM */
static tyImage boundary_xpm = { "16 16 3 1", "   c None", ".  c #000000",
		"+  c #FFFFFF", "................", "................",
		"..............+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
		"....+.+.+.+.+...", "...+.+.+.+.+..+.", "....+.+.+.+.+...",
		"...+.+.+.+.+..+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
		"....+.+.+.+.+...", "...+.+.+.+.+..+.", "................",
		"..+.+.+.+.+.+.+.", "................" };

/* XPM */
static tyImage invalid_xpm = { "16 16 2 1", "   c None", ".  c #FFFFFF",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................", "................", "................",
		"................" };

/******************************************************************************
 * imagesCreate - Converts all the XPM images to the type Image
 ******************************************************************************/
void imagesCreate(void) {
	emptyImg = tyCreateImage(empty_xpm);
	heroImg = tyCreateImage(hero_xpm);
	chaserImg = tyCreateImage(chaser_xpm);
	blockImg = tyCreateImage(block_xpm);
	boundaryImg = tyCreateImage(boundary_xpm);
	invalidImg = tyCreateImage(invalid_xpm);
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/* ACTORS */
/******************************************************************************
 * Constants, types and functions for all kinds of actors
 ******************************************************************************/
#define ACTOR_PIXELS_X	16
#define ACTOR_PIXELS_Y	16

//tracks current level, each level puts one more monster chasing the player
int level = 0;

typedef enum {
	EMPTY, HERO, CHASER, BLOCK, BOUNDARY
} ActorKind;
typedef struct {
// specific fields can go here, but probably none will be needed
} Hero;
typedef struct {
	int counter;
// specific fields can go here, but probably none will be needed
} Chaser;
typedef struct {
// specific fields can go here, but probably none will be needed
} Block;
typedef struct {
// specific fields can go here, but probably none will be needed
} Boundary;
typedef struct {
// factored common fields
	ActorKind kind;
	int x, y, n;
	Image image;
	union {
// specific fields for each kind
		Hero hero;
		Chaser chaser;
		Block block;
		Boundary boundary;
	} u;
} ActorStruct, *Actor;
#define WORLD_SIZE_X	31
#define WORLD_SIZE_Y	18
#define N_MONSTERS		1
typedef struct {
	Actor world[WORLD_SIZE_X][WORLD_SIZE_Y];
	Actor hero;
	Actor monsters[N_MONSTERS];
} GameStruct, *Game;

/******************************************************************************
 * actorImage - Get the screen image corresponding to some kind of actor
 ******************************************************************************/
Image actorImage(ActorKind kind) {
	switch (kind) {
	case EMPTY:
		return emptyImg;
	case HERO:
		return heroImg;
	case CHASER:
		return chaserImg;
	case BLOCK:
		return blockImg;
	case BOUNDARY:
		return boundaryImg;
	default:
		return invalidImg;
	}
}

/******************************************************************************
 * cellSet - Useful function to update one cell in the matrix and in the screen
 ******************************************************************************/
void cellSet(Game g, Actor a, int x, int y, Image img) {
	tyDrawImage(img, x * ACTOR_PIXELS_X, y * ACTOR_PIXELS_Y);
	g->world[x][y] = a;
}

/******************************************************************************
 * cellIsEmpty - Check in the matrix if a cell is empty (if contains NULL)
 ******************************************************************************/
bool cellIsEmpty(Game g, int x, int y) {
	return g->world[x][y] == NULL;
}

/******************************************************************************
 * actorShow - Install an actor in the matrix and in the screen
 ******************************************************************************/
void actorShow(Game g, Actor a) {
	cellSet(g, a, a->x, a->y, a->image);
}

/******************************************************************************
 * actorHide - Remove an actor from the matrix and from the screen
 ******************************************************************************/
void actorHide(Game g, Actor a) {
	cellSet(g, NULL, a->x, a->y, emptyImg);
}

/******************************************************************************
 * actorMove - Move an actor to a new position
 * pre: the new position is empty
 ******************************************************************************/
void actorMove(Game g, Actor a, int nx, int ny) {
	actorHide(g, a);
	a->x = nx;
	a->y = ny;
	actorShow(g, a);
}

/******************************************************************************
 * actorNew - Creates a new actor and installs it in the matrix and the screen
 ******************************************************************************/
Actor actorNew(Game g, ActorKind kind, int x, int y) {
	Actor a = malloc(sizeof(ActorStruct));
	a->kind = kind;
	a->x = x;
	a->y = y;
	a->image = actorImage(kind);
	actorShow(g, a);
	return a;
}

//returns the actor's kind
ActorKind getKind(Game g, int x, int y) {
	Actor a = g->world[x][y];
	return a->kind;
}

/**
 * @pre: actor != null and so far a neighbour can only be a block, later we could add something else as a neighbour
 */
bool hasNeighbour(Game g, int x, int y) {
	Actor neighbour = g->world[x][y];
	return !cellIsEmpty(g, x, y) && neighbour->kind == BLOCK;
}
/**
 * @PRE: is a block
 * g- game
 * a- the current actor
 * x- block's x position
 * y- block's y position
 * dx - direction of movement x
 * dy - direction of movement y
 *
 */
bool push(Game g, Actor a, int x, int y, int dx, int dy) {

	Actor pushedBlock = a;
	bool isPushable = false;
	int nx = x + dx; //neighbour x position
	int ny = y + dy; //neighbour y position

	if (cellIsEmpty(g, nx, ny)) {
		actorMove(g, pushedBlock, nx, ny);
		isPushable = true;
	} else {

		if (hasNeighbour(g, nx, ny)) { //a neighbour can only be a block so far, new exceptions could later be added on this function

			isPushable = true;
			Actor neighbour = g->world[nx][ny];

			if (push(g, neighbour, nx, ny, dx, dy)) {
				actorMove(g, pushedBlock, nx, ny);
				actorMove(g, neighbour, nx + dx, ny + dy);
			} else
				isPushable = false;

		}
	}
	return isPushable;
}

/******************************************************************************
 * heroAnimation - The hero moves using the cursor keys
 * done! later we could add more cases that the hero could react upon
 ******************************************************************************/
void heroAnimation(Game g, Actor a) {

	int dx = tyKeyDeltaX(), dy = tyKeyDeltaY();
	int nx = a->x + dx, ny = a->y + dy;

	if (cellIsEmpty(g, nx, ny))
		actorMove(g, a, nx, ny);

	else {

		ActorKind kind = getKind(g, nx, ny);
		Actor nextActor = g->world[nx][ny];

		switch (kind) {

		case BLOCK:
			if (push(g, nextActor, nx, ny, dx, dy)) //nx ny are the block's current positions actually
				actorMove(g, a, nx, ny);
			break;
		case CHASER:
			actorShow(g, nextActor);
			endGameDefeat();
			break;

		default:
			break;
		}
	}
}

//checks if something is worth eating
bool isTasty(Game g, Actor a) {
	switch (a->kind) {
	case HERO:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//checks if the monster can move to a cell
bool monsterCanMoveTo(Game g, int x, int y) {
	Actor a = g->world[x][y];
	return cellIsEmpty(g, x, y) || isTasty(g, a);
}
//checks if the monsters are trapped, meaning the game is over
bool areMonstersTrapped(Game g) {

	int n = 0;
	while (n < N_MONSTERS + level) {

		Actor chaser = g->monsters[n];

		int monsterX = chaser->x;
		int monsterY = chaser->y;

		//if monster is surrounded by something
		if (!cellIsEmpty(g, monsterX - 1, monsterY + 1)
				&& !cellIsEmpty(g, monsterX, monsterY + 1)
				&& !cellIsEmpty(g, monsterX + 1, monsterY + 1)
				&& !cellIsEmpty(g, monsterX + 1, monsterY)
				&& !cellIsEmpty(g, monsterX + 1, monsterY - 1)
				&& !cellIsEmpty(g, monsterX, monsterY - 1)
				&& !cellIsEmpty(g, monsterX - 1, monsterY - 1)
				&& !cellIsEmpty(g, monsterX - 1, monsterY))
			n++;
		else
			return false;
	}
	return true;
}

//returns true if the hero and chaser are in the same cell
bool foundHero(int cx, int cy, int hx, int hy) {
	return cx == hx && cy == hy;
}

//is the cell the monster will walk to the hero?
bool willHeroBeThere(Game g, int x, int y) {
	int hx = g->hero->x;
	int hy = g->hero->y;
	return hx == x && hy == y;
}

//ends the game with the player winning, and starts it over again on the next level
void endGameVictory() {
	level++;
	tyAlertDialog("POGGERS!!!", "LEVEL %d COMPLETED", level);
	tyHandleStart();
}
//ends the game with the player losing, and starts it over again
void endGameDefeat() {
	level = 0;
	tyAlertDialog("GAME OVER", "NOOB! :)");
	tyHandleStart();
}

//Operates chaser movement and win/lose conditions
void chaserAnimation(Game g, Actor a) {

	//chaser location
	int cx = a->x;
	int cy = a->y;

	//hero location
	int hx = g->hero->x;
	int hy = g->hero->y;

	Actor chaser = g->world[cx][cy];
	Actor hero = g->world[hx][hy];

	//initializing direction of movement variables as 0
	int dx = 0;
	int dy = 0;

	//is the game cleared
	if (areMonstersTrapped(g)) {
		endGameVictory();

	} else if (foundHero(cx, cy, hx, hy)) {
		actorShow(g, chaser);
		endGameDefeat();
	}
	//chaser only makes a move whenever counter equals 10
	if (a->u.chaser.counter == 10) {

		//Diagonais
		if (cx < hx && cy < hy && monsterCanMoveTo(g, cx + 1, cy + 1)) {
			//direction of movement on x and y
			dx = 1;
			dy = 1;
		} else if (cx < hx && cy > hy && monsterCanMoveTo(g, cx + 1, cy - 1)) {
			dx = 1;
			dy = -1;
		} else if (cx > hx && cy < hy && monsterCanMoveTo(g, cx - 1, cy + 1)) {
			dx = -1;
			dy = 1;
		} else if (cx > hx && cy > hy && monsterCanMoveTo(g, cx - 1, cy - 1)) {
			dx = -1;
			dy = -1;
		}
		//Horizontais
		else if (cx < hx && monsterCanMoveTo(g, cx + 1, cy)) {
			dx = 1;
			dy = 0;
		} else if (cx > hx && monsterCanMoveTo(g, cx - 1, cy)) {
			dx = -1;
			dy = 0;
		}
		//Verticais
		else if (cy < hy && monsterCanMoveTo(g, cx, cy + 1)) {
			dx = 0;
			dy = 1;
		} else if (cy > hy && monsterCanMoveTo(g, cx, cy - 1)) {
			dx = 0;
			dy = -1;
		}
		if (willHeroBeThere(g, cx + dx, cy + dy)) {
			actorMove(g, a, cx + dx, cy + dy);
			endGameDefeat();

		} else {

			actorMove(g, a, cx + dx, cy + dy);
			a->u.chaser.counter = 0;
		}

	} else
		a->u.chaser.counter = a->u.chaser.counter + 1;
}

/******************************************************************************
 * actorAnimation - The actor behaves according to its kind
 * done, new actor animations could be added in the future
 ******************************************************************************/
void actorAnimation(Game g, Actor a) {
	switch (a->kind) {
	case CHASER:
		chaserAnimation(g, a);
		break;
	case HERO:
		heroAnimation(g, a);
		break;
	default:
		break;
	}
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* GAME */

/******************************************************************************
 * gameClearWorld - Clear the matrix and the screen
 ******************************************************************************/
void gameClearWorld(Game g) {
	for (int y = 0; y < WORLD_SIZE_Y; y++)
		for (int x = 0; x < WORLD_SIZE_X; x++)
			cellSet(g, NULL, x, y, emptyImg);
}
/******************************************************************************
 * gameInstallBoundaries - Install the boundary blocks
 ******************************************************************************/
void gameInstallBoundaries(Game g) {
	for (int y = 0; y < WORLD_SIZE_Y; y++)
		for (int x = 0; x < WORLD_SIZE_X; x++)
			if (x == 0 || x == WORLD_SIZE_X - 1 || y == 0
					|| y == WORLD_SIZE_Y - 1) {
				actorNew(g, BOUNDARY, x, y);
			}
}
/******************************************************************************
 * gameInstallBlocks - Install the movable blocks
 * done!
 ******************************************************************************/
void gameInstallBlocks(Game g) {
	int n = 0;
	while (n < 110) {
		int x = tyRand(WORLD_SIZE_X);
		int y = tyRand(WORLD_SIZE_Y);
		if (cellIsEmpty(g, x, y)) {
			actorNew(g, BLOCK, x, y);
			n++;
		}
	}
}

/******************************************************************************
 * gameInstallMonsters - Install the monsters
 * done! assumes there is at least one monster in the game though
 ******************************************************************************/
void gameInstallMonsters(Game g) {

	int heroX = g->hero->x;
	int heroY = g->hero->y;
	int n = 0;

	while (n < (N_MONSTERS + level)) {
		int x = tyRand(WORLD_SIZE_X);
		int y = tyRand(WORLD_SIZE_Y);
		if (cellIsEmpty(g, x, y) && tyDistance(x, y, heroX, heroY) > 4) {
			g->monsters[n] = actorNew(g, CHASER, x, y);
			g->monsters[n]->u.chaser.counter = 0;
			n++;
		}
	}
}

/******************************************************************************
 * gameInstallHero - Install the hero
 * done!
 ******************************************************************************/
void gameInstallHero(Game g) {
	int n = 0;
	while (n < 1) {
		int x = tyRand(WORLD_SIZE_X);
		int y = tyRand(WORLD_SIZE_Y);
		if (cellIsEmpty(g, x, y)) {
			g->hero = actorNew(g, HERO, x, y);
			n++;
		}
	}

}

/******************************************************************************
 * gameInit - Initialize the matrix and the screen
 ******************************************************************************/
Game gameInit(Game g) {
	if (g == NULL)
		g = malloc(sizeof(GameStruct));
	imagesCreate();
	gameClearWorld(g);
	gameInstallBoundaries(g);
	gameInstallBlocks(g);
	gameInstallHero(g);
	gameInstallMonsters(g);
	return g;
}
/******************************************************************************
 * gameRedraw - Redraw the entire scenario. This function is called by
 * tyHandleRedraw in very specific circumstances. It should not be used anywhere
 * else because you don't want to be constantly redrawing the whole scenario.
 ******************************************************************************/
void gameRedraw(Game g) {
	for (int y = 0; y < WORLD_SIZE_Y; y++)
		for (int x = 0; x < WORLD_SIZE_X; x++) {
			Actor a = g->world[x][y];
			if (!cellIsEmpty(g, x, y))
				actorShow(g, a);
		}
}

/******************************************************************************
 * gameAnimation - Send animation events to all the animated actors
 * This function is called every tenth of a second (more or less...)
 * TODO
 ******************************************************************************/
void gameAnimation(Game g) {
	actorAnimation(g, g->hero);
	int i = 0;
	while (i < (N_MONSTERS + level)) {
		actorAnimation(g, g->monsters[i]);
		i++;
	}
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* STATUS BAR */

/******************************************************************************
 * The function status populates the status bar, at the bottom of the window
 ******************************************************************************/

#define STATUS_ITEMS	5

void status(void) {
	String s;
	sprintf(s, "TIME = %d seg.", tySeconds());
	tySetStatusText(4, s);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* MENU COMMANDS */

/******************************************************************************
 * Each function handles one menu command
 ******************************************************************************/

void comandAbout(void) {
	tyAlertDialog("About", "%s\n%s\n\n%s\n%s",
	APP_NAME, "(Reimplementation of the original game of Chuck Shotton)",
	AUTHOR1,
	AUTHOR2);
}

void comandRestart(void) {
	tyHandleStart();
}

void comandFinish(void) {
	tyAlertDialog("Quit", "See you later!");
	tyQuit();
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* FUNCTIONS REQUIRED BY THE FRAMEWORK wxTiny */

/******************************************************************************
 * Section with all the "root" "functions required by the wxTiny support system.
 *
 * There is no function "main" in a wxTiny program, as the main function is
 * hidden within the wxTiny library. The most important thing you need to know
 * is that the function "tyHandleStart" is called when the program begins,
 * and that function "tyHandleTime" is called periodically 10 times per second,
 * to make the program progress. In some sense, this pair of functions makes a kind
 * of main function.
 *
 * More information about the wxTiny functions in the file "wxTiny.h".
 ******************************************************************************/

static Game game = NULL; // defined here to be available to the "root" functions

/******************************************************************************
 * tyAppName - specify the name of the app
 ******************************************************************************/
tyStr tyAppName(void) {
	return APP_NAME;
}

/******************************************************************************
 * tyWidth - specify the width of the window
 ******************************************************************************/
int tyWidth(void) {
	return WORLD_SIZE_X * ACTOR_PIXELS_X;
}

/******************************************************************************
 * tyHeight - specify the height of the window
 ******************************************************************************/
int tyHeight(void) {
	return WORLD_SIZE_Y * ACTOR_PIXELS_Y;
}

/******************************************************************************
 * tyStatusItems - specify the number of slots available in the status bar
 ******************************************************************************/
int tyStatusItems(void) {
	return STATUS_ITEMS;
}

/******************************************************************************
 * tyMenus - specify the menus
 ******************************************************************************/
tyStr tyMenus(void) {
	return ":+Menu"
			":&About\tCtrl-A"
			":-"
			":&Restart\tCtrl-R"
			":-"
			":&Quit\tCtrl-Q";
}

/******************************************************************************
 * tyHandleMenuCommand - Handle the menu commands
 ******************************************************************************/
void tyHandleMenuCommand(tyStr command) {
	if (strcmp(command, "About") == 0)
		comandAbout();
	else if (strcmp(command, "Restart") == 0)
		comandRestart();
	else if (strcmp(command, "Quit") == 0)
		comandFinish();
	else
		tyFatalError("Unknown command: \"%s\"", command);
}

/******************************************************************************
 * tyHandleRedraw - redraw the window
 *
 * Automatically called in rare situations, for example if the game window is
 * minimized and then maximized
 ******************************************************************************/
void tyHandleRedraw(void) {
	gameRedraw(game);
}

/******************************************************************************
 * tyHandleTime - Called periodically, around 10 time per second
 *
 * This function does the following: animate the actors; update the status bar;
 *	possibly more things
 ******************************************************************************/
void tyHandleTime(void) {
	status();
	gameAnimation(game);
}

/******************************************************************************
 * tyHandleStart - Program initialization
 ******************************************************************************/
void tyHandleStart(void) {
	tySecondsSetZero();
	game = gameInit(game);
}
