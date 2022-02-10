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
#include <optional>
#include <sstream>
#include <string>

// Game difficulty, determines how smart the computer is
enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

std::string prompt(const std::string &msg) {
    std::string input;
    std::cout << msg;
    std::cin >> input;
    return input;
}

bool isQuitMessage(const std::string &input) {
    return input == "q" || input == "Q";
}

// Prompt user for an integer value
std::optional<int> toInt(const std::string& str) {
    try {
        return std::optional<int>(stoi(str));
    }
    catch (const std::invalid_argument&) { }
    catch (const std::out_of_range&) { }
    return std::nullopt;
}

// Returns std::nullopt if user wishes to quit
std::optional<Difficulty> promptDifficulty() {
    // Repeat until valid input or quit
    const auto invalidInput = []() {
        std::cerr << "Not a difficulty option" << std::endl;
        return promptDifficulty();
    };

    std::string input = prompt("Difficulty ('0' = Easy, '1' = Medium, '2' = Hard): ");

    if (isQuitMessage(input)) {
        return std::nullopt;
    }

    std::optional<int> inputAsInt = toInt(input);

    if (!inputAsInt.has_value()) {
        return invalidInput();
    }

    switch (inputAsInt.value()) {
    case 0:
        return Difficulty::EASY;
    case 1:
        return Difficulty::MEDIUM;
    case 2:
        return Difficulty::HARD;
    default:
        return invalidInput();
    }
}

// Check if pick is legal
bool validatePick(int remaining, int pick, int limit) {
    return pick > 0 && pick <= limit && pick <= remaining;
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
        // Keep best pick if on hard
    }
    return pick;
}

// Return std::nullopt if user wishes to quit
std::optional<int> getHumanPick(int remaining, int limit) {
    std::stringstream ss;
    ss << "How many will you choose? (1-" << limit << "): ";

    std::string input = prompt(ss.str());

    if (isQuitMessage(input)) {
        return std::nullopt;
    }

    std::optional<int> inputAsInt = toInt(input);

    // Repeat until user enters a valid pick or quits
    if (!inputAsInt.has_value() || !validatePick(remaining, inputAsInt.value(), limit)) {
        std::cout << "Your pick was not valid" << std::endl;
        std::cout << "Toothpicks remaining: " << remaining << std::endl;
        return getHumanPick(remaining, limit);
    }

    return inputAsInt.value();
}

int main() {
    // Seed rng
    srand(time(nullptr));

    const int startingToothpicks = 23;
    const int picksPerTurn = 3;

    // Main loop
    bool running = true;
    while (running) {
        int toothpicksRemaining = startingToothpicks;

        std::cout << std::endl << "Enter 'q' at any time to quit" << std::endl << std::endl;

        // Prompt user for difficulty
        Difficulty difficulty = [&running]() {
            std::optional<Difficulty> d = promptDifficulty();
            if (!d.has_value()) {
                running = false;
            }
            return d.value_or(Difficulty::EASY);
        }();

        if (running) {
            // Individual game loop
            bool gameOver = false;
            while (!gameOver) {
                // Human's turn
                std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
                std::optional<int> input = getHumanPick(toothpicksRemaining, picksPerTurn);
                if (!input.has_value()) {
                    gameOver = true;
                    running = false;
                }
                else {
                    toothpicksRemaining -= input.value();
                }

                // Computer's turn
                if (!gameOver) {
                    std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;

                    // Check if a winner can be decided
                    // If yes, restart or quit
                    if (toothpicksRemaining == 1) {
                        std::cout << "Human wins!" << std::endl;
                        gameOver = true;
                    } else if (toothpicksRemaining == 0) {
                        std::cout << "Computer wins!" << std::endl;
                        gameOver = true;
                    }
                }

                if (!gameOver) {
                    int computerPick = generateComputerPick(difficulty, toothpicksRemaining, picksPerTurn);
                    std::cout << "Computer picks " << computerPick << std::endl;
                    toothpicksRemaining -= computerPick;

                    // Check if the computer has made a winning pick
                    if (toothpicksRemaining == 1) {
                        std::cout << "Toothpicks remaining: " << toothpicksRemaining << std::endl;
                        std::cout << "Computer wins!" << std::endl;
                        gameOver = true;
                    }
                }
            }
        }

        // Ask the user if they want to play again
        if (running) running = [](const std::string &input) {
            return input == "y" || input == "Y";
        }(prompt("Would you like to play again? (y/n): "));;
    }

    std::cout << "Thanks for playing!" << std::endl;
}