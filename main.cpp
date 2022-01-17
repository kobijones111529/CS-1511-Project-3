#include <algorithm>
#include <iostream>
#include <string>

bool makePick(int &remaining, int pick, int limit) {
    if (pick < 1) {
        return false;
    }
    else if (pick > limit) {
        return false;
    }
    else if (remaining - pick < 0) {
        return false;
    }
    else {
        remaining -= pick;
    }
    return true;
}

int main() {
    int toothpicksRemaining = 23;
    int picksPerTurn = 3;
    int defaultComputerPick = 1;

    bool running = true;
    while (running) {
        std::string userPickIn = "";
        int userPick = 0;

        std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
        std::cout << "How many will you choose? (1-" << picksPerTurn << "): ";
        std::cin >> userPickIn;

        try {
            userPick = stoi(userPickIn);
            if (!makePick(toothpicksRemaining, userPick, picksPerTurn)) {
                std::cout << "Your pick was not valid" << std::endl;
                continue;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Your pick was not valid" << std::endl;
            continue;
        }

        std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;

        if (toothpicksRemaining == 1) {
            std::cout << "Human wins!" << std::endl;
            toothpicksRemaining = 23;
            continue;
        } else if (toothpicksRemaining == 0) {
            std::cout << "Computer wins!" << std::endl;
            toothpicksRemaining = 23;
            continue;
        }

        int computerPick = (toothpicksRemaining - 1) % (picksPerTurn + 1);
        if (computerPick == 0) {
            if (toothpicksRemaining > picksPerTurn + 1) {
                computerPick = rand() % picksPerTurn + 1;
            } else {
                computerPick = 1;
            }
        }
        std::cout << "Computer picks " << computerPick << std::endl;
        toothpicksRemaining -= computerPick;

        if (toothpicksRemaining == 1) {
            std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
            std::cout << "Computer wins!" << std::endl;
            toothpicksRemaining = 23;
            continue;
        }
    }

    return 0;
}