//
// Created by Dopamine on 12/6/2024.
//

#ifndef C___PROJECTS_GAME6_H
#define C___PROJECTS_GAME6_H

#include "BoardGame_Classes.h"
#include <iostream>

bool GhostTurn = false;
bool Taken [3][3] = {false};
using namespace std;


class TicTacToeBoard : public Board<int> {
public:

    int count = 0;

    TicTacToeBoard() {
        rows = 3;
        columns = 3;
        board = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new int[columns]{0}; // Initialize with 0s
        }
    }

    ~TicTacToeBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, int symbol) override {
        if (GhostTurn) {
            return true;
        }
        if (cin.fail() || x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0) {
            cout << "Invalid move. Try again.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            return false;
        }
        board[x][y] = symbol;
        Taken[x][y] = true;
        n_moves++;
        return true;
    }

    void display_board() override {
        cout << endl;
        cout << "      1    2    3\n";
        for (int i = 0; i < rows; ++i) {
            cout << " " << i + 1;
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == 0)
                    cout << " |   ";
                else if (board[i][j] == 1)
                    cout << " | X ";
                else
                    cout << " | O ";
            }
            cout << " |\n   -----+----+-----\n";
        }
    }

    bool is_win() override {
        if (GhostTurn) {
            return true;
        }

        for (int i = 0; i < rows; ++i) {
            if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                GhostTurn = true;
                return false;
            }
            if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                GhostTurn = true;
                return false;
            }
        }
        if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            GhostTurn = true;
            return false;
        }
        if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            GhostTurn = true;
            return false;
        }

        return false;
    }

    bool is_draw() override {
        return n_moves == 9 && !is_win();
    }

    bool game_is_over() override {
        if (GhostTurn) return false;
        return is_win() || is_draw();
    }
};


template <typename T>
class Human_Player : public Player<T> {
public:
    Human_Player(string name, T symbol) : Player<T>(name, symbol) {}

    void getmove(int& x, int& y) override {
        if (GhostTurn) {
            return ;
        }
        cout << this->name << " (" << this->symbol << ") - Enter row and column (1-3): ";
        cin >> x >> y;
        x--; // Convert to 0-based indexing
        y--;
    }
};

template<typename T>
class InverseRandomPlayer : public RandomPlayer<T> {
public:
    InverseRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
        this->dimension = 3;
        this->name = "Random Computer Player";
        srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
    }

    void getmove(int& x, int& y) override {
        if (GhostTurn) {
            return ;
        }
        x = rand() % this->dimension;  // Random number between 0 and 2
        y = rand() % this->dimension;
        while (taken[x][y]) {
            x = rand() % this->dimension;  // Random number between 0 and 2
            y = rand() % this->dimension;
        }
    }
};

void Game6(){
    int choice;
    Player<int>* players[2];
    string playerXName, player2Name;
    // Create board and players
    TicTacToeBoard* board = new TicTacToeBoard();

    cout << "\nWelcome to Misere Tic Tac Toc Game. :)\n";

    // Set up player 1
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;



    switch(choice) {
        case 1:
            cout << "Enter Player X name: ";
            cin >> playerXName;
            players[0] = new Human_Player<int>(playerXName, 1);
            break;
        case 2:
            players[0] = new InverseRandomPlayer<int>(1);
            break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
            return;
    }

    // Set up player 2
    cout << "Choose Player 2 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;



    switch(choice) {
        case 1:
            cout << "Enter Player 2 name: ";
            cin >> player2Name;
            players[1] = new Human_Player<int>(player2Name, 2);
            break;
        case 2:
            players[1] = new InverseRandomPlayer<int>(2);
            break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
            return;
    }



    // Create GameManager
    GameManager<int> gameManager(board, players);

    // Run the game
    gameManager.run();
    // Clean up
    delete board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

}

#endif //C___PROJECTS_GAME6_H
