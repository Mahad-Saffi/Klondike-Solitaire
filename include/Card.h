#pragma once

#include <windows.h>
#include <iostream>

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

void fullsc()
{
    HWND Hwnd = GetForegroundWindow();
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
    SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(Hwnd,HWND_TOP,0,0,x,y,0);
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