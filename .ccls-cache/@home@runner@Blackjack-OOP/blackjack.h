#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

class Card {
public:
    enum Rank {
        TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK, QUEEN, KING, ACE,
        MAX_RANKS
    };
    enum Suit {
        CLUBS, DIAMONDS, HEARTS, SPADES,
        MAX_SUITS
    };

    Card(Rank rank = TWO, Suit suit = CLUBS);
    int value() const;
    void print() const;

private:
    Rank m_rank;
    Suit m_suit;
};

class Deck {
public:
    Deck();
    void shuffle();
    Card deal();
private:
    std::vector<Card> m_deck;
    int m_cardIndex;
};

class Hand {
public:
    void addCard(const Card& card);
    int total() const;
    void clear();
    size_t count() const;
    void print() const;
    Card getCard(size_t index) const;

private:
    std::vector<Card> m_cards;
};

class Player {
public:
    Player(const std::string& name, int chips = 0);
    void print() const;
    void addCard(const Card& card);
    int total() const;
    bool isBusted() const;
    void clear();
    void changeChips(int amount);
    int getChips() const;
    Card getCard(size_t index) const;
    const Hand& getHand() const;

private:
    std::string m_name;
    Hand m_hand;
    int m_chips;
};

class BlackjackGame {
public:
    BlackjackGame();
    void play();
    int getPlayerChips() const;
    bool isBlackjack(const Player& player) const;

private:
    void dealInitialCards();
    bool playerTurn(Player& player);
    void dealerTurn();
    int determineWinner();
    int getBet() const;

    Deck m_deck;
    Player m_dealer;
    Player m_player;
};