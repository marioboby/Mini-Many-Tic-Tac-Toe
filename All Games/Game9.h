//
// Created by Dopamine on 12/9/2024.
//

#ifndef C___PROJECTS_GAME9_H
#define C___PROJECTS_GAME9_H

#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;
bool SghostTurn = false;
bool Staken [3][3] = {false};

// TicTacToeBoard class to represent the game board
class SUSBoard : public Board<char> {

    bool checkedBoard [9] = {false};
    string sus = "SUS";
    bool turn = false;
    int p1Score = 0 , p2Score = 0;

public:
    // Constructor
    SUSBoard() {
        rows = 3;
        columns = 3;
        n_moves = 0;

        // Initialize the board with empty spaces
        board = new char*[rows];
        for (int i = 0; i < rows; i++) {
            board[i] = new char[columns];
            for (int j = 0; j < columns; j++) {
                board[i][j] = ' ';
            }
        }
    }

    // Destructor
    ~SUSBoard() {
        for (int i = 0; i < rows; i++) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Update the board with the player's symbol
    bool update_board(int x, int y, char symbol) override {
        if (SghostTurn) return true;
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != ' ') {
            cout << "Invalid move. Try again.\n";
            cin.clear();
            cin.ignore(INT_MAX , '\n');
            return false;
        }

        if (toupper(symbol) != 'S' && toupper(symbol) != 'U') {
            cout << "Invalid symbol. Try again.\n";
            cin.clear();
            cin.ignore(INT_MAX , '\n');
            return false;
        }

        board[x][y] = symbol;
        Staken[x][y] = true;
        n_moves++;
        turn = (turn != true);
        return true;
    }

    // Display the current state of the board
    void display_board() override {
        if (SghostTurn) return;
        cout << "    1   2   3" << endl;
        for (int i = 0; i < rows; i++) {
            cout << i + 1;
            for (int j = 0; j < columns; j++) {
                cout << " | " << board[i][j];
            }
            cout << " |" << endl;
            if (i < rows - 1) {
                cout << "  ---+---+---" << endl;
            }
        }
        cout << endl;
    }

    // Check if there is a winner
    bool is_win() override {
        if (SghostTurn) return true;
        bool found = false;

        map<int, bool> foundWords;
        vector<string> possibleWords = {
                string(1, board[0][0]) + board[0][1] + board[0][2],
                string(1, board[1][0]) + board[1][1] + board[1][2],
                string(1, board[2][0]) + board[2][1] + board[2][2],
                string(1, board[0][0]) + board[1][0] + board[2][0],
                string(1, board[0][1]) + board[1][1] + board[2][1],
                string(1, board[0][2]) + board[1][2] + board[2][2],
                string(1, board[0][0]) + board[1][1] + board[2][2],
                string(1, board[0][2]) + board[1][1] + board[2][0]
        };

        for (int i = 0; i < possibleWords.size(); i++) {
            if (possibleWords[i] == sus && !checkedBoard[i]) {
                checkedBoard[i] = true;
                if (turn) {
                    p1Score++;
                }
                else p2Score++;
                found = true;
            }
        }

        if (found) {
            cout << "Scores- Player 1 -> " << p1Score << " -- Player 2 -> " << p2Score << endl;
        }

        if (n_moves == 9) {
            if (p1Score != p2Score) {
                if (p1Score > p2Score) return true;
                SghostTurn = true;
                return false;
            }
        }
        return false;
    }

    // Check if the game is a draw
    bool is_draw() override {
        return n_moves == rows * columns && !is_win();
    }

    // Check if the game is over
    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

// HumanPlayer class to represent human interaction
class SHumanPlayer : public Player<char> {
public:
    SHumanPlayer(string name, char symbol) : Player<char>(name, symbol) {}

    void getmove(int& x, int& y) override {
        if (SghostTurn) return;
        cout << name << " (" << symbol << "), enter row and column (1-3): ";
        cin >> x >> y;
        x--; // Adjust to 0-based index
        y--;
    }
};


// Random Computer Player
class SUSRandomPlayer : public Player<char> {
public:
    SUSRandomPlayer(char symbol) : Player<char>("Random Computer", symbol) {}

    void getmove(int& x, int& y) override {
        // Improved random move generation
        vector<pair<int, int>> availableMoves;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (!Staken[i][j]) {
                    availableMoves.push_back({i, j});
                }
            }
        }

        if (!availableMoves.empty()) {
            int randomIndex = rand() % availableMoves.size();
            x = availableMoves[randomIndex].first;
            y = availableMoves[randomIndex].second;
        }
    }
};

void Game9(){
    srand(static_cast<unsigned>(time(0))); // Seed RNG

    Player<char>* players[2];
    SUSBoard* board = new SUSBoard();

    cout << "\nWelcome to FCAI SUS X-O Game!\n";

    // Setup Player 1
    cout << "Choose Player 1 type:\n1. Human\n2. Random Computer\n";
    int choice;
    cin >> choice;

    char p1Symbol;
    if (choice == 1) {
        string name;
        cout << "Enter Player 1 name: ";
        cin >> name;

        // Symbol selection
        cout << "Player 1, choose your symbol (S or U): ";
        cin >> p1Symbol;
        p1Symbol = toupper(p1Symbol);
        while (p1Symbol != 'S' && p1Symbol != 'U') {
            cout << "Invalid choice. Please choose 'S' or 'U': ";
            cin >> p1Symbol;
            p1Symbol = toupper(p1Symbol);
        }

        players[0] = new SHumanPlayer(name, p1Symbol);
    } else {
        // Random player
        p1Symbol = (rand() % 2 == 0) ? 'S' : 'U';
        players[0] = new SUSRandomPlayer(p1Symbol);
        cout << "Player 1 (Random Computer) will play as " << p1Symbol << ".\n";
    }

    // Setup Player 2
    cout << "Choose Player 2 type:\n1. Human\n2. Random Computer\n";
    cin >> choice;

    char p2Symbol = (p1Symbol == 'S') ? 'U' : 'S';
    if (choice == 1) {
        string name;
        cout << "Enter Player 2 name: ";
        cin >> name;

        cout << "Player 2 will play as " << p2Symbol << ".\n";
        players[1] = new SHumanPlayer(name, p2Symbol);
    } else {
        players[1] = new SUSRandomPlayer(p2Symbol);
        cout << "Player 2 (Random Computer) will play as " << p2Symbol << ".\n";
    }

    // GameManager
    GameManager<char> gameManager(board, players);
    gameManager.run();

    delete board;
    delete players[0];
    delete players[1];
}

#endif //C___PROJECTS_GAME9_H
