#include "blackjack.h"
using namespace std;

int main() {
    cout << "Welcome to the Blackjack game!" <<  endl;

    BlackjackGame game;
    char playAgain = 'y';

    while ((playAgain == 'y' || playAgain == 'Y') && game.getPlayerChips() > 0) {
        game.play();

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    }

    cout << "Thanks for playing!" <<  endl;
    return 0;
}