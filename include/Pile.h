#pragma once

#include "Card.h"
// This is abstract class for Tableau, Foundation, Stock and Waste Piles
class Pile {
    public:
        virtual bool addCard(const Card& card) = 0;
        virtual Card removeCard() = 0;
        virtual Card topCard() const = 0;
        virtual bool isEmpty() const = 0;
        virtual ~Pile() = default;
};