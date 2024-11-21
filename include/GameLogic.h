#pragma once

#include <chrono>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <array>
#include "Deck.h"
#include "StockPile.h"
#include "WastePile.h"
#include "FoundationPile.h"
#include "TableauPile.h"


class GameLogic {
private:
    Deck deck;
    StockPile stockPile;
    WastePile wastePile;
    std::array<FoundationPile, 4> foundationPiles;
    std::array<TableauPile, 7> tableauPiles;
    int moveCount;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    bool isValidTableauMove(const Card& card, const Card& target) {
        // Different color (red/black) and one rank lower
        bool isDifferentColor = ((card.getSuit() == Card::Heart || card.getSuit() == Card::Diamond) !=
                               (target.getSuit() == Card::Heart || target.getSuit() == Card::Diamond));
        return isDifferentColor && (card.getRank() == target.getRank() - 1);
    }

    bool isValidTableauSequence(const std::vector<Card>& cards) {
        for (size_t i = 0; i < cards.size() - 1; i++) {
            if (!isValidTableauMove(cards[i+1], cards[i])) {
                return false;
            }
        }
        return true;
    }

    bool moveMultipleCards(int sourceIndex, int destIndex, int cardCount) {
        try {
            if (cardCount > tableauPiles[sourceIndex].getSize()) {
                return false;
            }

            std::vector<Card> cardsToMove = tableauPiles[sourceIndex].peekCards(cardCount);

            if (!isValidTableauSequence(cardsToMove)) {
                return false;
            }

            if (!tableauPiles[destIndex].isEmpty()) {
                if (!isValidTableauMove(cardsToMove[0], tableauPiles[destIndex].topCard())) {
                    return false;
                }
            } else if (cardsToMove[0].getRank() != Card::King) {
                return false;
            }

            cardsToMove = tableauPiles[sourceIndex].removeCards(cardCount);
            tableauPiles[destIndex].addCards(cardsToMove);
            moveCount++;
            return true;
        }
        catch (const std::exception&) {
            std::cerr << "Invalid move" << std::endl;
            return false;
        }
    }

public:
    GameLogic() : foundationPiles{
        FoundationPile(),
        FoundationPile(),
        FoundationPile(),
        FoundationPile()
    }, moveCount(0) {
        initializeGame();
    }

    void initializeGame() {
        deck.initializeDeck();
        deck.shuffleDeck();

        // Initialize 7 Tableaus
        for (int i = 0; i < 7; i++) {
            for (int j = i; j < 7; j++) {
                Card card = deck.dealCard();
                // Flip Up the top card of each tableau
                if (i == j) card.flip();
                tableauPiles[j].addCard(card);
            }
        }

        // Move remaining deck cards to stockPile
        while(!deck.isEmpty()) {
            stockPile.addCard(deck.dealCard());
        }

        // Start the clock
        startTime = std::chrono::steady_clock::now();
    }

    void drawCard() {
        if (!stockPile.isEmpty()) {
            Card card = stockPile.removeCard();
            card.flip();  // Ensure the card is face up
            wastePile.addCard(card);
            moveCount++;
        } else if (!wastePile.isEmpty()) {
            while (!wastePile.isEmpty()) {
                Card card = wastePile.removeCard();
                card.flip();  // Ensure the card is face down
                stockPile.addCard(card);
            }
        } else {
            std::cerr << "No cards left to draw!" << std::endl;
        }
    }

    // indexes => StockPile: 0, WastePile: 1, Tableaus: 2 to 8, Foundations: 9 to 12
    bool moveCard(int sourcePile, int destinationPile, int cardCount = 1) {
        try {
            // Stock to waste (draw)
            if (sourcePile == 0 && destinationPile == 1) {
                drawCard();
                return true;
            }

            // Tableau to tableau (Mutiple cards)
            if (sourcePile >= 2 && sourcePile <= 8 && 
                destinationPile >= 2 && destinationPile <= 8 && 
                cardCount > 1) {
                return moveMultipleCards(sourcePile - 2, destinationPile - 2, cardCount);
            }

            // Waste to tableau
            if (sourcePile == 1 && destinationPile >= 2 && destinationPile <= 8) {
                if (wastePile.isEmpty()) return false;
                Card card = wastePile.topCard();
                int tableauIndex = destinationPile - 2;
                
                if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[tableauIndex].addCard(wastePile.removeCard());
                    moveCount++;
                    return true;
                }
                
                if (!tableauPiles[tableauIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
                    tableauPiles[tableauIndex].addCard(wastePile.removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Waste to foundation
            if (sourcePile == 1 && destinationPile >= 9 && destinationPile <= 12) {
                if (wastePile.isEmpty()) return false;
                Card card = wastePile.topCard();
                if(foundationPiles[destinationPile - 9].addCard(card)) {
                    wastePile.removeCard();
                    moveCount++;
                    return true;
                } else {
                    return false;
                }
            }

            // Foundation to tableau
            if (sourcePile >= 9 && sourcePile <= 12 && destinationPile >= 2 && destinationPile <= 8) {
                int foundationIndex = sourcePile - 9;
                if (foundationPiles[foundationIndex].isEmpty()) return false;
                Card card = foundationPiles[foundationIndex].topCard();
                int tableauIndex = destinationPile - 2;
                if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
                    moveCount++;
                    return true;
                }
                if (!tableauPiles[tableauIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
                    tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Tableau to tableau
            if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 2 && destinationPile <= 8) {
                int sourceIndex = sourcePile - 2;
                int destIndex = destinationPile - 2;
                
                if (tableauPiles[sourceIndex].isEmpty()) return false;
                Card card = tableauPiles[sourceIndex].topCard();
                
                if (tableauPiles[destIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
                    moveCount++;
                    return true;
                }
                
                if (!tableauPiles[destIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[destIndex].topCard())) {
                    tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Tableau to foundation
            if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 9 && destinationPile <= 12) {
                int tableauIndex = sourcePile - 2;
                if (tableauPiles[tableauIndex].isEmpty()) return false;
                Card card = tableauPiles[tableauIndex].topCard();
                if (foundationPiles[destinationPile - 9].addCard(card)) {
                    tableauPiles[tableauIndex].removeCard();
                    moveCount++;
                    return true;
                } else {
                    return false;
                }
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            return false;
        }
    }
    bool isGameWon() const {
        // Check if all foundation piles have 13 cards
        for (const auto& pile : foundationPiles) {
            if (pile.isEmpty() || pile.getSize() != 13) return false;
        }
        return true;
    }

    void displayGame() const {
        setCursorPosition(0, 3);
        std::cout << "Stock Pile =>";
        stockPile.displayTopCard();
        setCursorPosition(30, 3);
        std::cout << "<= Waste Pile\n";
        wastePile.displayTopCard();
    
        setCursorPosition(65, 0);
        std::cout << "Foundations" << std::endl;
        for (int i = 0; i < foundationPiles.size(); ++i) {
            foundationPiles[i].displayTopCard(50+(10*i),1);
            std::cout << "\n";
        }

        setCursorPosition(30, 8);
        std::cout << "Tableaus" << std::endl;
        int currentX = -7;
        int currentY = 9;
        for (int i = 0; i < tableauPiles.size(); ++i) {
            setCursorPosition(currentX+=10, currentY);
            std::cout << i + 1;
            tableauPiles[i].displayTopCard(10*i,10);
        }

        int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count();
        setCursorPosition(95, 2);
        std::cout << "Move Count: " << moveCount;
        setCursorPosition(95, 4);
        std::cout << "Time: " << elapsedTime << " seconds";
    }

    std::string displayHint() {
        std::vector<std::string> hints;
        try {
            if (!getWastePile().isEmpty()) {
                const Card wasteCard = getWastePile().topCard();
                for (int i = 0; i < 4; ++i) {
                    if (foundationPiles[i].isValidMove(wasteCard)) {
                        hints.push_back("Hint: Move card from Waste to Foundation " + std::to_string(i + 1));
                    }
                }
            }

            if (!getWastePile().isEmpty()) {
                const Card wasteCard = getWastePile().topCard();
                for (int i = 0; i < 7; ++i) {
                    const Card destCard = tableauPiles[i].topCard();
                    if (isValidTableauMove(wasteCard, destCard)) {
                        hints.push_back("Hint: Move card from Waste to Tableau " + std::to_string(i + 1));
                    }
                }
            }

            for (int i = 0; i < 7; ++i) {
                if (!tableauPiles[i].isEmpty()) {
                    const Card sourceCard = tableauPiles[i].topCard();
                    for (int j = 0; j < 7; ++j) {
                        const Card destCard = tableauPiles[j].topCard();
                        if (isValidTableauMove(sourceCard, destCard)) {
                            hints.push_back("Hint: Move card from Tableau " + std::to_string(i + 1) + " to Tableau " + std::to_string(j + 1));
                        }
                    }
                }
            }

            if (!getStockPile().isEmpty()) {
                hints.push_back("Hint: Move card from Stock to Waste");
            }

            // return random hint from hint vector
            if (hints.empty()) {
                return "No hints available";
            }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, hints.size() - 1);
            return hints[distr(gen)];


        } catch (const std::exception& e) { 
            return "Hint: Move card from Stock to Waste";
        }
    }

    const StockPile& getStockPile() const { return stockPile; }

    const WastePile& getWastePile() const { return wastePile; }
};