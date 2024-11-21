#pragma once

#include "Pile.h"
#include "Stack.h"
#include "Card.h"

class WastePile: Pile {
    private:
        Stack cards;

    public: 
        bool addCard(const Card& card) override {
            Card newCard = card;
            if (!newCard.isFaceUp()) {
                newCard.flip();
            }
            cards.push(newCard);
            return true;
        }

        Card removeCard() override {
            if (isEmpty()) std::cerr << "Waste Pile is Empty!" << std::endl;
            else {
                Card topCard = cards.pop();
                return topCard;
            }
        }

        Card topCard() const override {
            if (isEmpty()) std::cerr << "Waste Pile is Empty!" << std::endl;
            else return cards.peek();
        }

        bool isEmpty() const override {
            return cards.isEmpty();
        }

        int getSize() const {
            return cards.getSize();
        }

        void displayTopCard() const {
            if (cards.isEmpty()) {
                PrintCardBox(Card::Suit::Club, Card::Rank::Flipped, 22, 1);
            }
            else {
                PrintCardBox(cards.peek().getSuit(), cards.peek().getRank(),22,1);
            }
        }
};