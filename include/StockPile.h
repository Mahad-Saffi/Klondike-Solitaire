#pragma once

#include "Pile.h"
#include "Stack.h"
#include "Card.h"

class StockPile: Pile {
    private:
        Stack cards;

    public: 
        bool addCard(const Card& card) override {
            Card newCard = card;
            if (newCard.isFaceUp()) {
                newCard.flip();
            }
            cards.push(card);
            return true;
        }

        Card removeCard() override {
            if (isEmpty()) {
                std::cerr << "Stock Pile is empty!";
            } else {
                return cards.pop();
            }
        }

        Card topCard() const override {
            if (isEmpty()) {
                std::cerr << "Stock Pile is empty!";
            } else {
                return cards.peek();
            }
        }

        bool isEmpty() const override {
            return cards.isEmpty();
        }

        int getSize() const {
            return cards.getSize();
        }

        void displayTopCard() const {
            if (!cards.isEmpty()) {
                PrintCardBox(Card::Suit::Club,Card::Rank::Flipped,14,1);
            }
        }
};