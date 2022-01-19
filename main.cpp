#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

// Prompt user for an integer value
bool promptInt(const std::string& msg, int& intOut) {
    std::string res = "";

    std::cout << msg;
    std::cin >> res;

    try {
        intOut = stoi(res);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

// Prompt user for a boolean value
bool promptBool(const std::string& msg, const std::function<bool(std::string)>& cmp) {
    std::string res = "";

    std::cout << msg;
    std::cin >> res;

    return cmp(res);
}

// Ask the user if they wish to keep playing
bool promptContinue() {
    return promptBool("Would you like to play again? (y/n): ", [](std::string input) -> bool {
        if (input == "y" || input == "Y") {
            return true;
        }
        else {
            return false;
        }
    }); 
}

// Remove toothpicks from pile if legal
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

// Figure out the ideal pick, otherwise choose random number
int calculateBestPick(int remaining, int limit) {
    int pick = (remaining - 1) % (limit + 1);
    if (pick < 1) {
        if (remaining > limit + 1) {
            pick = rand() % limit + 1;
        } else {
            pick = 1;
        }
    }
    return pick;
}

int main() {
    const int startingToothpicks = 23;
    const int picksPerTurn = 4;
    int toothpicksRemaining = startingToothpicks;

    bool running = true;
    while (running) {
        int userPick = 0;

        std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
        std::stringstream ss;
        ss << "How many will you choose? (1-" << picksPerTurn << "): ";
        if (!promptInt(ss.str(), userPick) || !makePick(toothpicksRemaining, userPick, picksPerTurn)) {
            std::cout << "Your pick was not valid" << std::endl;
            continue;
        }

        std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;

        if (toothpicksRemaining == 1) {
            std::cout << "Human wins!" << std::endl;
            running = promptContinue();
            toothpicksRemaining = 23;
            continue;
        } else if (toothpicksRemaining == 0) {
            std::cout << "Computer wins!" << std::endl;
            running = promptContinue();
            toothpicksRemaining = 23;
            continue;
        }

        int computerPick = calculateBestPick(toothpicksRemaining, picksPerTurn);
        std::cout << "Computer picks " << computerPick << std::endl;
        makePick(toothpicksRemaining, computerPick, picksPerTurn);

        if (toothpicksRemaining == 1) {
            std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
            std::cout << "Computer wins!" << std::endl;
            running = promptContinue();
            toothpicksRemaining = 23;
            continue;
        }
    }

    return 0;
}