#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

//========================
// CARD STRUCTURE
//========================
class Card {
public:
    string value;
    string suit;
    Card* next;

    Card(string v = "", string s = "") : value(v), suit(s), next(nullptr) {}

    string toString() {
        return value + " of " + suit;
    }

    bool isEmpty() {
        return value == "" && suit == "";
    }
};

//========================
// STACK (for Earned & Center Cards)
//========================
class Stack {
public:
    Card* top;
    Stack() { top = nullptr; }

    void push(Card c) {
        Card* newCard = new Card(c.value, c.suit);
        newCard->next = top;
        top = newCard;
    }

    Card pop() {
        if (!top) return Card();
        Card* temp = top;
        top = top->next;
        Card c = *temp;
        delete temp;
        return c;
    }

    Card peek() {
        if (!top) return Card();
        return *top;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void stealFrom(Stack& other) {
        if (other.isEmpty()) return;
        Card* temp = other.top;
        while (temp) {
            push(*temp);
            temp = temp->next;
        }
        while (!other.isEmpty()) other.pop();
    }

    void display() {
        Card* curr = top;
        int i = 1;
        while (curr) {
            cout << i++ << ". " << curr->toString() << endl;
            curr = curr->next;
        }
    }

    int collectMatches(Card card, Stack& earned) {
        Stack temp;
        int count = 0;
        while (!isEmpty()) {
            Card c = pop();
            if (c.value == card.value) {
                earned.push(c);
                count++;
            } else {
                temp.push(c);
            }
        }
        while (!temp.isEmpty()) push(temp.pop());
        return count;
    }

    // ➕ Find N-th node from the end
    Card findNthFromEnd(int n) {
        Card* first = top;
        Card* second = top;

        for (int i = 0; i < n; i++) {
            if (!first) return Card(); // out of range
            first = first->next;
        }

        while (first) {
            first = first->next;
            second = second->next;
        }

        return second ? *second : Card();
    }
};

//========================
// QUEUE (for Draw Pile)
//========================
class Queue {
    Card* front;
    Card* rear;
public:
    Queue() { front = rear = nullptr; }

    void enqueue(Card c) {
        Card* newCard = new Card(c.value, c.suit);
        if (!rear) {
            front = rear = newCard;
        } else {
            rear->next = newCard;
            rear = newCard;
        }
    }

    Card dequeue() {
        if (!front) return Card();
        Card* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        Card c = *temp;
        delete temp;
        return c;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};

//========================
// PLAYER CLASS
//========================
class Player {
public:
    Card hand[10];
    int handSize;
    Stack earnedPile;

    Player() { handSize = 0; }

    void addCardToHand(Card c) {
        if (handSize < 10)
            hand[handSize++] = c;
    }

    void displayHand() {
        cout << "Player's Hand:\n";
        for (int i = 0; i < handSize; i++) {
            cout << i + 1 << ". " << hand[i].toString() << endl;
        }
    }

    Card lookAtHand(int index) {
        if (index < 0 || index >= handSize) return Card();
        return hand[index];
    }

    Card removeCardFromHand(int index) {
        if (index < 0 || index >= handSize) return Card();
        Card selected = hand[index];
        for (int i = index; i < handSize - 1; i++) hand[i] = hand[i + 1];
        handSize--;
        return selected;
    }

    Card getMatchingCard(Card c) {
        for (int i = 0; i < handSize; i++) {
            if (hand[i].value == c.value)
                return hand[i];
        }
        return Card();
    }

    void removeMatchingCardFromHand(Card c) {
        for (int i = 0; i < handSize; i++) {
            if (hand[i].value == c.value) {
                removeCardFromHand(i);
                return;
            }
        }
    }

    int getHandSize() { return handSize; }

    int calculatePoints() {
        int score = 0;
        Card* curr = earnedPile.top;
        while (curr) {
            if (curr->value == "J" || curr->value == "Q" || curr->value == "K")
                score += 10;
            else if (curr->value == "A")
                score += 20;
            else
                score += 5;
            curr = curr->next;
        }
        return score;
    }
};

//========================
// GAME CLASS
//========================
class Game {
private:
    Queue drawPile;
    Stack centerCards;
    Player* players;
    int numPlayers;

    void shuffleDeck(Card* deck, int size) {
        for (int i = 0; i < size; i++) {
            int r = rand() % size;
            Card temp = deck[i];
            deck[i] = deck[r];
            deck[r] = temp;
        }
    }

    void displayCenterCards() {
        cout << "\nCenter cards:\n";
        centerCards.display();
    }

    void displayAllPlayersEarnedTops() {
        for (int i = 0; i < numPlayers; i++) {
            cout << "Player " << i + 1 << " Earned Pile: ";
            if (players[i].earnedPile.isEmpty()) cout << "(empty)\n";
            else cout << players[i].earnedPile.peek().toString() << "\n";
        }
    }

    void processTurn(int playerIdx) {
        Player& p = players[playerIdx];
        cout << "\n==============================\n";
        cout << "      PLAYER " << playerIdx + 1 << " TURN\n";
        cout << "==============================\n";

        bool turnActive = true;
        while (turnActive) {
            if (!drawPile.isEmpty()) {
                Card drawn = drawPile.dequeue();
                cout << "You drew: " << drawn.toString() << "\n";
                p.addCardToHand(drawn);
            }

            while (true) {
                displayCenterCards();
                p.displayHand();
                displayAllPlayersEarnedTops();

                cout << "Choose action:\n1. Make a move (match/steal)\n2. Put a card to center\nChoice: ";
                int action;
                cin >> action;
                if (action == 2) {
                    cout << "Which card to put (1-" << p.getHandSize() << "): ";
                    int discard;
                    cin >> discard;
                    centerCards.push(p.removeCardFromHand(discard - 1));
                    turnActive = false;
                    break;
                } else {
                    cout << "Enter move (1-" << p.getHandSize() << " for hand, a-... for steal): ";
                    string move;
                    cin >> move;
                    bool matched = false;
                    if (isdigit(move[0])) {
                        int idx = stoi(move) - 1;
                        Card selected = p.lookAtHand(idx);
                        int collected = centerCards.collectMatches(selected, p.earnedPile);
                        if (collected > 0) {
                            p.earnedPile.push(selected);
                            p.removeCardFromHand(idx);
                            matched = true;
                            cout << collected << " cards matched and collected!\n";
                        } else {
                            cout << "No match found.\n";
                        }
                    } else {
                        int target = move[0] - 'a';
                        if (target >= 0 && target < numPlayers && target != playerIdx) {
                            Card myTop = p.getMatchingCard(players[target].earnedPile.peek());
                            if (!myTop.isEmpty()) {
                                p.earnedPile.stealFrom(players[target].earnedPile);
                                p.earnedPile.push(myTop);
                                p.removeMatchingCardFromHand(myTop);
                                matched = true;
                                cout << "Stolen earned pile from Player " << target + 1 << "!\n";
                            } else {
                                cout << "No match with top card of Player " << target + 1 << ".\n";
                            }
                        }
                    }

                    if (!matched) {
                        cout << "No valid move made. Please try again or discard a card.\n";
                    } else {
                        if (!drawPile.isEmpty()) {
                            Card drawn = drawPile.dequeue();
                            cout << "You drew: " << drawn.toString() << "\n";
                            p.addCardToHand(drawn);
                        }
                    }
                }
            }
        }
    }

public:
    Game(int n) : numPlayers(n) {
        players = new Player[numPlayers];
        Card fullDeck[52];
        string suits[] = {"Clubs", "Spades", "Hearts", "Diamonds"};
        string values[] = {"A", "K", "Q", "J", "10", "9", "8", "7", "6", "5", "4", "3", "2"};

        int idx = 0;
        for (string suit : suits) {
            for (string value : values) {
                fullDeck[idx++] = Card(value, suit);
            }
        }

        shuffleDeck(fullDeck, 52);
        for (int i = 0; i < 52; i++) drawPile.enqueue(fullDeck[i]);

        for (int i = 0; i < numPlayers; i++) {
            for (int j = 0; j < 4; j++) {
                players[i].addCardToHand(drawPile.dequeue());
            }
        }

        for (int i = 0; i < 4; i++) {
            centerCards.push(drawPile.dequeue());
        }
    }

    void startGame() {
        while (!drawPile.isEmpty()) {
            for (int i = 0; i < numPlayers; i++) {
                if (players[i].getHandSize() == 0 && drawPile.isEmpty()) continue;
                processTurn(i);
            }
        }

        cout << "\n🎉 Game Over! 🎉\n";
        int maxPoints = -1, winner = -1;
        for (int i = 0; i < numPlayers; i++) {
            int pts = players[i].calculatePoints();
            cout << "Player " << i + 1 << " Score: " << pts << "\n";
            if (pts > maxPoints) {
                maxPoints = pts;
                winner = i;
            }
        }
        cout << "🏆 Winner is Player " << winner + 1 << " with " << maxPoints << " points!\n";
    }
};

//========================
// MAIN FUNCTION
//========================
int main() {
    srand(time(0));

    // Welcome and Instructions
    cout << "=========================================\n";
    cout << "      🎴 WELCOME TO THE CARD MATCH GAME! 🎴\n";
    cout << "=========================================\n";
    cout << "Instructions:\n";
    cout << "1. 🎯 Match cards from your hand with center cards to earn them.\n";
    cout << "2. 🕵️ Steal another player's earned pile if you have a card matching their top.\n";
    cout << "3. 🗑️ No move? Discard a card to the center.\n";
    cout << "4. 🧠 Each round, draw a new card.\n";
    cout << "5. 🏆 Scoring:\n";
    cout << "   - A = 20 pts\n   - K/Q/J = 10 pts\n   - Numbers = 5 pts\n";
    cout << "=========================================\n\n";

    int n;
    cout << "Enter number of players (min 2): ";
    cin >> n;

    if (n < 2 ||n=='a') {
        cout << "❌ Enter a valid number of players (minimum 2)!\n";
    } else {
        Game game(n);
        game.startGame();
    }

    return 0;
}
