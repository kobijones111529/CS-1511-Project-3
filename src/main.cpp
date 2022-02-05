/*
 * CS 1511 Project 3
 * Created by Kobi Jones on February 4, 2022
 * 
 * This program plays a game of "23" with the user.
 * The user and computer take turns picking toothpicks
 * (1-3) from a pile (starting with 23) until the loser
 * is forced to draw the last toothpick.
 */

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

// Game difficulty, determines how smart the computer is
enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

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

// Prompt user for difficulty
bool promptDifficulty(Difficulty &difficultyOut) {
        int res = 0;
        if (!promptInt("Difficulty ('0' = Easy, '1' = Medium, '2' = Hard): ", res)) {
            return false;
        }
        switch (res) {
        case 0:
            difficultyOut = Difficulty::EASY;
            return true;
        case 1:
            difficultyOut = Difficulty::MEDIUM;
            return true;
        case 2:
            difficultyOut = Difficulty::HARD;
            return true;
        default:
            return false;
        }
}

// Ask the user if they wish to keep playing
bool promptContinue() {
    return promptBool("Would you like to play again? (y/n): ", [](std::string input) -> bool {
        return input == "y" || input == "Y";
    }); 
}

// Remove toothpicks from pile if legal
bool makePick(int &remaining, int pick, int limit) {
    if (pick < 1 || pick > limit || pick > remaining) {
        return false;
    }

    remaining -= pick;
    return true;
}

int generateRandomPick(int limit) {
    return rand() % limit + 1;
}

// Figure out the ideal pick, otherwise choose random number
int generateComputerPick(Difficulty difficulty, int remaining, int limit) {
    int pick = (remaining - 1) % (limit + 1); // Ideal pick, between 0 (if no ideal pick) and the pick limit
    
    // If no ideal pick, choose randomly
    if (pick < 1) {
        if (remaining > limit + 1) {
            pick = generateRandomPick(limit);
        } else {
            pick = 1;
        }
    }

    // Computer ai difficulty applies above this threshold (7 for limit=3)
    if (remaining > limit * 2 + 1) {
        switch (difficulty) {
        case Difficulty::EASY:
            pick = generateRandomPick(limit); // Pick randomly every time
            break;
        case Difficulty::MEDIUM:
            pick = rand() % 2 ? pick : generateRandomPick(limit); // Pick randomly half the time
            break;
        }
    }
    return pick;
}

int main() {
    // Seed rng
    srand(time(nullptr));

    const int startingToothpicks = 23;
    const int picksPerTurn = 3;

    bool running = true;
    while (running) {
        int toothpicksRemaining = startingToothpicks;

        // Prompt user for difficulty
        Difficulty difficulty = Difficulty::EASY;
        while (!promptDifficulty(difficulty)) {
            std::cerr << "Not a valid difficulty" << std::endl;
        }

        bool gameOver = false;
        while (!gameOver) {
            int userPick = 0;

            std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
            // Prompt user for pick
            std::stringstream ss;
            ss << "How many will you choose? (1-" << picksPerTurn << "): ";
            while (!promptInt(ss.str(), userPick) || !makePick(toothpicksRemaining, userPick, picksPerTurn)) {
                std::cout << "Your pick was not valid" << std::endl;
            }

            std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;

            // Check if a winner can be decided
            // If yes, restart or quit
            if (toothpicksRemaining == 1) {
                std::cout << "Human wins!" << std::endl;
                gameOver = true;
                continue;
            } else if (toothpicksRemaining == 0) {
                std::cout << "Computer wins!" << std::endl;
                gameOver = true;
                continue;
            }

            int computerPick = generateComputerPick(difficulty, toothpicksRemaining, picksPerTurn);
            std::cout << "Computer picks " << computerPick << std::endl;
            makePick(toothpicksRemaining, computerPick, picksPerTurn);

            // Check if the computer has made a winning pick
            if (toothpicksRemaining == 1) {
                std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
                std::cout << "Computer wins!" << std::endl;
                gameOver = true;
                continue;
            }
        }

        // Ask the user if they want to play again
        running = promptContinue();
    }

    std::cout << "Thanks for playing!" << std::endl;
}