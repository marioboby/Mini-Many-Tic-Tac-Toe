#include "Game1.h"
#include "Game2.h"
#include "Game3.h"
#include "Game4.h"
#include "Game5.h"
#include "Game6.h"
#include "Game8.h"
#include "Game9.h"

using namespace std;


void displayMenu() {
    cout << "\n=== Games Menu ===\n";
    cout << "1. Pyramid Tic-Tac-Toe\n";
    cout << "2. Four-in-a-row\n";
    cout << "3. 5 x 5 Tic Tac Toe\n";
    cout << "4. Word Tic-Tac-Toe.\n";
    cout << "5. Numerical Tic-Tac-Toe\n";
    cout << "6. Misere Tic-Tac-Toe\n";
    cout << "7. Ultimate Tic-Tac-Toc\n";
    cout << "8. SUS\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {

    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "You selected Pyramid Tic-Tac-Toe.\n";
                Game1();
                break;
            case 2:
                cout << "You selected Four-in-a-row.\n";
                Game2();
                break;
            case 3:
                cout << "You selected 5 x 5 Tic Tac Toe.\n";
                Game3();
                break;
            case 4:
                cout << "You selected Word Tic-Tac-Toe\n";
                Game4();
                break;
            case 5:
                cout << "You selected Numerical Tic-Tac-Toe.\n";
                Game5();
                break;
            case 6:
                cout << "You selected Misere Tic-Tac-Toe.\n";
                Game6();
                break;
            case 7:
                cout << "You selected Ultimate Tic-Tac-Toc.\n";
                Game8();
                break;
            case 8:
                cout << "You selected SUS.\n";
                Game9();
                break;
            case 9:
                cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}