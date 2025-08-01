//
// Created by Dopamine on 12/6/2024.
//

#ifndef C___PROJECTS_GAME3_H
#define C___PROJECTS_GAME3_H

#include "BoardGame_Classes.h"
using namespace std;

bool taken [5][5] = {false};
bool ghostTurn = false;

template <typename T>
class humanPlayer : public Player<T> {
public:
    humanPlayer(string name, T symbol) : Player<T>(name, symbol) {}

    void getmove(int& x, int& y) override {
        if (ghostTurn) return;
        cout << this->name << " (" << this->symbol << ") - Enter row and column (1-5): ";
        cin >> x >> y;
        x--; // Convert to 0-based indexing
        y--;
    }

};

template<typename T>
class FiveRandomPlayer : public RandomPlayer<T> {
public:
    FiveRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
        this->dimension = 5;
        this->name = "Random Computer Player";
        srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
    }

    void getmove(int& x, int& y) override {
        x = rand() % this->dimension;  // Random number between 0 and 2
        y = rand() % this->dimension;
        while (taken[x][y]) {
            x = rand() % this->dimension;  // Random number between 0 and 2
            y = rand() % this->dimension;
        }
    }
};


class FiveFiveBoard : public Board<int> {
protected:
    bool chechedBoard[5][5] = {false}; // 5x5 board initialized with 0
    int p1Score = 0;
    int p2Score = 0;
    int x = 0, o = 0;
public:
    FiveFiveBoard() {
        rows = 5;
        columns = 5;
        board = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new int[columns]{0}; // Initialize with 0s
        }

    }

    ~FiveFiveBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;


    }

    bool update_board(int x, int y, int symbol) override {
        if (ghostTurn) return true;
        if (cin.fail() || x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0) {
            cout << "Invalid move. Try again.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            return false;
        }
        board[x][y] = symbol;
        taken[x][y] = true;
        n_moves++;
        return true;
    }

    void display_board() override {
        const string YELLOW = "\033[1;33m";
        const string RED = "\033[1;31m";
        const string RESET = "\033[0m";

        cout << endl;

        cout << "    1    2    3    4    5" << endl; // Column headers
        for (int i = 0; i < 5; i++) {
            cout << i + 1; // Row header
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == 0) {
                    cout << " |   "; // Empty cell
                } else if (board[i][j] == 1) {
                    if (chechedBoard[i][j] == true) {
                        cout << " | " << YELLOW << "X" << RESET << " "; // Player 1's counted cell in yellow
                    } else {
                        cout << " | X "; // Player 1's normal cell
                    }
                } else if (board[i][j] == 2) {
                    if (chechedBoard[i][j] == true) {
                        cout << " | " << RED << "O" << RESET << " "; // Player 2's counted cell in red
                    } else {
                        cout << " | O "; // Player 2's normal cell
                    }
                }
            }
            cout << " |" << endl;
            cout << "  -----+----+-----+-----+---" << endl; // Row separator
        }
    }


    bool is_win() override {

        if (n_moves < 24) return false;

        // Horizontal and vertical checks
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                // Horizontal check
                if (j <= 2 && board[i][j] != 0 &&
                    board[i][j] == board[i][j + 1] &&
                    board[i][j + 1] == board[i][j + 2]) {

                    if (x == 0)
                        x = this->board[i][j];
                    if (this->board[i][j] == x) {
                        p1Score++;
                        cout << "Player " << x << " +1 (H): ";
                    } else {
                        if (o == 0)
                            o = this->board[i][j];
                        p2Score++;
                        cout << "Player " << o << " +1 (H): ";
                    }
                    cout << "(" << i + 1 << ", " << j + 1 << ") "
                         << "(" << i + 1 << ", " << j + 2 << ") "
                         << "(" << i + 1 << ", " << j + 3 << ")\n";
                }

                // Vertical check
                if (i <= 2 && board[i][j] != 0 &&
                    board[i][j] == board[i + 1][j] &&
                    board[i + 1][j] == board[i + 2][j]) {

                    if (x == 0)
                        x = this->board[i][j];
                    if (this->board[i][j] == x) {
                        p1Score++;
                        cout << "Player " << x << " +1 (V): ";
                    } else {
                        if (o == 0)
                            o = this->board[i][j];
                        p2Score++;
                        cout << "Player " << o << " +1 (V): ";
                    }
                    cout << "(" << i + 1 << ", " << j + 1 << ") "
                         << "(" << i + 2 << ", " << j + 1  << ") "
                         << "(" << i + 3 << ", " << j + 1 << ")\n";
                }

                // Diagonal checks (top-left to bottom-right)
                if (i <= 2 && j <= 2 && board[i][j] != 0 &&
                    board[i][j] == board[i + 1][j + 1] &&
                    board[i + 1][j + 1] == board[i + 2][j + 2]) {

                    if (x == 0)
                        x = this->board[i][j];
                    if (this->board[i][j] == x) {
                        p1Score++;
                        cout << "Player " << x << " +1 (D): ";
                    } else {
                        if (o == 0)
                            o = this->board[i][j];
                        p2Score++;
                        cout << "Player " << o << " +1 (D): ";
                    }
                    cout << "(" << i + 1 << ", " << j + 1 << ") "
                         << "(" << i + 2 << ", " << j + 2 << ") "
                         << "(" << i + 3 << ", " << j + 3 << ")\n";
                }

                // Diagonal checks (bottom-left to top-right)
                if (i >= 2 && j <= 2 && board[i][j] != 0 &&
                    board[i][j] == board[i - 1][j + 1] &&
                    board[i - 1][j + 1] == board[i - 2][j + 2]) {


                    if (x == 0)
                        x = this->board[i][j];
                    if (this->board[i][j] == x) {
                        p1Score++;
                        cout << "Player " << x << " +1 (~D): ";
                    } else {
                        if (o == 0)
                            o = this->board[i][j];
                        p2Score++;
                        cout << "Player " << o << " +1 (~D): ";
                    }
                    cout << "(" << i + 1 << ", " << j + 1 << ") "
                         << "(" << i << ", " << j + 2 << ") "
                         << "(" << i - 1 << ", " << j + 3 << ")\n";
                }
            }
        }

        return false;
    }


    bool is_draw() override {
        return false;
    }

    bool game_is_over() override {
        if (n_moves == 24) {
            cout << "Player " << x << " Score: " << p1Score << "\n"
                 << "Player " << o << " Score: " << p2Score << "\n";
            if (p1Score > p2Score) {
                cout << "Player " << x << " wins!";
            }
            else if (p2Score > p1Score) {
                cout << "Player " << o << " wins!";
            }
            else {
                cout << "Draw!";
            }
            return true;
        }
        return false;
    }
};


void Game3(){
    int choice;
    Player<int>* players[2];
    string playerXName, player2Name;
    // Create board and players
    FiveFiveBoard* board = new FiveFiveBoard();

    cout << "\nWelcome to FCAI 5x5 X-O Game. :)\n";

    // Set up player 1
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;



    switch(choice) {
        case 1:
            cout << "Enter Player X name: ";
            cin >> playerXName;
            players[0] = new humanPlayer<int>(playerXName, 1);
            break;
        case 2:
            players[0] = new FiveRandomPlayer<int>(1);
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
            players[1] = new humanPlayer<int>(player2Name, 2);
            break;
        case 2:
            players[1] = new FiveRandomPlayer<int>(2);
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

#endif //C___PROJECTS_GAME3_H
