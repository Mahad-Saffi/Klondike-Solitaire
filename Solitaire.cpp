#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <array>
#include <string>
#include <memory>
#include <Windows.h>
#include <ctime>

//defaults to red font with white background
void PrintColoredText(std::string text,WORD wAttributes= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_RED | BACKGROUND_INTENSITY) {
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD originalAttributes;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;

    // Print text with a different color
    SetConsoleTextAttribute(hConsole,wAttributes );
    std::cout << text;
    SetConsoleTextAttribute(hConsole, originalAttributes);
}

// Set cursor position
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, coord);
}

class Card {
    public:
        enum Suit {Heart = 1, Diamond, Club, Spade};
        enum Rank {Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Flipped};

        Card(Suit suit, Rank rank) : rank(rank), suit(suit), isFacedUp(false) {}
        Card() : rank(Ace), suit(Heart), isFacedUp(false) {}

        Rank getRank() const { return rank; }
        void setRank(Rank rank) { this->rank = rank; } 
        Suit getSuit() const { return suit; }
        void setSuit(Suit suit) { this->suit = suit; }
        bool isFaceUp() const { return this->isFacedUp; }
        bool flip() { isFacedUp = !isFacedUp; return isFacedUp; }
        size_t getHashValue() { return static_cast<int>(suit) * 13 + static_cast<int>(rank); }
        bool operator==(Card& other) const {
            return this->suit == other.getSuit() && this->rank == other.getRank();
        }
        void PrintSuitSymbol(int index) const {
            switch (index)
            {
            case Card::Heart:
                PrintColoredText("♥");
                break;
            case Card::Diamond:
                PrintColoredText("♦");
                break;
            case Card::Club:
                std::cout << "♣";
                break;
            case Card::Spade:
                std::cout << "♠";
                break;
            default:
                break;
            }
        }

    private:
        Rank rank;
        Suit suit;
        bool isFacedUp;
};

WORD PrintCardBox(Card::Suit index, Card::Rank rank,int x,int y,int StackPosition=0,WORD oldCardColor=0) {
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, coord);
    WORD CardColorCode = rank == Card::Rank::Flipped?BACKGROUND_GREEN|BACKGROUND_INTENSITY:BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    WORD CardTextColorCode = index <= 2 ? BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED : BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    CardTextColorCode = rank == Card::Rank::Flipped ? BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_BLUE : CardTextColorCode;
    std::string symbol;
    std::string rankText;
    if (rank == Card::Rank::Flipped) {
        symbol = "?";
        rankText = "? ";
    }
    else {
        switch (index)
        {
        case Card::Heart:
            symbol = "♥";
            break;
        case Card::Diamond:
            symbol = "♦";
            break;
        case Card::Club:
            symbol = "♣";
            break;
        case Card::Spade:
            symbol = "♠";
            break;
        default:
            break;
        }

        switch (rank)
        {
        case Card::Ace:
            rankText = "A ";
            break;
        case Card::Two:
            rankText = "2 ";
            break;
        case Card::Three:
            rankText = "3 ";
            break;
        case Card::Four:
            rankText = "4 ";
            break;
        case Card::Five:
            rankText = "5 ";
            break;
        case Card::Six:
            rankText = "6 ";
            break;
        case Card::Seven:
            rankText = "7 ";
            break;
        case Card::Eight:
            rankText = "8 ";
            break;
        case Card::Nine:
            rankText = "9 ";
            break;
        case Card::Ten:
            rankText = "10";
            break;
        case Card::Jack:
            rankText = "J ";
            break;
        case Card::Queen:
            rankText = "Q ";
            break;
        case Card::King:
            rankText = "K ";
            break;
        default:
            break;
        }
    }
    if(StackPosition==0|| oldCardColor==0)
        std::cout << " _____ ";
    else {
        
        std::cout << "|"; PrintColoredText("_____", oldCardColor); std::cout << "|";

    }
    coord.Y=coord.Y+1;
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "|"; PrintColoredText(rankText + "  " + symbol, CardTextColorCode); std::cout << "|";
    coord.Y++;
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "|"; PrintColoredText("     ", CardTextColorCode); std::cout << "|";
    coord.Y++;
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "|"; PrintColoredText("     ", CardTextColorCode); std::cout << "|";
    char reverse[] = { rankText[1], rankText[0] ,'\0' };
    rankText = rankText == "10" ? rankText : reverse;
    coord.Y++;
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "|"; PrintColoredText(symbol + "  " + rankText, CardTextColorCode); std::cout << "|";
    coord.Y++;
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << " ‾‾‾‾‾ ";
    return CardColorCode;
}

class Node {
public:
    Card data;
    std::unique_ptr<Node> next;

    Node(const Card& data) : data(data), next(nullptr) {}
};

class LinkedList {
protected:
    std::unique_ptr<Node> head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    virtual ~LinkedList() = default; // Smart pointers handle deletion

    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return size; }
};

class Stack : public LinkedList {
public:
    Stack() = default;

    Stack(const Stack& other) {
        if (other.head) {
            head = std::make_unique<Node>(other.head->data);
            Node* current = head.get();
            Node* otherCurrent = other.head->next.get();
            while (otherCurrent) {
                current->next = std::make_unique<Node>(otherCurrent->data);
                current = current->next.get();
                otherCurrent = otherCurrent->next.get();
            }
        }
        size = other.size;
    }

    void push(const Card& card) {
        auto newNode = std::make_unique<Node>(card);
        newNode->next = std::move(head);
        head = std::move(newNode);
        size++;
    }

    Card pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is Empty");
        }
        Card card = head->data; // Make a copy of the data
        head = std::move(head->next); // Move ownership to the next node
        size--;
        return card;
    }

    Card peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is Empty");
        }
        return head->data; // Return a copy of the data
    }

    using LinkedList::isEmpty;
    using LinkedList::getSize;
};

class Queue : protected LinkedList {
private:
    Node* tail;

public:
    Queue() : tail(nullptr) {}

    void enqueue(const Card& card) {
        auto newNode = std::make_unique<Node>(card);
        Node* newTail = newNode.get(); // Raw pointer to the new node
        if (isEmpty()) {
            head = std::move(newNode);
        } else {
            tail->next = std::move(newNode);
        }
        tail = newTail;
        size++;
    }

    Card dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is Empty");
        }
        Card card = head->data; // Copy the data
        head = std::move(head->next); // Move ownership to the next node
        if (!head) {
            tail = nullptr;
        }
        size--;
        return card;
    }

    Card peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is Empty");
        }
        return head->data; // Return a copy of the data
    }

    using LinkedList::isEmpty;
    using LinkedList::getSize;
};


// This is abstract class for Tableau, Foundation, Stock and Waste Piles
class Pile {
    public:
        virtual bool addCard(const Card& card) = 0;
        virtual Card removeCard() = 0;
        virtual Card topCard() const = 0;
        virtual bool isEmpty() const = 0;
        virtual ~Pile() = default;
};

class TableauPile : public Pile {
private:
    Stack cards;

public:
    bool addCard(const Card& card) override {
        cards.push(card);
        return true;
    }

    Card removeCard() override {
        Card cardToReturn = cards.pop();
        if (!cards.isEmpty()) {
            if (!cards.peek().isFaceUp()) {
                Card temp = cards.pop();
                temp.flip();
                cards.push(temp);
            }
        }
        return cardToReturn;
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

    std::vector<Card> removeCards(int count) {
        if (count > cards.getSize()) {
            throw std::runtime_error("Not enough cards");
        }

        std::vector<Card> removedCards;
        Stack tempStack;
        // Remove cards and store them
        for (int i = 0; i < count; i++) {
            Card card = cards.pop();
            removedCards.push_back(card);
            tempStack.push(card);
        }

        // Flip the next card in the tableau
        if (!cards.isEmpty()) {
            if (!cards.peek().isFaceUp()) {
                Card temp = cards.pop();
                temp.flip();
                cards.push(temp);
            }
        }

        // Reverse order for proper sequence
        std::reverse(removedCards.begin(), removedCards.end());
        return removedCards;
    }

    void addCards(const std::vector<Card>& newCards) {
        for (const Card& card : newCards) {
            addCard(card);
        }
    }

    std::vector<Card> peekCards(int count) const {
        if (count > cards.getSize()) {
            throw std::runtime_error("Not enough cards");
        }

        std::vector<Card> visibleCards;
        Stack tempStack = cards;
        for (int i = 0; i < count; i++) {
            visibleCards.push_back(tempStack.pop());
        }
        std::reverse(visibleCards.begin(), visibleCards.end());
        return visibleCards;
    }

    void displayTopCard(int x,int y) const {
        if (!cards.isEmpty()) {
            Stack tempStack = cards;
            std::vector<Card> cardList;

            while (!tempStack.isEmpty()) {
                cardList.push_back(tempStack.pop());
            }

            bool foundFaceUp = false;

            // Display the cards in the tableau, handling face-down cards   
            const int originalSize = cardList.size();
            WORD oldColor=0;
            for (size_t i = cardList.size(); i > 0; --i) {
                if (cardList[i - 1].isFaceUp()) {
                oldColor = PrintCardBox(cardList[i - 1].getSuit(), cardList[i - 1].getRank(),x,y+((originalSize-i)*2), originalSize - i,oldColor);
                    foundFaceUp = true;
                } else {
                oldColor = PrintCardBox(Card::Suit::Club, Card::Rank::Flipped, x, y + ((originalSize - i)*2 ),originalSize-i,oldColor);
                }
            }
        }
    }
};

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
                PrintCardBox(Card::Suit::Club,Card::Rank::Flipped,22,1);
            }
            else {
                PrintCardBox(cards.peek().getSuit(), cards.peek().getRank(),22,1);
            }
        }
};

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


class Solitaire {
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
    Solitaire() : foundationPiles{
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

void fullsc()
{
    HWND Hwnd = GetForegroundWindow();
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
    SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(Hwnd,HWND_TOP,0,0,x,y,0);
}

int main() {
    Solitaire game;
    int source, destination, cardCount;

    // fullsc();
    SetConsoleOutputCP(CP_UTF8);

    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the current console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        // Set the new buffer size (set Y to 1500 for a scrollable buffer)
        COORD newSize;
        newSize.X = csbi.dwSize.X; // Keep the same number of columns (width)
        newSize.Y = 1500; // Set the desired buffer height (this allows more scrollable rows)

        // Set the new buffer size
        if (SetConsoleScreenBufferSize(hConsole, newSize)) {
            std::cout << "Console buffer size set to 1500 rows (scrollable)." << std::endl;
        }
        else {
            std::cerr << "Failed to set console buffer size." << std::endl;
        }

        // Set the window size to a fixed value (e.g., 80 columns and 20 rows)
        SMALL_RECT newWindow;
        newWindow.Top = csbi.srWindow.Top;
        newWindow.Left = csbi.srWindow.Left;
        newWindow.Right = csbi.srWindow.Left + 200; // 80 columns (0-based index)
        newWindow.Bottom = csbi.srWindow.Top + 3000; // 20 rows (0-based index)

        // Apply the window size
        if (SetConsoleWindowInfo(hConsole, TRUE, &newWindow)) {
            std::cout << "Console window size set to 80x20 and is fixed." << std::endl;
        }
        else {
            std::cerr << "Failed to set console window size." << std::endl;
        }
    }
    else {
        std::cerr << "Error getting console screen buffer info!" << std::endl;
    }
    system("color F0");

    // Maximizing the console window
    system("cls");
    fullsc();
    std::string strCommand;

    while (true) {
        int Y = 10;
        system("cls");
        game.displayGame();  // Display the initial game state
        setCursorPosition(90, Y);
        std::cout << "----- Menu -----";
        setCursorPosition(80, Y+=2);
        std::cout << "1: Move card from Stock to Waste";
        setCursorPosition(80, Y+=1);
        std::cout << "2: Move card from Waste to Foundation";
        setCursorPosition(80, Y+=1);
        std::cout << "3: Move card from Waste to Tableau";
        setCursorPosition(80, Y+=1);
        std::cout << "4: Move card from Tableau to Foundation";
        setCursorPosition(80, Y+=1);
        std::cout << "5: Move card from foundation to Tableau";
        setCursorPosition(80, Y+=1);
        std::cout << "6: Move card between Tableaus";
        setCursorPosition(80, Y+=1);
        std::cout << "7: Move multiple cards between Tableaus";
        setCursorPosition(80, Y+=1);
        std::cout << "8: Display Hint";
        setCursorPosition(80, Y+=1);
        std::cout << "9: Exit";
        setCursorPosition(80, Y+=1);
        std::cout << "Enter your command: ";
        std::cin >> strCommand;

        int command;
        try
        {
            command = std::stoi(strCommand);
        }
        catch(const std::exception& e)
        {
            setCursorPosition(80, Y+=1);
            std::cerr << "Invalid command. Please try again.";
            Sleep(2000);
            continue;
        }
        
        int tableauIndex, foundationIndex, fromTableauIndex, toTableauIndex, count;

        switch (command)
        {
        case 1:
            game.moveCard(0, 1);
            break;
        case 2:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter foundation index: ";
            std::cin >> foundationIndex;
            game.moveCard(1, foundationIndex + 8);
            break;
        case 3:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter tableau index: ";
            std::cin >> tableauIndex;
            game.moveCard(1, tableauIndex + 1);
            break;
        case 4:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter tableau index: ";
            std::cin >> tableauIndex;
            setCursorPosition(80, Y+=1);
            std::cout << "Enter Foundation index: ";
            std::cin >> foundationIndex;
            game.moveCard(tableauIndex + 1, foundationIndex + 8);
            break;
        case 5:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter from foundation index: ";
            std::cin >> foundationIndex;
            setCursorPosition(80, Y+=1);
            std::cout << "Enter to tableau index: ";
            std::cin >> tableauIndex;
            game.moveCard(foundationIndex + 8, tableauIndex + 1);
            break;
        case 6:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter from tableau index: ";
            std::cin >> fromTableauIndex;
            setCursorPosition(80, Y+=1);
            std::cout << "Enter to tableau index: ";
            std::cin >> toTableauIndex;
            game.moveCard(fromTableauIndex + 1, toTableauIndex + 1);
            break;
        case 7:
            setCursorPosition(80, Y+=1);
            std::cout << "Enter from tableau index: ";
            std::cin >> fromTableauIndex;
            setCursorPosition(80, Y+=1);
            std::cout << "Enter to tableau index: ";
            std::cin >> toTableauIndex;
            setCursorPosition(80, Y+=1);
            std::cout << "Enter number of cards you want to move: ";
            std::cin >> count;
            if(!game.moveCard(fromTableauIndex + 1, toTableauIndex + 1, count)) {
                setCursorPosition(80, Y+=1);
                std::cerr << "Invalid move" << std::endl;
                Sleep(2000);
            }
            break;
        case 8:
            setCursorPosition(80, Y+=1);
            std::cout << game.displayHint();
            Sleep(1000);
            break;
        case 9:
            setCursorPosition(80, Y+=1);
            std::cout << "Exiting game...";
            break;
        default:
            setCursorPosition(80, Y+=1);
            std::cerr << "Invalid command. Please try again.";
            Sleep(2000);
            break;
        }

        if (game.isGameWon()) {
            system("cls");
            setCursorPosition(80, 10);
            std::cout << "Congratulations! You have won the game!";
            Sleep(5000);
            break;
        }
    }
}
