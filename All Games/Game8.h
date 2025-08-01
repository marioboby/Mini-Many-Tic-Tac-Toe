//
// Created by Dopamine on 12/9/2024.
//

#ifndef C___PROJECTS_GAME8_H
#define C___PROJECTS_GAME8_H

#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;

// SmallBoard Implementation
template <typename T>
class SmallBoard : public Board<T> {
public:
    SmallBoard() {
        this->rows = 3;
        this->columns = 3;
        this->board = new T*[3];
        for (int i = 0; i < 3; i++) {
            this->board[i] = new T[3]();
        }
    }

    ~SmallBoard() {
        for (int i = 0; i < 3; i++) delete[] this->board[i];
        delete[] this->board;
    }

    bool update_board(int x, int y, T symbol) override {
        if (x < 0 || x >= 3 || y < 0 || y >= 3 || this->board[x][y] != 0)
            return false;
        this->board[x][y] = symbol;
        this->n_moves++;
        return true;
    }

    void display_board() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << (this->board[i][j] == 0 ? '.' : this->board[i][j]) << " ";
            }
            cout << endl;
        }
    }

    bool is_win() override {
        for (int i = 0; i < 3; i++) {
            if (this->board[i][0] != 0 && this->board[i][0] == this->board[i][1] && this->board[i][1] == this->board[i][2])
                return true;
            if (this->board[0][i] != 0 && this->board[0][i] == this->board[1][i] && this->board[1][i] == this->board[2][i])
                return true;
        }
        if (this->board[0][0] != 0 && this->board[0][0] == this->board[1][1] && this->board[1][1] == this->board[2][2])
            return true;
        if (this->board[0][2] != 0 && this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0])
            return true;
        return false;
    }

    bool is_draw() override {
        if (is_win()) return false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (this->board[i][j] == 0) return false;
            }
        }
        return true;
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

// UltimateBoard Implementation
template <typename T>
class UltimateBoard : public Board<T> {
private:
    T bigBoard[3][3][3][3] = {};
    T mainBoard[3][3] = {};

public:
    UltimateBoard() {
        this->rows = 9;
        this->columns = 9;
    }

    bool update_board(int x, int y, T symbol) override {
        int bigRow = x / 3, bigCol = y / 3;       // Small board indices
        int smallRow = x % 3, smallCol = y % 3;  // Cell indices within the small board

        if (bigRow < 0 || bigRow >= 3 || bigCol < 0 || bigCol >= 3 ||
            smallRow < 0 || smallRow >= 3 || smallCol < 0 || smallCol >= 3 ||
            mainBoard[bigRow][bigCol] != 0 || bigBoard[bigRow][bigCol][smallRow][smallCol] != 0) {
            return false;
        }

        bigBoard[bigRow][bigCol][smallRow][smallCol] = symbol;

        if (check_small_board(bigRow, bigCol, symbol)) {
            mainBoard[bigRow][bigCol] = symbol;
            cout << "Player " << symbol << " has won the small board ("
                 << bigRow + 1 << ", " << bigCol + 1 << ")!" << endl;
        }
        return true;
    }

    void display_board() override {
        cout << "\nUltimate Tic Tac Toe Board:\n";
        for (int bigRow = 0; bigRow < 3; bigRow++) {
            for (int smallRow = 0; smallRow < 3; smallRow++) {
                for (int bigCol = 0; bigCol < 3; bigCol++) {
                    cout << "| ";
                    for (int smallCol = 0; smallCol < 3; smallCol++) {
                        T val = bigBoard[bigRow][bigCol][smallRow][smallCol];
                        cout << (val == 0 ? '.' : val) << " ";
                    }
                    cout << "| ";
                }
                cout << endl;
            }
            if (bigRow < 2) cout << string(29, '-') << endl;
        }
    }

    bool is_win() override {
        for (int i = 0; i < 3; i++) {
            if ((mainBoard[i][0] != 0 && mainBoard[i][0] == mainBoard[i][1] && mainBoard[i][1] == mainBoard[i][2]) ||
                (mainBoard[0][i] != 0 && mainBoard[0][i] == mainBoard[1][i] && mainBoard[1][i] == mainBoard[2][i]))
                return true;
        }
        if ((mainBoard[0][0] != 0 && mainBoard[0][0] == mainBoard[1][1] && mainBoard[1][1] == mainBoard[2][2]) ||
            (mainBoard[0][2] != 0 && mainBoard[0][2] == mainBoard[1][1] && mainBoard[1][1] == mainBoard[2][0]))
            return true;
        return false;
    }

    bool is_draw() override {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (mainBoard[r][c] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }

    bool check_small_board(int row, int col, T symbol) {
        for (int i = 0; i < 3; i++) {
            if (bigBoard[row][col][i][0] == symbol && bigBoard[row][col][i][1] == symbol && bigBoard[row][col][i][2] == symbol)
                return true;
            if (bigBoard[row][col][0][i] == symbol && bigBoard[row][col][1][i] == symbol && bigBoard[row][col][2][i] == symbol)
                return true;
        }
        if (bigBoard[row][col][0][0] == symbol && bigBoard[row][col][1][1] == symbol && bigBoard[row][col][2][2] == symbol)
            return true;
        if (bigBoard[row][col][0][2] == symbol && bigBoard[row][col][1][1] == symbol && bigBoard[row][col][2][0] == symbol)
            return true;
        return false;
    }
};

// Player Implementations
template <typename T>
class UltimatePlayer : public Player<T> {
public:
    UltimatePlayer(string n, T symbol) : Player<T>(n, symbol) {}

    void getmove(int& x, int& y) override {
        cout << this->name << ", enter your move (row col): ";
        cin >> x >> y;
        x -= 1;
        y -= 1;
    }
};

template <typename T>
class UltimateRandomPlayer: public RandomPlayer<T>{
public:
    UltimateRandomPlayer(T symbol);
    void getmove(int &x, int &y);
};

template <typename T>
UltimateRandomPlayer<T>::UltimateRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 9;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void UltimateRandomPlayer<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;  // Random number between 0 and 8 for x (row)
    y = rand() % this->dimension;  // Random number between 0 and 8 for y (column)
}


// Main Menu and Execution
void showMenu() {
    cout << "\nWelcome to Ultimate Tic Tac Toe!" << endl;
    cout << "1. Play against another player." << endl;
    cout << "2. Play against a random computer." << endl;
    cout << "Enter your choice: ";
}

void Game8(){
    UltimateBoard<char> board;

    Player<char>* players[2];

    int choice;
    showMenu();
    cin >> choice;

    if (choice == 1) {
        string player1Name, player2Name;
        cout << "Enter Player 1's name: ";
        cin >> player1Name;
        cout << "Enter Player 2's name: ";
        cin >> player2Name;

        players[0] = new UltimatePlayer<char>(player1Name, 'X');
        players[1] = new UltimatePlayer<char>(player2Name, 'O');
    } else if (choice == 2) {
        string player1Name;
        cout << "Enter Player 1's name: ";
        cin >> player1Name;

        players[0] = new UltimatePlayer<char>(player1Name, 'X');
        players[1] = new UltimateRandomPlayer<char>('O');  // Random player for Player 2
    } else {
        cout << "Invalid choice!" << endl;
        return;
    }

    // Set the board for both players
    for (int i = 0; i < 2; i++) players[i]->setBoard(&board);

    int currentPlayer = 0;
    while (!board.game_is_over()) {
        board.display_board();  // Display the board before the move

        // Show the current player's turn
        cout << players[currentPlayer]->getname() << "'s turn (" << players[currentPlayer]->getsymbol() << "):\n";

        int x, y;
        // Get the move and update the board
        do {
            players[currentPlayer]->getmove(x, y);
        } while (!board.update_board(x, y, players[currentPlayer]->getsymbol()));  // Ensure move is valid

        // Check if the game is won or drawn after each move
        if (board.is_win()) {
            board.display_board();
            cout << players[currentPlayer]->getname() << " wins!" << endl;
            break;
        }

        if (board.is_draw()) {
            board.display_board();
            cout << "It's a draw!" << endl;
            break;
        }

        // Switch to the other player
        currentPlayer = 1 - currentPlayer;
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < 2; i++) delete players[i];
}

#endif //C___PROJECTS_GAME8_H
