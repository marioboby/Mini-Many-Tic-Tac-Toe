//
// Created by Dopamine on 12/6/2024.
//

#ifndef C___PROJECTS_GAME5_H
#define C___PROJECTS_GAME5_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <set>
using namespace std;

// Numerical Tic-Tac-Toe Board Class
class NumericalTicTacToeBoard : public Board<int> {
private:
    set<int> used_numbers; // To track already used numbers

public:
    NumericalTicTacToeBoard() {
        rows = 3;
        columns = 3;
        board = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new int[columns]{0}; // Initialize with 0s
        }
    }

    ~NumericalTicTacToeBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, int symbol) override {
        // Check boundaries, empty cell, and unused number
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0 || used_numbers.count(symbol)) {
            cout << "Invalid move. Try again.\n";
            return 1;
        }
        board[x][y] = symbol;
        used_numbers.insert(symbol);
        n_moves++;
        return true;
    }

    void display_board() override {

        cout << "_______________________" << endl;
        cout << "     1    2    3" << endl;
        for (int i = 0; i < rows; ++i) {
            cout << i + 1;
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == 0) {
                    cout << " |   ";
                } else {
                    cout << " | " << board[i][j] << " ";
                }
            }
            cout << " |" << endl;
            if (i < rows - 1) cout << "  -----+----+-----" << endl;
        }
        cout << "_______________________" << endl;
    }

    bool is_win() override {
        // Check rows
        for (int i = 0; i < rows; ++i) {
            if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0 &&
                board[i][0] + board[i][1] + board[i][2] == 15)
                return true;
        }

        // Check columns
        for (int j = 0; j < columns; ++j) {
            if (board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0 &&
                board[0][j] + board[1][j] + board[2][j] == 15)
                return true;
        }

        // Check diagonals
        if (board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0 &&
            board[0][0] + board[1][1] + board[2][2] == 15)
            return true;
        if (board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0 &&
            board[0][2] + board[1][1] + board[2][0] == 15)
            return true;

        return false;
    }

    bool is_draw() override {
        return n_moves == 9 && !is_win(); // All cells filled and no winner
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

// HumanPlayer Class with Numerical Tic-Tac-Toe Rules
template <typename T>
class NumericalHumanPlayer : public Player<T> {
private:
    set<T> available_numbers; // Track available numbers for the player

public:
    NumericalHumanPlayer(string name, T symbol_start, bool is_odd) : Player<T>(name, symbol_start) {
        if (is_odd) {
            available_numbers = {1, 3, 5, 7, 9};
        } else {
            available_numbers = {2, 4, 6, 8};
        }
    }

    void getmove(int& x, int& y) override {
        T symbol;
        cout << this->name << " - Enter row (1-3), column (1-3), and number: ";
        cin >> x >> y >> symbol;

        x--; // Convert to 0-based indexing
        y--;

        if (available_numbers.count(symbol)) {
            this->symbol = symbol;
            available_numbers.erase(symbol);
        } else {
            cout << "Invalid number. Choose a valid number from your set.\n";
            getmove(x, y);
        }
    }
};

template <typename T>
class NumericalRandomPlayer : public RandomPlayer<T>{
public:
    NumericalRandomPlayer (T symbol);
    void getmove(int &x, int &y) ;
};


template <typename T>
NumericalRandomPlayer<T>::NumericalRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void NumericalRandomPlayer<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
}

void Game5(){
    auto* board = new NumericalTicTacToeBoard();

    // Player 1 is human and uses odd numbers
    auto* player1 = new NumericalHumanPlayer<int>("Player 1", 1, true);  // Odd numbers

    // Ask Player 1 if they want to play against a human or a random computer
    int choice;
    cout << "\nChoose your opponent:\n";
    cout << "1. Play against Player 2 (Human)\n";
    cout << "2. Play against Random Computer \n";
    cin >> choice;

    // Declare player2 as a pointer to the base class Player<int>
    Player<int>* player2 = nullptr;

    // Depending on the choice, create either a human Player 2 or a random computer player
    switch (choice) {
        case 1:
            player2 = new NumericalHumanPlayer<int>("Player 2", 2, false); // Even numbers
            break;
        case 2:
            player2 = new NumericalRandomPlayer<int>(2); // Random computer player with symbol 2 (even numbers)
            break;
        default:
            cout << "Invalid choice! Exiting...\n";
            delete board;
            return; // Exit on invalid input
    }

    // Set the board for both players
    player1->setBoard(board);
    player2->setBoard(board);

    // Create GameManager
    Player<int>* players[2] = {player1, player2};
    GameManager<int> gameManager(board, players);

    // Run the game
    gameManager.run();

    // Clean up
    delete board;
    delete player1;
    delete player2;
}

#endif //C___PROJECTS_GAME5_H
