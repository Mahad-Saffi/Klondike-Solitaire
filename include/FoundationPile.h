#pragma once

#include "Pile.h"
#include "Stack.h"
#include "Card.h"

class FoundationPile: public Pile {
private:
    Stack cards;

public:
    bool addCard(const Card& card) override {
        if (isValidMove(card)) {
            cards.push(card);
            return true;
        } else {
            return false;
        }
    }

    Card removeCard() override {
        return cards.pop();
    }

    Card topCard() const override {
        return cards.peek();
    }

    bool isEmpty() const override {
        return cards.isEmpty();
    }

    int getSize() const {
        return cards.getSize();
    }

    virtual void displayTopCard(int x,int y) const {
        if (!cards.isEmpty()) {
            Card topCard = cards.peek();
            PrintCardBox(topCard.getSuit(), topCard.getRank(),x,y);
        }
        else {
            PrintCardBox(Card::Suit::Club, Card::Rank::Flipped, x, y);
        }
    }

public:
    bool isValidMove(const Card& other) const {
        if(isEmpty() && other.getRank() == Card::Rank::Ace) return true;
        else {
            Card card = cards.peek();
            return (other.getSuit() == card.getSuit() && static_cast<int>(other.getRank()) == static_cast<int>(card.getRank() + 1));
        }
    }
};