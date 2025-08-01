//
// Created by Dopamine on 12/6/2024.
//

#ifndef C___PROJECTS_GAME4_H
#define C___PROJECTS_GAME4_H

#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;

// TicTacToeBoard class to represent the game board
class WordTicTacToeBoard : public Board<char> {
private:
    vector<string> words;

public:
    // Constructor
    WordTicTacToeBoard(const vector<string>& validWords) {
        rows = 3;
        columns = 3;
        n_moves = 0;
        words = validWords;

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
    ~WordTicTacToeBoard() {
        for (int i = 0; i < rows; i++) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Update the board with the player's symbol
    bool update_board(int x, int y, char symbol) override {
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != ' ') {
            cout << "Invalid move. Try again.\n";
            return false;
        }
        board[x][y] = symbol;
        n_moves++;
        return true;
    }

    // Display the current state of the board
    void display_board() override {
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

        for (const string& word : possibleWords) {
            if (find(words.begin(), words.end(), word) != words.end()) {
                return true; // A valid word found
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
class WHumanPlayer : public Player<char> {
public:
    WHumanPlayer(string name) : Player<char>(name, ' ') {}

    void getmove(int& x, int& y) override {
        // Prompt the player to enter their symbol
        cout << name << ", enter your symbol for this turn: ";
        cin >> symbol;
        symbol = toupper(symbol);

        // Prompt the player to enter row and column
        cout << name << ", enter the row and column (1-3): ";
        cin >> x >> y;
        x--; // Adjust to 0-based index
        y--;
    }
};

template <typename T>
class Random_Player : public RandomPlayer<T> {
public:
    Random_Player() : RandomPlayer<T>(' ') {
        srand(static_cast<unsigned int>(time(0)));
    }

    void getmove(int& x, int& y) override {
        // Randomly choose an uppercase English letter for the current move
        this->symbol = 'A' + (rand() % 26);

        // Randomly choose a position on the board
        x = rand() % 3; // Random row (0-2)
        y = rand() % 3; // Random column (0-2)
    }
};




// Function to read words from a file
vector<string> readWordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> words;

    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return words; // Return empty vector
    }

    string word;
    while (file >> word) {
        words.push_back(word);
    }

    file.close();
    return words;
}

void Game4() {
    int choice;
    Player<char>* players[2];
    string player1Name, player2Name;

    // Read valid words from the file
    vector<string> words = readWordsFromFile("test_cases.txt");

    WordTicTacToeBoard board(words);

    cout << "\nWelcome to Word Tic Tac Toe. :)\n";

    // Set up player 1
    cout << "Choose Player 1 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Enter Player 1 name: ";
            cin >> player1Name;
            players[0] = new WHumanPlayer(player1Name);
            break;
        case 2:
            players[0] = new Random_Player<char>();
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

    switch (choice) {
        case 1:
            cout << "Enter Player 2 name: ";
            cin >> player2Name;
            players[1] = new WHumanPlayer(player2Name);
            break;
        case 2:
            players[1] = new Random_Player<char>();
            break;
        default:
            cout << "Invalid choice for Player 2. Exiting the game.\n";
            return;
    }

    // Assign the board to the players
    players[0]->setBoard(&board);
    players[1]->setBoard(&board);

    // Create GameManager
    GameManager<char> gameManager(&board, players);

    // Run the game
    gameManager.run();

    // Clean up
    delete players[0];
    delete players[1];
}




#endif //C___PROJECTS_GAME4_H
