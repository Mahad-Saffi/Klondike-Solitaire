# Solitaire Game

This is a console-based implementation of the classic Solitaire card game.

## Features

- Move cards between different piles (Stock, Waste, Tableau, Foundation)
- Display the current game state
- Display hints for possible moves
- Check if the game is won

## How to Play

1. **Move card from Stock to Waste**: Draw a card from the Stock pile to the Waste pile.
2. **Move card from Waste to Foundation**: Move the top card from the Waste pile to a Foundation pile.
3. **Move card from Waste to Tableau**: Move the top card from the Waste pile to a Tableau pile.
4. **Move card from Tableau to Foundation**: Move the top card from a Tableau pile to a Foundation pile.
5. **Move card from Foundation to Tableau**: Move the top card from a Foundation pile to a Tableau pile.
6. **Move card between Tableaus**: Move the top card from one Tableau pile to another.
7. **Move multiple cards between Tableaus**: Move multiple cards from one Tableau pile to another.
8. **Display Hint**: Display a hint for a possible move.
9. **Exit**: Exit the game.

## Controls

- Use the number keys (1-9) to select an action from the menu.
- Follow the prompts to enter the necessary indices for the selected action.

## Building and Running (cmd)

1. **Clone the project**:
    ```sh
    git clone https://github.com/Mahad-Saffi/Klondike-Solitaire.git
    cd Klondike-Solitaire
    ```

2. **Build the project**:
    ```sh
    g++ Solitaire.cpp -o Solitaire.exe
    ```

3. **Run the executable**:
    ```sh
    .\Solitaire.exe
    ```

## Dependencies

- Windows OS (for console manipulation functions)
- C++11 or later

## Project Glimpse

![Solitaire Game](/Demo/Demo.gif)

## Code Structure

- `Card`: Represents a single card with suit and rank.
- `Node`: Represents a node in a linked list.
- `LinkedList`: A basic linked list implementation.
- `Stack`: A stack implementation using `LinkedList`.
- `Queue`: A queue implementation using `LinkedList`.
- `Pile`: An abstract class for different types of card piles.
- `TableauPile`: Represents a tableau pile in Solitaire.
- `FoundationPile`: Represents a foundation pile in Solitaire.
- `StockPile`: Represents the stock pile in Solitaire.
- `WastePile`: Represents the waste pile in Solitaire.
- `Deck`: Represents a deck of cards.
- `Solitaire`: The main game class that handles game logic and state.

## License

This project is licensed under the MIT License.
