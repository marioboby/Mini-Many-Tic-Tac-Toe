//
// Created by Dopamine on 12/6/2024.
//

#ifndef C___PROJECTS_GAME1_H
#define C___PROJECTS_GAME1_H


#include "BoardGame_Classes.h"
#include <iostream>
using namespace std;

class Game3x5Board : public Board<int> {
public:
    Game3x5Board() {
        rows = 3;
        columns = 5;
        board = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new int[columns]{0}; // Initialize with 0s
        }

        // Set inactive cells (represented by 3)
        board[0][0] = board[0][1] = board[0][3] = board[0][4] = 3; // First row
        board[1][0] = board[1][4] = 3;                             // Second row
    }

    ~Game3x5Board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, int symbol) override {
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0) {
            //cout << "Invalid move. Try again.\n";
            return false;
        }
        board[x][y] = symbol;
        n_moves++;
        return true;
    }

    void display_board() override {
        cout << "\n     1      2      3      4      5\n";
        for (int i = 0; i < rows; ++i) {
            cout << " " << i + 1;
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == 0)
                    cout << " |   | ";
                else if (board[i][j] == 1)
                    cout << " | X | ";
                else if (board[i][j] == 2)
                    cout << " | O | ";
                else
                    cout << "       "; // Inactive cell
            }
            cout << endl;
        }
    }

    bool is_win() override {
        // Check horizontal lines
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns - 2; ++j) {
                if (board[i][j] != 0 && board[i][j] != 3 &&
                    board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2])
                    return true;
            }
        }

        // Check vertical lines
        for (int j = 0; j < columns; ++j) {
            if (board[0][j] != 0 && board[0][j] != 3 &&
                board[0][j] == board[1][j] && board[1][j] == board[2][j])
                return true;
        }

        // Check diagonals
        for (int i = 0; i < rows - 2; ++i) {
            for (int j = 0; j < columns; ++j) {
                // Top-left to bottom-right
                if (j + 2 < columns && board[i][j] != 0 && board[i][j] != 3 &&
                    board[i][j] == board[i + 1][j + 1] && board[i + 1][j + 1] == board[i + 2][j + 2])
                    return true;

                // Top-right to bottom-left
                if (j - 2 >= 0 && board[i][j] != 0 && board[i][j] != 3 &&
                    board[i][j] == board[i + 1][j - 1] && board[i + 1][j - 1] == board[i + 2][j - 2])
                    return true;
            }
        }
        return false;
    }

    bool is_draw() override {
        return n_moves == 9 && !is_win(); // 7 active cells available
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

template <typename T>
class HumanPlayer : public Player<T> {
public:
    HumanPlayer(string name, T symbol) : Player<T>(name, symbol) {}

    void getmove(int& x, int& y) override {
        cout << this->name << " (" << this->symbol << ") - Enter row (1-3) and column (1-5): ";
        cin >> x >> y;
        x--; // Convert to 0-based indexing
        y--;
    }
};

template <typename T>
class P_X_O_Random_Player : public RandomPlayer<T>{
public:
    P_X_O_Random_Player (T symbol) : RandomPlayer<T>(symbol){
        srand(static_cast<unsigned int>(time(0)));
    };
    void getmove(int &x, int &y){
        x = rand() % 3;  // Random number between 0 and 2
        y = rand() % 5;  // Random number between 0 and 4
    } ;
};

void Game1(){
    int choice;
    Player<int>* players[2];
    string playerXName, player2Name;
    // Create board and players
    Game3x5Board* board = new Game3x5Board();

    cout << "\nWelcome to FCAI Pyramid X-O Game. :)\n";

    // Set up player 1
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;

    switch(choice) {
        case 1:
            cout << "Enter Player X name: ";
            cin >> playerXName;
            players[0] = new HumanPlayer<int>(playerXName, 1);
            break;
        case 2:
            players[0] = new P_X_O_Random_Player<int>(1);
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
            players[1] = new HumanPlayer<int>(player2Name, 2);
            break;
        case 2:
            players[1] = new P_X_O_Random_Player<int>(2);
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

#endif //C___PROJECTS_GAME1_H
