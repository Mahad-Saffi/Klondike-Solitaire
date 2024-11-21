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

#include "include/Deck.h"
#include "include/StockPile.h"
#include "include/WastePile.h"
#include "include/FoundationPile.h"
#include "include/TableauPile.h"
#include "include/GameLogic.h"

void setConsoleSettings();
void gameLoop();

int main() {
    setConsoleSettings();
    system("cls");
    fullsc();
    gameLoop();
}


void setConsoleSettings() {
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
}


void gameLoop() {
    GameLogic game;
    int source, destination, cardCount;
    std::string strCommand;
    bool isGameEnded = false;

    while (!isGameEnded) {
        int Y = 10;
        system("cls");
        game.displayGame();
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
            isGameEnded = true;
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
