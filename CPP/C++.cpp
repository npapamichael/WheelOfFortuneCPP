// Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <unordered_set>


//using namespace std to avoid extra code.
using namespace std;

enum Types {
    // Special types for all slices

    Money = 1,
    LoseTurn = 2,
    Bankrupt = 3,
    BankruptP = 4,
    SecondChance = 5,
    Steal = 6,
    Jackpot = 7
};

class CSlice {
public:
    // Initalizing type, amount and description for the slices
    int type;
    int amount;
    string description;
};

const int numSlices = 16;
CSlice wheelSlices[numSlices];

void initializeWheel() {
    ifstream wheel("wheel.txt");

    // Read and store CSlice objects
    for (int i = 0; i < numSlices; ++i) {
        wheel >> wheelSlices[i].type >> wheelSlices[i].amount;
        getline(wheel >> ws, wheelSlices[i].description);
    }

    wheel.close();
}

int spinWheel() {
    // Simulate spinning the wheel and getting a random slice
    return rand() % numSlices;
}

int main() {

    cout << "Welcome everybody to the WHEEL of FORTUNE!!" << endl;
    cout << "Our contestants for today will be John and Maria." << endl;
    srand(static_cast<unsigned>(time(nullptr)));

    // Initialize the wheel slices
    initializeWheel();

    // Open the file containing rounds
    ifstream wordFile("rounds.txt");

    // Variables to store total money accumulated by each contestant and extra tokens
    int extrachanceJohn = 0;
    int extrachanceMaria = 0;
    int totalJohnMoney = 0;
    int totalMariaMoney = 0;

    // Keep playing rounds until no more words left in the file
    string selectedWord;
    while (wordFile >> selectedWord) {
        // Initialize variables for the current round
        int johnMoney = 0;
        int mariaMoney = 0;

        // Reset game state for the new round
        int counter = selectedWord.size();
        vector<char> hidden(selectedWord.size(), '_');
        unordered_set<char> availableLetters;
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            availableLetters.insert(ch);
        }
        bool foundLetter = false;


        //seed 5 for consisted results.
        srand(5);
        while (counter != 0) {
            int randomNum;
            char chosenLetter;

            int temp = rand() % 2 + 1;

            do {
                randomNum = rand() % numSlices;
                CSlice currentSlice = wheelSlices[randomNum];

                cout << (temp == 1 ? "John" : "Maria") << " spins and lands on ";

                if (currentSlice.type == Money) {
                    int moneyAmount = currentSlice.amount;
                    cout << moneyAmount << "EUR." << endl;

                    auto it = availableLetters.begin();
                    advance(it, rand() % availableLetters.size());
                    chosenLetter = *it;

                    cout << "Guessing letter: " << chosenLetter << endl;

                    availableLetters.erase(chosenLetter);
                    // if letter is found then spin the wheel
                    size_t foundPos = selectedWord.find(chosenLetter);
                    if (foundPos != string::npos) {
                        for (size_t i = 0; i < selectedWord.size(); ++i) {
                            if (selectedWord[i] == chosenLetter && hidden[i] == '_') {
                                hidden[i] = chosenLetter;
                                --counter;

                                if (temp == 1)
                                    johnMoney += moneyAmount;
                                else
                                    mariaMoney += moneyAmount;
                            }
                        }

                        cout << (temp == 1 ? "John" : "Maria") << "'s total money: " << (temp == 1 ? johnMoney : mariaMoney) << "EUR." << endl;
                        cout << "Hidden Word: ";
                        for (char letter : hidden) {
                            cout << letter << ' ';
                        }
                        cout << endl;

                        // if word found exit the while loop
                        if (counter == 0) {
                            cout << "Word found!!" << endl;
                            break;
                        }
                    }
                    else {
                        cout << "Incorrect guess." << endl;
                        foundLetter = false;
                    }
                }
                else {
                    cout << "Type: " << currentSlice.type << ", Amount: " << currentSlice.amount
                        << ", Description: " << currentSlice.description << endl;
                    foundLetter = false;
                }
                if (currentSlice.type == Bankrupt) { // Initalizes the Bunkrupt slice amount and couts.
                    if (temp == 1) { //Player One
                        johnMoney = 0;
                        extrachanceJohn = 0;
                        cout << "bankrupt." << endl;
                        cout << "John's money is now 0" << endl;

                    }
                    else if (temp == 2) { //Player 2 
                        mariaMoney = 0;
                        extrachanceMaria = 0;
                        cout << "bankrupt." << endl;
                        cout << "Maria's money is now 0" << endl;
                    }
                }
                if (currentSlice.type == LoseTurn) {// Initalizes the LoseTutn slice couts and player switch.
                    if (temp == 1) {
                        if (extrachanceJohn < 1) { // Player One.
                            cout << "John loses his turn." << endl;
                        }
                        else if (extrachanceJohn > 0) {
                            extrachanceJohn--;
                            cout << "John used a second chance token." << endl;
                            cout << "John's remaining tokens are: " << extrachanceJohn << endl;
                            temp = (temp == 1) ? 2 : 1;
                        }

                    }
                    else if (temp == 2) { // Player Two.
                        if (extrachanceMaria < 1) {
                            cout << "Maria loses his turn." << endl;
                        }
                        else if (extrachanceMaria > 0) {
                            extrachanceMaria--;
                            cout << "Maria used a second chance token." << endl;
                            cout << "Maria's remaining tokens are: " << extrachanceMaria << endl;
                            temp = (temp == 1) ? 2 : 1;
                        }

                    }
                }
                if (currentSlice.type == Jackpot) { // Initializes Jackpot slice and doubles money.
                    if (temp == 1) { // Player One.
                        cout << "JOHN HAS LANDED ON JACKPOT!!" << endl;
                        cout << "John doubles his money!" << endl;
                        johnMoney = johnMoney * 2;
                        cout << "John's total money: " << johnMoney << "EUR." << endl;

                    }
                    else if (temp == 2) { //Plauyer two.
                        cout << "MARIA HAS LANDED ON JACKPOT!!" << endl;
                        cout << "Maria doubles his money!" << endl;
                        mariaMoney = mariaMoney * 2;
                        cout << "Maria's total money: " << mariaMoney << "EUR." << endl;

                    }
                }
                if (currentSlice.type == Steal) { // Initializes Steal slice and halves money.
                    if (temp == 1) { // Player One.
                        cout << "John lands on steal" << endl;
                        mariaMoney = mariaMoney / 2;
                        johnMoney = johnMoney + mariaMoney;
                        cout << "John's total money: " << johnMoney << "EUR." << endl;

                    }
                    else if (temp == 2) { // Player Two.
                        cout << "Maria lands on steal" << endl;
                        johnMoney = johnMoney / 2;
                        mariaMoney = johnMoney + mariaMoney;
                        cout << "Maria's total money: " << mariaMoney << "EUR." << endl;

                    }
                }
                if (currentSlice.type == SecondChance) { // Initializes SecondChance slice and gives extra token
                    if (temp == 1) { // Player One.
                        extrachanceJohn++;
                        cout << "John's tokens are now: " << extrachanceJohn << endl;

                    }
                    else if (temp == 2) { // Player Two.
                        extrachanceMaria++;
                        cout << "Maria's tokens are now: " << extrachanceMaria << endl;
                    }
                }



                // Handle other slice types

                if (!foundLetter) {
                    temp = (temp == 1) ? 2 : 1;
                }
            } while (availableLetters.size() > 0);
        }

        // Accumulate total money for each contestant
        totalJohnMoney += johnMoney;
        totalMariaMoney += mariaMoney;

        cout << "John has won " << johnMoney << " in this round." << endl;
        cout << "Maria has won " << mariaMoney << " in this round." << endl;
    }
    // Print the total money of each player
    cout << "Total money earned by John: " << totalJohnMoney << endl;
    cout << "Total money earned by Maria: " << totalMariaMoney << endl;


    //Determine the winner.
    if (totalMariaMoney > totalJohnMoney) {
        cout << "MARIA HAS WON" << endl;
    }
    else if (totalJohnMoney > totalMariaMoney) {
        cout << "JOHN HAS WON" << endl;
    }
    else {
        cout << "It's a tie!" << endl;
    }

    cout << "Thank you for playing!" << endl;

    return 0;
}