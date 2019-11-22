//Alice Chambers
//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <fstream>              // file I/O
#include <vector>				// to use vectors
using namespace std;
//include our own libraries
#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.
#include <windows.h>

//defining the size of the grid
const int  SIZEX(20);    	//horizontal dimension
const int  SIZEY(15);		//vertical dimension
//defining symbols used for display of the grid and content
const char SPOT('0');   	//spot
const char BODY('O');		//body
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char MOUSE('@');		//mouse
const char PILL('+');		//power up pill
const char GOOSE('M');		//mongoose threat
//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
//defining the other command letters
const char CHEAT('C');      //to cheat
const char QUIT('Q');		//to end the game
const char BESTSCORES('B');	//to pause the game for best scores record
int pillTime = 0;

struct Item {			//items to be placed in the maze
	int x, y;
	char symbol;
	bool visible = false;
};

struct playerDetails {
	string bestPlayerName = "anonymous";
	int bestPlayerScore = -1;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
	void preStartGame(string & playerName);
	void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item & spot, vector<Item> snake, Item & pill, Item body, Item & mouse, Item & goose, int& mouseAmount);
	void renderGame(const char g[][SIZEX], const string & mess, int& mouseAmount, int& scoreAmount, string & playerName, int& highScore);
	bool updateGame(char grid[][SIZEX], char maze[SIZEY][SIZEX], Item & spot, Item & mouse, Item & pill, Item & goose, const int keyCode, string & mess, int& mouseAmount, int& scoreAmount, int& currentDirection, vector<Item> & snake, bool& hasCheated);
	bool wantsToQuit(const int key);
	bool wantsToCheat(const int key, bool& cheatCalled);
	bool wantsToBS(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void endProgram(int key, int& mouseAmount, Item & spot, char g[][SIZEX], string & playerName, int& scoreAmount, bool& hasCheated, int& highScore);
	void sleepAmount();
	void IfCheat(string & message, int& key, vector<Item> & snake, Item & spot, char g[][SIZEX]);
	void bestScoreSave(string & playerName, int& scoreAmount, bool& hasCheated, int& highScore, int& mouseAmount, const bool cheatCalled);
	void bestScoreOpen(string & playerName, int& highScore);
	void bestScoresRecord();
	void loadTopScores(vector<playerDetails> & record);
	void updateTopScores(vector<playerDetails> & record, int highScore, string playerName);
	void saveTopScores(vector<playerDetails> & record);

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT, true }; 	//spot's position and symbol
	vector<Item> snake = { spot };      //spot being passed in as an Item of the vector 'body'
	Item body = { 0,0,BODY, true };		//item for body of the snake
	for (int i = 0; i < 3; ++i)			//3 body pieces inserted at the end of the vector
	{
		snake.push_back(body);
	}
	Item mouse = { 0, 0, MOUSE };		//mouse position and symbol
	Item pill = { 0, 0, PILL };			//pill's postion and symbol
	Item goose = { 0, 0, GOOSE };		//mongoose's position and symbol
	string message("LET'S START...");	//starting message to player
	string playerName;					//the players name that they input at the start of the program

	int mouseAmount = 0;				//the amount of mice caught
	int scoreAmount = 0;				//the score of the current player
	int highScore = 0;					//this players best score so far
	int currentDirection = LEFT;		//the current direction of the snake (automatic motion)

	int key;																		//current key selected by player
	bool gameOver = false;															//game over set to false 
	bool hasCheated = false;														//has cheated set to false until C is pressed
	bool cheatCalled = false;														//cheat called set to false until the cheat function has been called
	bool hasPaused = false;

	vector<playerDetails> record;
	ifstream file("bestScoreRecord.txt");
	if (file.fail())
	{
		playerDetails initpd;
		for (int i = 0; i < 6; i++)
		{
			record.push_back(initpd);
		}
		saveTopScores(record);
	}

	loadTopScores(record);
	seed();																			//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Snake Game - Group 5");
	preStartGame(playerName);														//appears before the game starts
	bestScoreOpen(playerName, highScore);											//finds the saved best score for the player name inputted
	initialiseGame(grid, maze, spot, snake, pill, body, mouse, goose, mouseAmount);  //initialises the game

	do {
		renderGame(grid, message, mouseAmount, scoreAmount, playerName, highScore);	//display game info, modified grid and messages
		sleepAmount();
		if (_kbhit())																//a key has been pressed. This code was created with help from a question email from Pascale
		{
			key = getKeyPress();													//read in selected key: arrow or letter command
		}
		else
		{
			key = currentDirection;													//keep going in same direction (for automatic motion)
		}

		if (isArrowKey(key))														//if the key is an arrow update the game
		{
			gameOver = updateGame(grid, maze, spot, mouse, pill, goose, key, message, mouseAmount, scoreAmount, currentDirection, snake, hasCheated);
		}
		else if (wantsToBS(key))
		{
			bestScoresRecord();
			hasPaused = !hasPaused;
		}
		else if (wantsToCheat(key, cheatCalled) && hasCheated == false)				//if the user presses the cheat key it will be activated
		{
			IfCheat(message, key, snake, spot, grid);								//calls the cheat function
			hasCheated = !hasCheated;												// sets cheat to the opposite of what it is already
		}
		else if (wantsToCheat(key, cheatCalled) && hasCheated == true)				//if cheat is already activated
		{
			void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
			showMessage(clBlack, clRed, 40, 20, "CHEAT MODE IS DEACTIVATED");
			hasCheated = !hasCheated;
		}
		else
		{
			message = "INVALID KEY!     ";											//set 'Invalid key' message
		}

	} while (!wantsToQuit(key) && gameOver == false);												//while user does not want to quit and the gameOver is still set to false

	renderGame(grid, message, mouseAmount, scoreAmount, playerName, highScore);						//display game info, modified grid and messages
	endProgram(key, mouseAmount, spot, maze, playerName, scoreAmount, hasCheated, highScore);		//displays end game messages
	bestScoreSave(playerName, scoreAmount, hasCheated, highScore, mouseAmount, cheatCalled);		//saves the score of the player (according to the conditions on the spec)
	updateTopScores(record, highScore, playerName);
	saveTopScores(record);
	return 0;
}

//---------------------------------------------------------------------------
//----- pre game state
//---------------------------------------------------------------------------

void preStartGame(string& playerName)																				//text displayed before the game begins
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
	showMessage(clBlack, clMagenta, 20, 10, "SNAKE");
	showMessage(clBlack, clMagenta, 20, 11, "Enter your name: ");													//the user can enter their name for it to be displayed in the game 
	cin >> playerName;

	while (playerName.length() > 20)																				//user must input less than 20 characters
	{
		clrscr();
		showMessage(clBlack, clMagenta, 20, 10, "Try again, (MAX 20 CHARACTERS) ");
		showMessage(clBlack, clMagenta, 20, 11, "Enter your name: ");
		cin >> playerName;
	}

	clrscr();
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void sleepAmount()										//sets the sleep amount so it gets increasingly faster as the players score increases
{
	Sleep(600);
}

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, vector<Item> snake, Item& pill, Item body, Item& mouse, Item& goose, int& mouseAmount)
{
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(Item & spot, Item body, char maze[SIZEY][SIZEX]);
	void setMouseInitalCoordinates(Item & mouse, char maze[SIZEY][SIZEX]);
	void setPillInitalCoordinates(Item & pill, char maze[SIZEY][SIZEX]);
	void setMongooseInitalCoordinates(Item & goose, char maze[SIZEY][SIZEX]);
	void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item & spot, vector<Item> snake, Item & mouse, Item & pill, Item & goose, int& mouseAmount);

	setInitialMazeStructure(maze);										//initialise maze
	setSpotInitialCoordinates(spot, body, maze);						//sets the spot in the grid
	setMouseInitalCoordinates(mouse, maze);								//sets the mouse in the grid
	setPillInitalCoordinates(pill, maze);								//sets the pill in the grid
	setMongooseInitalCoordinates(goose, maze);							//sets the mongoose in the grid
	updateGrid(grid, maze, spot, snake, mouse, pill, goose, mouseAmount);		//prepare and update grid
}

void setSpotInitialCoordinates(Item& spot, Item body, char maze[SIZEY][SIZEX])
{ //set spot coordinates inside the grid at random at beginning of game
	do {
		spot.y = 7;      //vertical coordinate, in middle to start
		spot.x = 10;      //horizontal coordinate, in middle to start
	} while (maze[spot.y][spot.x] != TUNNEL);
}

void setMouseInitalCoordinates(Item& mouse, char maze[SIZEY][SIZEX])
{//set mouse coordinates inside the grid at random at beginning of game
	do {
		mouse.y = random(SIZEY - 2);
		mouse.x = random(SIZEX - 2);
	} while (maze[mouse.y][mouse.x] != TUNNEL);
	mouse.visible = true;
}

void setPillInitalCoordinates(Item& pill, char maze[SIZEY][SIZEX])
{//set pill coordinates inside the grid at random at beginning of game
	do {
		pill.y = random(SIZEY - 2);
		pill.x = random(SIZEX - 2);
	} while (maze[pill.y][pill.x] != TUNNEL);
	pill.visible = true;

}

void setMongooseInitalCoordinates(Item& goose, char maze[SIZEY][SIZEX])
{//set goose coordinates inside the grid at random at beginning of game
	do {
		goose.y = random(SIZEY - 2);
		goose.x = random(SIZEX - 2);
	} while (maze[goose.y][goose.x] != TUNNEL);
	goose.visible = true;

}

void setInitialMazeStructure(char maze[SIZEY][SIZEX])
{
	//set the position of the walls in the maze
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used are changed
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			}
}
//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------
bool updateGame(char grid[][SIZEX], char maze[SIZEY][SIZEX], Item& spot, Item& mouse, Item& pill, Item& goose, const int keyCode, string& mess, int& mouseAmount, int& scoreAmount, int& currentDirection, vector<Item>& snake, bool& hasCheated)
{
	//function declarations
	bool updateGameData(char g[][SIZEX], Item & spot, Item & mouse, Item & pill, Item & goose, const int key, string & mess, int& mouseAmount, int& scoreAmount, char maze[SIZEY][SIZEX], int& currentDirection, vector<Item> & snake, bool& hasCheated);
	void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item & spot, vector<Item> snake, Item & mouse, Item & pill, Item & goose, int& mouseAmount);
	bool gameOver = updateGameData(grid, spot, mouse, pill, goose, keyCode, mess, mouseAmount, scoreAmount, maze, currentDirection, snake, hasCheated);
	updateGrid(grid, maze, spot, snake, mouse, pill, goose, mouseAmount);

	return gameOver;
}
bool updateGameData(char g[][SIZEX], Item& spot, Item& mouse, Item& pill, Item& goose, const int key, string& mess, int& mouseAmount, int& scoreAmount, char maze[SIZEY][SIZEX], int& currentDirection, vector<Item>& snake, bool& hasCheated)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy, int& currentDirection);
	assert(isArrowKey(key));
	void updateTailPosition(vector<Item> & snake, Item & spot);
	bool gameOver = false;
	//bool wantsToCheat(const int key, bool& cheatCalled);
	void addTailLength(vector<Item> & snake);
	void resetTailLength(vector<Item> & snake);

	int sec = 120;
	if (isArrowKey(key))
	{
		setMongooseInitalCoordinates(goose, maze);

		//TIMER
		sec--;
		showMessage(clBlack, clMagenta, 90, 0, "Seconds Left: ");
		cout << sec;

	}
	if (sec == 0)
	{
		gameOver = true;
		mess = "You ran out of time! YOU LOSE!";
	}



	//reset message to blank
	mess = "                                                        ";

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy, currentDirection);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y][spot.x])
	{					//...depending on what's on the position in grid...
	case TUNNEL:

		++scoreAmount;	//add 1 to the current players score
		if (pill.visible == true)
		{
			++pillTime;

			if (pillTime == 10)
			{
				pill.visible = false;
				pillTime = 0;
			}

		}
		updateTailPosition(snake, spot);
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;

	case WALL:  		//hit a wall and stay there

		++scoreAmount;	//add 1 to the current players score
		gameOver = true;//game is over when snake bumps into itself
		mess = "Snake bumped into a wall! YOU LOSE!";

		updateTailPosition(snake, spot);
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;

	case MOUSE:

		mess = "MOUSE CAUGHT!                  ";
		++mouseAmount;

		if (mouseAmount == 10)
		{
			gameOver = true;
		}
		setMouseInitalCoordinates(mouse, maze);
		if (hasCheated == false) { addTailLength(snake); }											//adds 2 tail pices when a mouse is collected
		if ((mouseAmount % 2 == 0) && mouseAmount != 0) { setPillInitalCoordinates(pill, maze); }	//sets pill co ordinates after 2 mice have been collected

		updateTailPosition(snake, spot);
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;

	case PILL:

		mess = "PILL EATEN - SNAKE TAIL RESET!";
		resetTailLength(snake);								//tail is reset to 3 cells long
		pill.visible = false;								//pill is removed from the maze

		updateTailPosition(snake, spot);			//reset the tail back to 3 cells long
		spot.y += dy;								//go in that Y direction
		spot.x += dx;								//go in that X direction
		break;

	case GOOSE:

		gameOver = true;
		mess = "Snake bumped into the Mongoose! YOU LOSE!";
		break;

	case BODY:

		gameOver = true;
		mess = "Snake bumped into itself! YOU LOSE!";
		break;
	}

	return gameOver;
}

void addTailLength(vector<Item>& snake)
{
	Item newPiece = { snake[snake.size() - 1].x, snake[snake.size() - 1].y, BODY, true };
	snake.push_back(newPiece);
	snake.push_back(newPiece);
}

void resetTailLength(vector<Item>& snake)
{
	while (snake.size() > 4) { snake.pop_back(); }
}

void updateTailPosition(vector<Item>& snake, Item& spot)
{
	for (size_t i = snake.size() - 1; i > 0; i--) //For the movement of the remaining body pieces
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	snake[0].x = spot.x;
	snake[0].y = spot.y;
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item& spot, vector<Item> snake, Item& mouse, Item& pill, Item& goose, int& mouseAmount)
{ //update grid configuration after each move
	void placeMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item & spot);
	void placeSnake(char g[][SIZEX], vector<Item> & snake);

	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, mouse);	//set mouse in grid
	placeSnake(grid, snake);//set snake on grid

	if (mouseAmount % 2 == 0 && mouseAmount != 0)
	{
		placeItem(grid, pill);  //set pill in grid if an even number of mice have been collected	
	}

	if (mouseAmount >= 3)
	{
		placeItem(grid, goose);
	}

}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item& item)
{ //place item at its new position in grid
	if (item.visible == true)
	{
		g[item.y][item.x] = item.symbol;
	}
}

void placeSnake(char g[][SIZEX], vector<Item>& snake)
{ //place item at its new position in grid
	for (int i = 0; i < snake.size(); ++i)
	{
		placeItem(g, snake.at(i));
	}
}

void IfCheat(string& message, int& key, vector<Item>& snake, Item& spot, char g[][SIZEX])
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
	showMessage(clBlack, clWhite, 40, 20, "CHEAT MODE IS ACTIVATED     ");

	for (int i = 0; i < 3; i++)
	{
		Beep(2450, 500);
	}

	resetTailLength(snake);
}

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy, int& currentDirection)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	char k = (char)toupper(key);
	switch (k)	//...depending on the selected key...
	{
	case 'A': case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		currentDirection = LEFT;
		break;
	case 'D': case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		currentDirection = RIGHT;
		break;
	case 'W': case UP:
		dx = 0;     //when UP arrow pressed...
		dy = -1;	//increase the Y coordinate
		currentDirection = UP;
		break;
	case 'S': case DOWN:
		dx = 0;		//when DOWN arrow pressed...
		dy = +1;	//increase the X coordinate
		currentDirection = DOWN;
		break;
	}
}

int getKeyPress()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'W', 'A', 'S' and 'D')
	char k = (char)toupper(key);
	return (k == LEFT) || (k == RIGHT) || (k == UP) || (k == DOWN) || (k == 'W') || (k == 'A') || (k == 'S') || (k == 'D');
}
bool wantsToQuit(const int key)
{
	char k = (char)toupper(key);              //check if the user wants to quit (when key is 'Q' or 'q')
	return k == QUIT;
}

bool wantsToCheat(const int key, bool& cheatCalled)
{
	cheatCalled = true;
	char k = (char)toupper(key);                 //check if the user wants to cheat (when key is 'C' or 'c')
	return k == CHEAT;
}

bool wantsToBS(const int key)
{
	char k = (char)toupper(key);                 //check if the user wants to pause(best scores record) (when key is 'P' or 'p')
	return k == BESTSCORES;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x)
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message;
}

void renderGame(const char g[][SIZEX], const string& mess, int& mouseAmount, int& scoreAmount, string& playerName, int& highScore)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	string tostring(int x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);
	void paintGrid(const char g[][SIZEX]);
	//display game title
	showMessage(clBlack, clMagenta, 0, 0, "___SNAKE GAME___");

	showMessage(clWhite, clDarkCyan, 40, 0, "Date and time: ");
	cout << getDate() << "   ";
	cout << getTime();

	showMessage(clWhite, clDarkCyan, 40, 1, "Alice Chambers      SE1   B8010489");

	//display menu options available

	showMessage(clWhite, clMagenta, 40, 5, "TO MOVE - USE KEYBOARD ARROWS ");
	showMessage(clWhite, clMagenta, 40, 6, "TO QUIT - ENTER 'Q'           ");
	showMessage(clWhite, clMagenta, 40, 7, "TO TOGGLE CHEAT - ENTER 'C' (keep snake at length 3)");
	showMessage(clWhite, clMagenta, 40, 8, "TO SHOW BEST SCORES - ENTER 'B'");
	showMessage(clBlack, clWhite, 40, 9, "Players name: ");
	cout << playerName;

	showMessage(clBlack, clWhite, 40, 10, "Best score so far: ");
	cout << highScore;

	showMessage(clBlack, clWhite, 40, 11, "Current Score: ");
	cout << scoreAmount;

	showMessage(clBlack, clWhite, 40, 12, "Mice Collected: ");
	cout << mouseAmount;
	showMessage(clBlack, clWhite, 57, 12, " out of 10");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 16, mess);	//display current message

	//display grid contents
	paintGrid(g);
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	selectBackColour(clBlack);
	selectTextColour(clWhite);
	gotoxy(0, 2);

	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
		{
			switch (g[row][col])
			{
			case TUNNEL: case WALL: selectTextColour(clWhite); break;
			case SPOT: selectTextColour(clGreen); break;
			case BODY: selectTextColour(clMagenta); break;
			case MOUSE: selectTextColour(clYellow); break;
			case PILL: selectTextColour(clCyan); break;
			case GOOSE: selectTextColour(clDarkBlue); break;

			}
			cout << g[row][col];	//output cell content
		}
		cout << endl;
	}
}

void endProgram(int key, int& mouseAmount, Item& spot, char g[][SIZEX], string& playerName, int& scoreAmount, bool& hasCheated, int& highScore)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);

	if (wantsToQuit(key) && mouseAmount < 10)
	{
		showMessage(clBlack, clYellow, 0, 20, "Player wants to quit! YOU LOSE!                  ");
		cout << endl;
	}

	if (mouseAmount == 10)
	{
		showMessage(clBlack, clYellow, 0, 20, "You collected 10 mice, well done! YOU WON!        ");
		cout << endl;
	}
	showMessage(clBlack, clYellow, 0, 21, "");

	system("pause");	//hold output screen until a keyboard key is hit
}

void bestScoreSave(string& playerName, int& scoreAmount, bool& hasCheated, int& highScore, int& mouseAmount, const bool cheatCalled)
{
	if (cheatCalled == false)
	{
		ofstream bestScore;
		bestScore.open(playerName + ".txt", ios::out);

		if (highScore == 500)
		{
			highScore = scoreAmount;
		}
		if (scoreAmount > highScore)
		{
			bestScore << scoreAmount;
			bestScore.close();
		}
		else
		{
			bestScore << highScore;
			bestScore.close();
		}

	}
}

void bestScoreOpen(string& playerName, int& highScore)
{
	ifstream bestScore;
	bestScore.open(playerName + ".txt", ios::in);
	if (bestScore.fail())
	{
		highScore = 500;
	}
	else
	{
		bestScore >> highScore;
		bestScore.close();
	}
}

void bestScoresRecord()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message);

	clrscr();
	cout << "BEST SCORES" << endl;
	ifstream file;
	file.open("bestScoreRecord.txt", ios::in);
	char character;
	while ((character = file.get()) != EOF)
		cout.put(character);
	cout << endl;
	file.close();

	showMessage(clBlack, clRed, 25, 15, "PAUSED - BEST SCORES        ");

	system("PAUSE");
	clrscr();
}

void loadTopScores(vector<playerDetails>& record)
{
	ifstream file;
	file.open("bestScoreRecord.txt", ios::in);

	for (int i = 0; i < 6; i++)
	{
		playerDetails pd;
		getline(file, pd.bestPlayerName);
		string score;
		getline(file, score);
		pd.bestPlayerScore = stoi(score);

		record.push_back(pd);
	}
	file.close();

}

void updateTopScores(vector<playerDetails>& record, int highScore, string playerName)
{
	for (int i = 0; i < 6; i++)
	{
		if (highScore > record[i].bestPlayerScore&& record[i].bestPlayerName != playerName)
		{
			playerDetails pd;
			pd.bestPlayerName = playerName;
			pd.bestPlayerScore = highScore;
			record.insert(record.begin() + i, pd);
			record.pop_back();
			break;
		}
	}
}

void saveTopScores(vector<playerDetails>& record)
{
	ofstream file;
	file.open("bestScoreRecord.txt", ios::out);

	for (int i = 0; i < 6; i++)
	{
		file << record[i].bestPlayerName << endl << record[i].bestPlayerScore << endl;
	}

	file.close();
}

