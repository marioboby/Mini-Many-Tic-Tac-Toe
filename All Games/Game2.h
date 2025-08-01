#ifndef _CONNECTFOUR_H
#define _CONNECTFOUR_H

#include "BoardGame_Classes.h"
#include <bits/stdc++.h>


class ConnectFourBoard : public Board<int> {
public:
    ConnectFourBoard();
    ~ConnectFourBoard();
    bool update_board(int x, int y, int symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    bool check_horizontal(int row, int col, int symbol);
    bool check_vertical(int row, int col, int symbol);
    bool check_diagonal(int row, int col, int symbol);
};

class ConnectFourPlayer : public Player<int> {
public:
    ConnectFourPlayer(std::string name, int symbol);
    void getmove(int& x, int& y) override;
};

class ConnectFourRandomPlayer : public RandomPlayer<int> {
public:
    ConnectFourRandomPlayer(int symbol, int dimension = 7);
    void getmove(int& x, int& y) override;
    bool is_valid_move(int col);
};


ConnectFourBoard::ConnectFourBoard() {
    rows = 6;
    columns = 7;
    n_moves = 0;
    board = new int*[rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new int[columns]{0};
    }
}

// Destructor for ConnectFourBoard
ConnectFourBoard::~ConnectFourBoard() {
    for (int i = 0; i < rows; i++) {
        delete[] board[i];
    }
    delete[] board;
}

// Update the board with a move
bool ConnectFourBoard::update_board(int x, int y, int symbol) {
    // Ensure the column is within bounds
    if (y < 0 || y >= columns) {
        return false;
    }

    // Find the lowest empty cell in the selected column (gravity)
    for (int i = rows - 1; i >= 0; i--) {
        if (board[i][y] == 0) { // If the cell is empty
            board[i][y] = symbol; // Place the piece
            n_moves++; // Increment the move counter
            return true; // Placement successful
        }
    }

    return false; // Column is full, no valid move
}



// Display the board
void ConnectFourBoard::display_board() {
    cout << "_______________________________" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == 0)
                cout << "|   ";
            else if (board[i][j] == 1)
                cout << "| x ";
            else
                cout << "| O ";
        }
        cout << "|" << endl;
    }
    cout << "-------------------------------" << endl;
    for (int j = 0; j < columns; j++) {
        cout << "  " << j + 1 << " ";
    }
    cout << endl;
}

// Check if there is a winner
bool ConnectFourBoard::is_win() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] != 0 &&
                (check_horizontal(i, j, board[i][j]) ||
                 check_vertical(i, j, board[i][j]) ||
                 check_diagonal(i, j, board[i][j]))) {
                return true;
            }
        }
    }
    return false;
}

// Check if the board is full (draw)
bool ConnectFourBoard::is_draw() {
    return n_moves == rows * columns;
}

// Check if the game is over
bool ConnectFourBoard::game_is_over() {
    return is_win() || is_draw();
}

// Horizontal check
bool ConnectFourBoard::check_horizontal(int row, int col, int symbol) {
    if (col + 3 >= columns) return false;
    return board[row][col] == symbol && board[row][col + 1] == symbol &&
           board[row][col + 2] == symbol && board[row][col + 3] == symbol;
}

// Vertical check
bool ConnectFourBoard::check_vertical(int row, int col, int symbol) {
    if (row + 3 >= rows) return false;
    return board[row][col] == symbol && board[row + 1][col] == symbol &&
           board[row + 2][col] == symbol && board[row + 3][col] == symbol;
}

// Diagonal check
bool ConnectFourBoard::check_diagonal(int row, int col, int symbol) {
    // Check \ diagonal
    if (row + 3 < rows && col + 3 < columns &&
        board[row][col] == symbol && board[row + 1][col + 1] == symbol &&
        board[row + 2][col + 2] == symbol && board[row + 3][col + 3] == symbol)
        return true;

    // Check / diagonal
    if (row + 3 < rows && col - 3 >= 0 &&
        board[row][col] == symbol && board[row + 1][col - 1] == symbol &&
        board[row + 2][col - 2] == symbol && board[row + 3][col - 3] == symbol)
        return true;

    return false;
}

// Constructor for ConnectFourPlayer
ConnectFourPlayer::ConnectFourPlayer(string name, int symbol)
        : Player(name, symbol) {}

// Get move from the player
void ConnectFourPlayer::getmove(int& x, int& y) {
    cout << this->name << " (" << (this->symbol == 1 ? "x" : "O") << "): Enter column (1-7): ";
    cin >> y;
    while (cin.fail() || y < 1 || y > 7) {
        cout << "Invalid input. Enter column (1-7): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> y;
    }
    x = 0; // Only column matters for Connect Four
    y -= 1;
}

// Constructor for ConnectFourRandomPlayer
ConnectFourRandomPlayer::ConnectFourRandomPlayer(int symbol, int dimension)
        : RandomPlayer<int>(symbol) {
    this->dimension = dimension;
    srand(time(0));
}

// Generate a random move for ConnectFourRandomPlayer
void ConnectFourRandomPlayer::getmove(int& x, int& y) {
    y = (rand() % dimension);


    x = 0;
    cout << "Computer (" << (symbol == 1 ? "x" : "O") << ") chose column: " << y + 1 << endl;
}

void Game2(){
    ConnectFourBoard board;

    // Menu for player to choose opponent
    cout << "\nWelcome to Connect Four!" << endl;
    cout << "Choose an opponent:" << endl;
    cout << "1. Play against another Player" << endl;
    cout << "2. Play against Random Computer Player" << endl;

    int choice;
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 2) {
        cout << "Invalid choice. Please select 1 or 2: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }

    ConnectFourPlayer player1("Player 1", 1);
    Player<int>* player2;

    if (choice == 1) {
        player2 = new ConnectFourPlayer("Player 2", 2);
    } else {
        player2 = new ConnectFourRandomPlayer(2);
    }

    Player<int>* players[2] = {&player1, player2};
    player1.setBoard(&board);
    player2->setBoard(&board);

    GameManager<int> game(&board, players);
    game.run();

    delete player2;
}

#endif //_CONNECTFOUR_H