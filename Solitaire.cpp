#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <array>
#include <string>
#include <memory>
#include <windows.h>
#include <ctime>

#include "include/Deck.h"
#include "include/StockPile.h"
#include "include/WastePile.h"
#include "include/FoundationPile.h"
#include "include/TableauPile.h"
#include "include/GameLogic.h"
#include "include/UI.h"

int main() {
    GameLoop();
}
