#include "blackjack.h"

Card::Card(Rank rank, Suit suit) : m_rank(rank), m_suit(suit) {}

int Card::value() const {
  if (m_rank == KING || m_rank == QUEEN || m_rank == JACK) {
    return 10;
  } else if (m_rank == ACE) {
    return 11;
  } else {
    return m_rank + 2;
  }
}

void Card::print() const {
  const char *rankStrings[] = {"2", "3",  "4", "5", "6", "7", "8",
                               "9", "10", "J", "Q", "K", "A"};
  const char *suitStrings[] = {"C", "D", "H", "S"};

  std::cout << rankStrings[m_rank] << suitStrings[m_suit];
}

Deck::Deck() : m_cardIndex(0) {
  for (int suit = 0; suit < Card::MAX_SUITS; ++suit) {
    for (int rank = 0; rank < Card::MAX_RANKS; ++rank) {
      m_deck.push_back(
          Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit)));
    }
  }
}

void Deck::shuffle() {
  std::srand(static_cast<unsigned int>(std::time(0)));
  std::random_shuffle(m_deck.begin(), m_deck.end());
  m_cardIndex = 0;
}

Card Deck::deal() {
  if (m_cardIndex < m_deck.size()) {
    return m_deck[m_cardIndex++];
  } else {
    return Card(Card::ACE,
                Card::SPADES); // If the deck is empty, return a default card
  }
}

void Hand::addCard(const Card &card) { m_cards.push_back(card); }

int Hand::total() const {
  int sum = 0;
  bool hasAce = false;

  for (const Card &card : m_cards) {
    sum += card.value();
    if (card.value() == 11) {
      hasAce = true;
    }
  }

  if (hasAce && sum > 21) {
    sum -= 10;
  }

  return sum;
}

size_t Hand::count() const {
    return m_cards.size();
}

void Hand::clear() { m_cards.clear(); }

void Hand::print() const {
  for (const Card &card : m_cards) {
    card.print();
    std::cout << " ";
  }
}

Card Hand::getCard(size_t index) const {
    if (index < m_cards.size()) {
        return m_cards[index];
    } else {
        return Card();
    }
}

Player::Player(const std::string &name, int chips)
    : m_name(name), m_chips(chips) {}

void Player::changeChips(int amount) { m_chips += amount; }

int Player::getChips() const { return m_chips; }

void Player::print() const {
  std::cout << m_name << ": ";
  m_hand.print();
  std::cout << " [" << total() << "]";
}

void Player::addCard(const Card &card) { m_hand.addCard(card); }

int Player::total() const { return m_hand.total(); }

bool Player::isBusted() const { return total() > 21; }

void Player::clear() { m_hand.clear(); }

const Hand& Player::getHand() const {
    return m_hand;
}

Card Player::getCard(size_t index) const { return m_hand.getCard(index); }

BlackjackGame::BlackjackGame() : m_dealer("Dealer"), m_player("Player", 1000) {}

void BlackjackGame::play() {
    m_deck.shuffle();
    dealInitialCards();

    int bet = getBet();

    if (isBlackjack(m_player)) {
        std::cout << "Player: ";
        m_player.print();
        std::cout << " - Blackjack!" << std::endl;
        std::cout << "You win!" << std::endl;
        m_player.changeChips(bet);
    } else {
        bool playerBusted = playerTurn(m_player);
        if (!playerBusted) {
            dealerTurn();
        }

        int winner = determineWinner();
        if (winner == 1) {
            m_player.changeChips(bet);
        } else if (winner == -1) {
            m_player.changeChips(-bet);
        }
        // No chip change if it's a tie (winner == 0)
    }

    std::cout << "You now have " << m_player.getChips() << " chips." << std::endl;

    m_player.clear();
    m_dealer.clear();
}

int BlackjackGame::getBet() const {
  int bet;
  while (true) {
    std::cout << std::endl << "You have " << m_player.getChips()
              << " chips. How many do you want to bet? ";
    std::cin >> bet;

    if (bet > 0 && bet <= m_player.getChips()) {
      break;
    } else {
      std::cout << "Invalid bet. Please enter a valid amount." << std::endl;
    }
  }
  return bet;
}

void BlackjackGame::dealInitialCards() {
  m_player.addCard(m_deck.deal());
  m_dealer.addCard(m_deck.deal());
  m_player.addCard(m_deck.deal());
  m_dealer.addCard(m_deck.deal());
}

bool BlackjackGame::isBlackjack(const Player& player) const {
    if (player.total() == 21 && player.getHand().count() == 2) {
        bool hasAce = false;
        bool hasTenValueCard = false;

        for (int i = 0; i < 2; ++i) {
            Card card = player.getHand().getCard(i);
            if (card.value() == 10) {
                hasTenValueCard = true;
            }
            if (card.value() == 11) {
                hasAce = true;
            }
        }

        return hasAce && hasTenValueCard;
    }
    return false;
  }

bool BlackjackGame::playerTurn(Player &player) {
  while (true) {
    std::cout << "Dealer: ";
    m_dealer.getCard(0).print();
    std::cout << " XX" << std::endl;
    player.print();
    std::cout << std::endl;

    if (player.isBusted()) {
      std::cout << player.total() << " - Busted!" << std::endl;
      return true;
    }

    std::cout << "Do you want to hit or stand? (h/s): ";
    char choice;
    std::cin >> choice;

    if (choice == 'h' || choice == 'H') {
      player.addCard(m_deck.deal());
    } else {
      break;
    }
  }
  return false;
}

void BlackjackGame::dealerTurn() {
  while (m_dealer.total() < 17) {
    m_dealer.addCard(m_deck.deal());
  }
}

int BlackjackGame::determineWinner() {
  m_player.print();
  std::cout << std::endl;
  m_dealer.print();
  std::cout << std::endl;

  if (m_player.isBusted()) {
    std::cout << "Dealer wins!" << std::endl;
    return -1;
  } else if (m_dealer.isBusted()) {
    std::cout << "Player wins!" << std::endl;
    return 1;
  } else if (m_player.total() > m_dealer.total()) {
    std::cout << "Player wins!" << std::endl;
    return 1;
  } else if (m_player.total() < m_dealer.total()) {
    std::cout << "Dealer wins!" << std::endl;
    return -1;
  } else {
    std::cout << "It's a tie!" << std::endl;
    return 0;
  }
}

int BlackjackGame::getPlayerChips() const { return m_player.getChips(); }