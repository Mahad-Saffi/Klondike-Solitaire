#pragma once

#include <vector>
#include "Card.h"
#include <algorithm>
#include <random>
#include <chrono>

class Deck {
    public:
        Deck() {}
        ~Deck() = default;

        bool initializeDeck() {
            resetDeck();
            for (Card::Suit i = Card::Suit::Heart; i <= Card::Suit::Spade; i = static_cast<Card::Suit>(static_cast<int>(i) + 1)) {
                for (Card::Rank j = Card::Rank::Ace; j <= Card::Rank::King; j = static_cast<Card::Rank>(static_cast<int>(j) + 1)) {
                    Card card(i, j);  
                    cards.push_back(card); 
                }
            }
            return true;
        }

        bool shuffleDeck() {
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
            return true;
        }

        Card dealCard() {
            Card card = cards.back();
            cards.pop_back();
            return card;
        }

        bool isEmpty() {
            return cards.empty();
        }

        void resetDeck() {
            cards.clear();
        }

    private: 
        std::vector<Card> cards;
};