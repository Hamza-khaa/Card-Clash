 Card Game in C++ – OOP & Data Structures Based

This is a console-based multiplayer card game built as a university project in C++. The game is designed to demonstrate strong concepts of **Object-Oriented Programming (OOP)** and **Data Structures**, making it a practical educational project as well as an entertaining terminal game.

---

# Key Concepts

-  Object-Oriented Programming (Classes, Inheritance, Encapsulation)
-  Linked List-based Stack and Queue implementation
-  Dynamic memory management using pointers
-  Use of arrays and custom data handling
-  Game logic and flow control

---

# Game Objective

- Match cards from your hand with center cards to earn them.
- Steal another player’s earned pile by matching the top card.
- Earn points by collecting valuable cards.
- The player with the highest score wins!

---

# Features

-  2+ player mode
-  Card shuffling and dealing
-  Draw pile (Queue), Earned pile (Stack), Center pile (Stack)
-  Stealing mechanism
-  Points calculation based on card types
-  Turn-based loop with game-end scoring

---

# Scoring System

| Card Type | Points  |
|-----------|---------|
| A         | 20 pts  |
| K, Q, J   | 10 pts  |
| 2–10      | 5 pts   |

---

# Compilation & Execution

1. Make sure you have a C++ compiler installed (e.g., g++).
2. Open terminal or PowerShell in the project directory.
3. Compile the code:

   ```bash
   g++ -o card_game main.cpp
