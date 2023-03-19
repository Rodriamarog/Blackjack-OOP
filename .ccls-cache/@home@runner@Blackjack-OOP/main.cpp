#include <iostream>
#include <limits>
#include "blackjack.h"

int main() {
    std::cout << "Welcome to the Blackjack game!" << std::endl;

    BlackjackGame game;
    char playAgain = 'y';

    while ((playAgain == 'y' || playAgain == 'Y') && game.getPlayerChips() > 0) {
        game.play();

        do {
            std::cout << "Do you want to play again? (y/n): ";
            std::cin >> playAgain;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' && playAgain != 'N');
    }

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}