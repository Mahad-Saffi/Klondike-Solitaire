#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <queue>
#include "GameLogic.h"
#include "Card.h"

#define printSelectedCard_X 100
#define printSelectedCard_Y 17


void setConsoleSettings() {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        COORD newSize;
        newSize.X = csbi.dwSize.X;
        newSize.Y = 1500;

        if (SetConsoleScreenBufferSize(hConsole, newSize)) {
            std::cout << "Console buffer size set to 1500 rows (scrollable)." << std::endl;
        }
        else {
            std::cerr << "Failed to set console buffer size." << std::endl;
        }

        SMALL_RECT newWindow;
        newWindow.Top = csbi.srWindow.Top;
        newWindow.Left = csbi.srWindow.Left;
        newWindow.Right = csbi.srWindow.Left + 200;
        newWindow.Bottom = csbi.srWindow.Top + 3000;

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
}

void hideCursor() {
    try {
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.bVisible = false;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    } catch (const std::exception& e) {
        std::cerr << "Exception in hideCursor: " << e.what() << std::endl;
    }
}

void printDataAtXY(int x, int y, const std::string& data) {
    setCursorPosition(x, y);
    std::cout << std::string(60, ' ');
    setCursorPosition(x, y);
    std::cout << "=> " << data;
}

void clearOutputArea(int x, int y, int lines) {
    for (int i = 0; i < lines; ++i) {
        setCursorPosition(x, y + i);
        std::cout << std::string(55, ' ');
    }
}

void updatePositions(GameLogic& game, std::vector<std::vector<std::vector<int>>>& TableauPositions, std::vector<std::vector<int>>& FoundationPositions, std::vector<std::vector<int>>& AdditionalPilesPositions) {
    int Y_START = 245;
    int X_START = 12;
    for (int i = 0; i < 7; ++i) {
        Y_START = 245;
        X_START = 12 + i * (58 + 38);
        int X_END = X_START + 58;
        TableauPositions[i].clear();
        if (game.getTableauPiles()[i].getCards().isEmpty()) {
            TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 160});
            continue;
        }
        for (int j = 0; j < game.getTableauPiles()[i].getCards().getSize(); ++j) {
            if (j == game.getTableauPiles()[i].getCards().getSize() - 1) {
                TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 80});
                Y_START += 80;
            } else {
                TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 38});
                Y_START += 38;
            }
        }
    }
    AdditionalPilesPositions[0] = {145, 205, 78, 160};
    AdditionalPilesPositions[1] = {222, 280, 78, 160};

    int Start_X = 492;
    int Common_X_DIFF = 58;
    int COMMON_PILE_TO_PILE_X_DIFF = 38;
    int Start_Y = 78;
    int END_Y = 160;
    for (int i = 0; i < 4; ++i) {
        FoundationPositions[i] = {Start_X, Start_X + Common_X_DIFF, Start_Y, END_Y};
        Start_X += Common_X_DIFF + COMMON_PILE_TO_PILE_X_DIFF;
    }
}

void handleMouseClick(GameLogic& game, POINT pt, std::vector<std::vector<std::vector<int>>>& TableauPositions, std::vector<std::vector<int>>& FoundationPositions, std::vector<std::vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardIndex, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
    if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
        for (int i = 0; i < 7; ++i) {
            Stack cards = game.getTableauPiles()[i].getCards();
            int cardCount = cards.getSize();
            std::vector<Card> tempCards;

            for (int j = 0; j < cardCount; ++j) {
                if (!cards.isEmpty()) {
                    tempCards.push_back(cards.pop());
                } else {
                    std::cerr << "Error: Attempted to pop from an empty stack." << std::endl;
                    break;
                }
            }

            for (int j = cardCount - 1; j >= 0; --j) {
                const auto& card = tempCards[j];
                int posIndex = cardCount - 1 - j;
                if (posIndex < TableauPositions[i].size() && pt.x >= TableauPositions[i][posIndex][0] && pt.x <= TableauPositions[i][posIndex][1] && pt.y >= TableauPositions[i][posIndex][2] && pt.y <= TableauPositions[i][posIndex][3] && card.isFaceUp()) {
                    isCardSelected = true;
                    selectedCard = card;
                    selectedTableau = i;
                    selectedCardIndex = posIndex;
                    selectedCardCount = cardCount - posIndex;
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Selected Card: ");
                    PrintCardBox(card.getSuit(), card.getRank(), printSelectedCard_X, printSelectedCard_Y);
                    break;
                }
            }
            if (isCardSelected) break;
        }

        if (!isCardSelected) {
            const auto& wastePile = game.getWastePile();
            if (!wastePile.isEmpty() && pt.x >= AdditionalPilesPositions[1][0] && pt.x <= AdditionalPilesPositions[1][1] && pt.y >= AdditionalPilesPositions[1][2] && pt.y <= AdditionalPilesPositions[1][3]) {
                selectedCard = wastePile.topCard();
                isWasteSelected = true;
                clearOutputArea(80, 15, 15);
                printDataAtXY(80, 20, "Selected Card: ");
                PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
            }
        }

        if (!isCardSelected && !isWasteSelected) {
            for (int i = 0; i < 4; ++i) {
                if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                    const auto& foundationPile = game.getFoundationPiles()[i];
                    if (!foundationPile.isEmpty()) {
                        selectedCard = foundationPile.topCard();
                        isFoundationSelected = true;
                        selectedFoundation = i;
                        clearOutputArea(80, 15, 15);
                        printDataAtXY(80, 20, "Selected Card: ");
                        PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
                    }
                }
            }
        }

        // Check for stock pile click
        if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
            if (pt.x >= AdditionalPilesPositions[0][0] && pt.x <= AdditionalPilesPositions[0][1] && pt.y >= AdditionalPilesPositions[0][2] && pt.y <= AdditionalPilesPositions[0][3]) {
                game.drawCard();
                updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                system("cls");
                game.displayGame();
            }
        }
    }
}

void handleCardMove(GameLogic& game, POINT pt, std::vector<std::vector<std::vector<int>>>& TableauPositions, std::vector<std::vector<int>>& FoundationPositions, std::vector<std::vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
    if (isCardSelected) {
        for (int i = 0; i < 7; ++i) {
            if (i != selectedTableau) {
                int lastIndex = TableauPositions[i].size() - 1;
                if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                    bool move = game.moveCard(selectedTableau + 2, i + 2, selectedCardCount);
                    if (!move) {
                        clearOutputArea(80, 15, 15);
                        printDataAtXY(80, 20, "Invalid Move!");
                        isCardSelected = false;
                    } else {
                        isCardSelected = false;
                        updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                        system("cls");
                        game.displayGame();
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                bool move = game.moveCard(selectedTableau + 2, i + 9, selectedCardCount);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isCardSelected = false;
                } else {
                    isCardSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    } else if (isWasteSelected) {
        for (int i = 0; i < 7; ++i) {
            int lastIndex = TableauPositions[i].size() - 1;
            if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                bool move = game.moveCard(1, i + 2);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isWasteSelected = false;
                } else {
                    isWasteSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                bool move = game.moveCard(1, i + 9);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isWasteSelected = false;
                } else {
                    isWasteSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    } else if (isFoundationSelected) {
        for (int i = 0; i < 7; ++i) {
            int lastIndex = TableauPositions[i].size() - 1;
            if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                bool move = game.moveCard(selectedFoundation + 9, i + 2);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isFoundationSelected = false;
                } else {
                    isFoundationSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    }
}

void handleDeselection(bool& isCardSelected, bool& isWasteSelected, bool& isFoundationSelected, int selectedTableau, int selectedFoundation) {
    if (isCardSelected) {
        isCardSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    } else if (isWasteSelected) {
        isWasteSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    } else if (isFoundationSelected) {
        isFoundationSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    }
}

void handleModeSwitch(bool& isDarkMode, GameLogic& game) {
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        system("color 0F"); // Black background, white text
    } else {
        system("color F0"); // White background, black text
    }
    system("cls");
    game.displayGame();
    clearOutputArea(80, 25, 1);
    printDataAtXY(80, 25, "Switched to " + std::string(isDarkMode ? "Dark" : "Light") + " Mode");
    Sleep(200);
}

void GameLoop() {
    setConsoleSettings();
    system("color F0");
    system("cls");
    fullsc();
    
    bool isDarkMode = false;
    try {
        GameLogic game;
        std::vector<std::vector<std::vector<int>>> TableauPositions(7);
        std::vector<std::vector<int>> FoundationPositions(4);
        std::vector<std::vector<int>> AdditionalPilesPositions(2);
        std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
        hideCursor();

        system("cls");
        game.displayGame();

        updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
        while (true) {
            try {
                int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count();
                setCursorPosition(95, 4);
                std::cout << "Time Elapsed: " << elapsedTime << "s";

                SHORT leftButton = GetAsyncKeyState(VK_LBUTTON);
                SHORT rightButton = GetAsyncKeyState(VK_RBUTTON);

                POINT pt;
                GetCursorPos(&pt);

                static bool isCardSelected = false;
                static Card selectedCard;
                static int selectedTableau = -1;
                static int selectedCardIndex = -1;
                static int selectedCardCount = 1;
                static bool isWasteSelected = false;
                static bool isFoundationSelected = false;
                static int selectedFoundation = -1;

                if (leftButton & 0x8000) {
                    handleMouseClick(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardIndex, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
                    handleCardMove(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
                    Sleep(200);
                }

                if (GetAsyncKeyState('D') & 0x8000) {
                    handleDeselection(isCardSelected, isWasteSelected, isFoundationSelected, selectedTableau, selectedFoundation);
                }

                if (GetAsyncKeyState('S') & 0x8000) {
                    handleModeSwitch(isDarkMode, game);
                }

                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    break;
                }

                Sleep(100);
            } catch (const std::exception& e) {
                std::cerr << "Exception in game loop: " << e.what() << std::endl;
            }
        }
        system("cls");
        game.displayGame();
    } catch (const std::exception& e) {
        std::cerr << "Exception in gameLoop: " << e.what() << std::endl;
    }
}
