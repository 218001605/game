#include <iostream>
#include "board.h"
#include "agent.h"

void playGame(agent* player1, agent* player2, const std::string& player1Name, const std::string& player2Name) {
    // Initialize the game board.
    board gameBoard;

    // Get the current state of the game board.
    std::string currentState = gameBoard.operator std::string();

    // Flag to switch between players.
    bool isCurrentPlayer1 = false;

    // Print the initial state of the game board.
    std::cout << "Initial State:\n" << currentState << std::endl;

    // Pointer to the previous movement.
    movement* previousMove = nullptr;

    // Pointer to the current movement.
    movement* currentMove = nullptr;

    // Counter for the total moves made.
    int totalMoves = 1;

    // Get the winner.
    std::string winner;

    // Loop until the game is over.
    while (gameBoard.inPlay() != false) {
        // Get the move made by the current player.
        try {
            // Get the current player.
            agent* currentPlayer = isCurrentPlayer1 ? player1 : player2;

            // Get the move made by the current player.
            movement moveMade = currentPlayer->move(previousMove, gameBoard);

            // Create a new movement object.
            currentMove = new movement(moveMade);

            // Delete the previous movement object.
            //delete previousMove;

            // Set the previous movement object to the current movement object.
            previousMove = currentMove;
        } catch (const std::string& errorMessage) {
            // Print the error message.
            std::cout << '\n' << errorMessage << std::endl;

            // Return from the function.
            return;
        }

        // Get the current player's symbol.
        char currentPlayerSymbol = isCurrentPlayer1 ? player1->getSymbol() : player2->getSymbol();

        // Get the current player's name.
        std::string currentPlayerName = isCurrentPlayer1 ? player1Name : player2Name;

        // Print the move.
        std::cout << "Move " << totalMoves << ": " << currentPlayerSymbol << "," << currentMove->l << "," << currentMove->n << "," << currentMove->fd << "," << currentMove->md << " -> (" << currentPlayerName << ")" << std::endl;

        // Execute the move.
        bool isValidMove = gameBoard.executeMove(currentPlayerSymbol, currentMove->l, currentMove->n, currentMove->fd, currentMove->md);

        // If the move is valid, update the current state of the game board.
        if (isValidMove) {
            currentState = gameBoard.operator std::string();
            cout<<currentState <<endl;
        } else {
            // If the move is invalid, print an error message.
            std::cout << "Invalid move!!" << std::endl;
            std::cout << "Enter again"<<std::endl;
            continue;
        }
        // Switch players.
        isCurrentPlayer1 = !isCurrentPlayer1;

        // Delete the current movement object.
        delete currentMove;

        // Refresh the offboard counts and get the winner.
        gameBoard.refreshOffboardCounts(winner, player1Name, player2Name);

        // Increment the total moves made.
        totalMoves++;
    }

    // Delete the previous movement object.
    delete previousMove;

    // Delete the player objects.
    delete player1;
    delete player2;

    // Print the final state of the game board.
    std::cout << "Final State: " << std::endl << currentState << std::endl;

    // Print the winner.
    std::cout << winner << " is the winner" << std::endl;

    // Print the number of moves made.
    std::cout << "After " << totalMoves - 1 << " moves were made" << std::endl;
}


// Play a game between two human players
void playHumanGame() {
    // Create player objects for player 1 and player 2
    // Player 1 is a human player represented by '@'
    agent& player1 = *new Human('@');
    // Player 2 is a human player represented by 'O'
    agent& player2 = *new Human('O');

    // Variables to store the names of player 1 and player 2
    string player1Name;
    string player2Name;

    // Get player names from the user
    cout << "Enter player 1 name: ";
    cin >> player1Name;

    cout << "Enter player 2 name: ";
    cin >> player2Name;

    // Print the player symbols and names
    cout << "Player 1 @\nPlayer 2 O\n";

    // Start the game between player 1 and player 2
    playGame(&player1, &player2, player1Name, player2Name);
}

// Observe a game between two computer players
void observe() {
    // Create player objects for player 1 and player 2
    // Player 1 is a computer player with random moves, represented by 'O'
    agent& player1 = *new randAgent('O');
    // Player 2 is a computer player with strategic moves, represented by '@'
    agent& player2 = *new randAgent('@');

    // Start the game between player 1 and player 2
    // Use default names for the computer players
    playGame(&player1, &player2, "COM 1", "COM 2");
}

// Play a game against a computer player
void playWithCom() {
    // Create player objects for player 1 and player 2
    // Player 1 is a human player represented by '@'
    agent& player1 = *new Human('@');
    // Player 2 is a computer player with random moves, represented by 'O'
    agent& player2 = *new distAgent('O');

    // Get the name of player 1 from the user
    string player1Name;
    cout << "Enter player 1 name: ";
    cin >> player1Name;

    // Print the player symbol for player 1 and indicate it's their turn to start
    cout << "Player 1 @\nPlayer 2 O\n";
    cout << "Player 1 start\n";

    // Start the game between player 1 and player 2
    // Use player 1's name and default name for the computer player
    playGame(&player1, &player2, player1Name, "COM 2");
}

int main() {
    char play;
    cout<< "Enter the following keys to play" << endl;
    cout << "C - to play with a computer"<< endl;
    cout << "P - to player with another human" << endl;
    cout << "O - to observe a game between com vs com hard|easy." << endl;
    cout << "Any key to exit." << endl;
    cin >> play;
    switch (play) {
        case 'C' : case 'c' :{
            playWithCom();
        }
        case 'P' : case 'p' :{
            playHumanGame();
        }
        case 'O': case 'o':{
            observe();
        }
        default: return 1;
    }
}