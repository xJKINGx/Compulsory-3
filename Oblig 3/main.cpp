#include "ConnectFour.h"

int main() {
	saveGame();
	player1Turn = true;

	std::srand(time(0));
	initBoard();

	std::cout << "Welcome to connect four! What is the name of the second player? (Type 'AI' to play against AI" <<
		std::endl;
	std::cout << "Type Save if you wish to access the save settings" << std::endl;
	std::cout << "Type Exit if you wish to exit the program" << std::endl;
	getline(std::cin, beginningInput);

	if (beginningInput == "AI" || beginningInput == "ai") {
		playGame(true);
	}
	else if (beginningInput == "Save" || beginningInput == "save") {
		saveSettings();
		main();
	}
	else if (beginningInput == "Exit" || beginningInput == "exit") {
		saveFile.close();
		exit(0);
	}
	else {
		playGame(false); 
		// If it reaches this point we know the user doesn't want to exit, save or play against AI
		// so this else statement menas that it will be player vs player
	}

	return 0;
}

void initBoard() {
	// This function fills the array will *, this also works as a board resetting function
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			boardvalues[i][j] = "*";
		}
	}
}

void drawBoard() {
	std::cout << "   1  2  3  4  5  6  7 ";
	std::cout << std::endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (j % col == 0) {
				std::cout << "| ";
			}
			std::cout << " " << boardvalues[i][j] << " ";
		}
		std::cout << " |" << std::endl;
	}
}

void playGame(bool vsAI) {
	drawBoard();
	int temp{};
	bool gameLoop = true;

	while (gameLoop) {
		if (player1Turn) {
			std::cout << "Player 1, place your piece: ";
			std::cin >> temp;
			clearCin();
			placePiece(temp, true);
		}
		else if (vsAI && player1Turn != true) {
			// VS AI
			placePiece((std::rand() % 7 + 1), false);
		}
		else {
			std::cout << "Player 2, place your piece: ";
			std::cin >> temp;
			clearCin();
			placePiece(temp, false);
		}
		
	}
}

void placePiece(int slot, bool player1) {
	if ((slot - 1) <= -1) {
		// This is to stop players from typing values 0 and lower
		std::cout << "Wrong input type, please enter a positive integer" << std::endl;
	}
	else if (slot > 7) {
		// This is to stop players from typing values higher than 7
		std::cout << "Please enter a number between 1 and 7" << std::endl;
	}
	else if (boardvalues[0][slot - 1] != "*") {
		std::cout << "This column is full! Place the piece somewhere else." << std::endl;
	}
	// Together the three if sentences above make sure the input is valid.
	// If none of the if sentences runs, then we know the input is valid, and we can place a piece
	else {
		for (int i = 0; i < col; i++) {
			// In order to make sure that a piece is being placed and that there is no available
			// space beneath it, we must first check if the piece below is available, or *.
			// If so then we let i be incremented, and we are now in the place we just scouted
			// and confirmed was available. Then we repeat this process of checking beneath us
			// until we have found either a placed piece, X or O, or if there is nothing, which means
			// we are at the bottom of the board. When there is no available slot beneath us, we will
			// check which turn it is, and place the correct piece at the bottom of the board
			// in the correct column

			if (boardvalues[i + 1][slot - 1] == "*") {
				// The piece below must be a "*"
				// Therefore we can move downwards
				continue;
			}
			else {
				// Either there is a placed piece below, or we are at the bottom, therefore, we place our piece
				if (player1) {
					boardvalues[i][slot - 1] = "X";
					player1Turn = false;
				}
				else {
					boardvalues[i][slot - 1] = "O";
					player1Turn = true;
				}
				break;
			}
		}
		drawBoard(); // We will draw the board again
		int endResult = endgame(slot); // Then we see if someone won or not
		postGameProcessing(endResult); // Then we process the result from endgame()

	}
	
}

void clearCin() { // clearCin() is used to clear cin buffer to get rid of bugs
	std::cin.clear();
	std::cin.ignore(32767, '\n');
}

int endgame(int placement) {
	int winning{}; // This variable is explained later in the postGameProcessing() function
	int xCounter = 0;
	int oCounter = 0;

	//
	// HORIZONTAL WINS
	//

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//It will go from left to right, checking if there is 4 in a row
			if (boardvalues[i][j] == "X") {
				// It will count the amount of times it encounters an X, if it is equal to 4 then we know we got a
				// 4 in a row, if it meets a O or *, then it't not a 4 in a row, and the counter breaks
				xCounter++;
				if (xCounter == 4) {
					winning = 1;
					break;
				}
			}
			else {
				xCounter = 0;
			}
			if (boardvalues[i][j] == "O") {
				oCounter++;
				if (oCounter == 4) {
					winning = 2;
					break;
				}
			}
			else {
				oCounter = 0;
			}
		}
	}

	//
	// VERTICAL WINS
	//
	for (int i = 0; i < row; i++) {
		// We should check the last piece placed and check if that piece completed a game
		// We need to only check vertically, so we don't need to go left to right
		// Otherwise it is pretty similar to checking horizontal wins
		if (boardvalues[i][placement - 1] == "X") {
			xCounter++;
			if (xCounter == 4) {
				winning = 1;
				break;
			}
		}
		else {
			xCounter = 0;
		}

		if (boardvalues[i][placement - 1] == "O") {
			oCounter++;
			if (oCounter == 4) {
				winning = 2;
				break;
			}
		}
		else {
			oCounter = 0;
		}
	}

	//
	// DIAGONAL WINS
	//

	for (int i = 0; i < 3; i++) {
		// i < 3 because only the 3 first rows can contain a diagonal victory
		for (int j = 0; j < col; j++) {
			// It will check the current space, and if it has an X it will go towards the lower right
			// It will continue until it gets to the win condition
			// Otherwise it will check the bottom left, and check almost the same thing as if it would check
			// to the bottom right
			if (boardvalues[i][j] == "X") {
				if (boardvalues[i + 1][j + 1] == "X") {
					if (boardvalues[i + 2][j + 2] == "X") {
						if (boardvalues[i + 3][j + 3] == "X") {
							winning = 1;
						}
					}
				}
				else if (boardvalues[i + 1][j - 1] == "X") {
					if (boardvalues[i + 2][j - 2] == "X") {
						if (boardvalues[i + 3][j - 3] == "X") {
							winning = 1;
						}
					}
				}
			}

			if (boardvalues[i][j] == "O") {
				if (boardvalues[i + 1][j + 1] == "O") {
					if (boardvalues[i + 2][j + 2] == "O") {
						if (boardvalues[i + 3][j + 3] == "O") {
							winning = 2;
						}
					}
				}
				else if (boardvalues[i + 1][j - 1] == "O") {
					if (boardvalues[i + 2][j - 2] == "O") {
						if (boardvalues[i + 3][j - 3] == "O") {
							winning = 2;
						}
					}
				}
			}
		}
	}

	// If player 1 nor player 2 has won the game yet, check the board if it is filled
	if (winning != 1 && winning != 2) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (boardvalues[i][j] == "*") {
					// It will check for available spaces, and if it finds one, then the board is not filled
					// and the game will continue
					winning = 4;
					break;
				}
			}
		}
	}

	// If none of the players has won and the board is filled, then it's a draw
	if (winning != 1 && winning != 2 && winning != 4) {
		winning = 3;
		// It's a draw
	}

	return winning;
}

void postGameProcessing(int temp) {
	// temp is the same as the winning variable
	// 1 = Player 1 win
	// 2 = Player 2 win
	// 3 = Draw
	// 4 = Continue

	std::string restartInput{};

	switch (temp) {
	case 1:
		std::cout << "PLAYER 1 WINS" << std::endl;
		temp1++;
		break;
	case 2:
		std::cout << "PLAYER 2 WINS" << std::endl;
		temp2++;
		break;
	case 3:
		std::cout << "GAME OVER - DRAW" << std::endl;
		break;
	case 4:
		break;
	default:
		break;
	}
	saveGame();
	// In other words, if the game is not continuing, give the user a prompt to play again
	if (temp != 4) {
		std::cout << "Do you want to play again? (Y/N): ";
		getline(std::cin, restartInput);
		if (restartInput == "Y" || restartInput == "y") {
			main();
		}
		else {
			exit(0);
		}
	}
}

void saveSettings() {
	std::cout << "Player 1: " << temp1 << " wins\nPlayer 2: " << temp2 << " wins" << std::endl;
}

void saveGame() {
	saveFile.seekg(0);

	while (saveFile >> num) {
		if (tempcounter == 1) {
			temp2 = num;
		}
		else {
			temp1 = num;
			tempcounter++;
		}
	}
	std::ofstream x("D:\\Visual Studio Projects\\Oblig 3\\save.txt", std::ofstream::trunc);
	x.close();
	std::fstream saveFile("D:\\Visual Studio Projects\\Oblig 3\\save.txt");

	saveFile << temp1 << "\n" << temp2;
	saveFile.close();
}