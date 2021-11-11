#pragma once

#include <iostream>
#include <string>
#include <fstream>

const int row = 6;
const int col = 7;
int num;
int temp1{};
int temp2{};
int tempcounter = 0;

std::string boardvalues[row][col]{};
std::string beginningInput{};
std::string buff{};
std::fstream saveFile("D:\\Visual Studio Projects\\Oblig 3\\save.txt");
std::streampos begin, end;

bool player1Turn = true;

void initBoard();
void drawBoard();
void playGame(bool vsAI);
void placePiece(int slot, bool player1);
void clearCin();
void postGameProcessing(int temp);
void saveSettings();
void saveGame();
int endgame(int placement);


