#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

struct Question {
    string text;
    vector<string> options;
    int correctOption;
    int points;
};

struct Player {
    string name;
    int score = 0;
    int lives = 3;
};

void shuffleQuestions(vector<Question>& questions) {
    srand(static_cast<unsigned>(time(0)));
    random_shuffle(questions.begin(), questions.end());
}

void displayQuestion(const Question& q, int questionNumber) {
    cout << "\nQuestion " << questionNumber << ":\n";
    cout << q.text << "\n";
    for (size_t i = 0; i < q.options.size(); ++i) {
        cout << i + 1 << ". " << q.options[i] << "\n";
    }
    cout << "\nYour answer (1-" << q.options.size() << "): ";
}

bool askQuestion(const Question& q, Player& player) {
    int answer = -1;
    bool timeUp = false;

    // Timer for 30 seconds
    thread timer([&]() {
        this_thread::sleep_for(chrono::seconds(20));
        if (answer == -1) {
            timeUp = true;
            cout << "\nTime's up!\n";
        }
    });

    cin >> answer;

    if (!timeUp) {
        if (answer - 1 == q.correctOption) {
            cout << "\nCorrect! You earned " << q.points << " points." << endl;
            player.score += q.points;
            timer.detach();
            return true;
        } else {
            cout << "\nWrong answer. The correct answer was: " << q.options[q.correctOption] << endl;
            player.lives--;
            timer.detach();
            return false;
        }
    } else {
        player.lives--;
        timer.join();
        return false;
    }
}

void startGame(vector<Question>& questions, Player& player) {
    shuffleQuestions(questions);

    for (size_t i = 0; i < questions.size() && player.lives > 0; ++i) {
        displayQuestion(questions[i], i + 1);
        bool answeredCorrectly = askQuestion(questions[i], player);

        if (!answeredCorrectly) {
            cout << "\nLives left: " << player.lives << endl;
        }

        if (player.lives == 0) {
            cout << "\nGame Over!" << endl;
            break;
        }
    }

    cout << "\nFinal Score: " << player.score << endl;
}

int main() {
    vector<Question> questions = {
        {"What is the boiling point of water?", {"100 degrees", "90 degrees", "120 degrees", "80 degrees"}, 0, 10},
        {"Who was the first president of the United States?", {"Abraham Lincoln", "George Washington", "Thomas Jefferson", "John Adams"}, 1, 20},
        {"How many players are on a soccer team?", {"9", "10", "11", "12"}, 2, 15},
        {"What planet is known as the Red Planet?", {"Earth", "Mars", "Jupiter", "Venus"}, 1, 10},
        {"In what year did World War II end?", {"1940", "1942", "1945", "1948"}, 2, 20},
        // Funny questions added
        {"What do you call a fish with no eyes?", {"A fish", "A blind fish", "Fsh", "A sea cucumber"}, 2, 5},
        {"Why don't skeletons fight each other?", {"They don't have the guts", "They're too busy", "They are lazy", "They have no brains"}, 0, 5},
        {"What is orange and sounds like a parrot?", {"A carrot", "An orange", "A pumpkin", "A pineapple"}, 0, 5},
        {"Why was the math book sad?", {"Because it had too many problems", "Because it couldn't add up", "Because it was too boring", "Because it had no solution"}, 0, 5},
        {"What do you call a bear with no teeth?", {"A gummy bear", "A toothless bear", "A soft bear", "A teddy bear"}, 0, 5}
    };

    Player player;
    cout << "Enter your name: ";
    cin >> player.name;

    cout << "\nWelcome, " << player.name << "! You have 3 lives. Good luck!\n";
    startGame(questions, player);

    return 0;
}

