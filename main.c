#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

#define MAX_GUESSES 6
#define WORD_LENGTH 5


// Check to see if the guess is in the file
int check_word_exists (const char guess[], const char dictionary[])
{
    int i = 0; // initialize a counter variable
    while (guess[i] != '\0' && dictionary[i] != '\0') // loop through the guess and dictionary arrays until one of them ends
    {
        if (guess[i] != dictionary[i]) // if the current characters don't match
        {
            return 0; // return false
        }
        i++; // increment the counter variable
    }
    return 1; // if we've looped through the entire arrays and all characters matched, return true
}

// Check to see if the guess matches the random word
int check_match (const char five_letter_word[], const char guess[])
{
    int i = 0; // initialize a counter variable
    while (guess[i] != '\0' && five_letter_word[i] != '\0') // loop through the guess and five_letter_word arrays until one of them ends
    {
        if (guess[i] != five_letter_word[i]) // if the current characters don't match
        {
            return 0; // return false
        }
        i++; // increment the counter variable
    }
    return 1; // if we've looped through the entire arrays and all characters matched, return true
}

// Prints the guess with the correct letter positions
void print_word_with_guesses (char *guess, const char *five_letter_word, const char correct_letters[])
{
    for (int i = 0; i < WORD_LENGTH; i++) // loop through each character in the guess array
    {
        if (guess[i] == five_letter_word[i]) // if the current character in guess matches the current character in five_letter_word
        {
            printf ("%c ", guess[i]); // print the current character in the guess
        }
        else // if the current character in guess does not match the current character in five_letter_word
        {
            int flag = 0; // initialize a flag variable
            for (int j = 0; j < WORD_LENGTH; j++) // loop through each character in the correct_letters array
            {
                if (guess[i] == correct_letters[j]) // if the current character in guess matches the current character in correct_letters
                {
                    printf ("%c ", guess[i]); // print the current character in the guess
                    flag = 1; // set the flag variable to true
                    break; // exit the loop
                }
            }
            if (flag == 0) // if no match was found
            {
                printf ("- "); // print a dash to indicate an incorrect letter
            }
        }
    }
    printf ("\n"); // print a newline character at the end
}

// Checks to see if any letters in the guess are in the random word
void print_correct_letters (const char *guess, const char *five_letter_word, char correct_letters[])
{
    int num_common_letters = 0; // initialize a variable to keep track of the number of common letters between guess and five_letter_word
    for (int i = 0; i < WORD_LENGTH; i++) // loop through each character in the guess array
    {
        int already_added = 0; // initialize a flag variable to keep track of whether the current character in guess has already been added to correct_letters
        for (int j = 0; j < WORD_LENGTH; j++) // loop through each character in the five_letter_word array
            if (guess[i] == five_letter_word[j]) // if the current character in guess matches the current character in five_letter_word
            {
                for (int k = 0; k < num_common_letters; k++) // loop through each character in the correct_letters array that has already been added
                {
                    if (guess[i] == correct_letters[k]) // if the current character in guess has already been added to correct_letters
                    {
                        already_added = 1; // set the flag variable to true
                        break; // exit the loop
                    }
                }
                if (!already_added) // if the current character in guess has not already been added to correct_letters
                {
                    correct_letters[num_common_letters] = guess[i]; // add the current character in guess to the correct_letters array
                    num_common_letters++; // increment the count of common letters
                }
                correct_letters[num_common_letters] = '\0'; // add a null terminator to the end of the correct_letters array
                break; // exit the loop
            }
    }
    printf ("The correct letters were: %s\n", correct_letters); // print the correct_letters array
    printf ("\n"); // print a newline at the end
}

// Function prints out the statistics report
void stats (int games_won, int games_lost, int num_games, int num_characters, int num_guesses)
{
    // Calculate the win percentage and percentage of the alphabet used
    float win_percent = ((float) games_won / (float) num_games * 100);
    float alphabet_percent = ((float) num_characters / 26 * 100);

    // Print the game results and statistics
    printf ("Won: %d\n", games_won);
    printf ("Lost: %d\n", games_lost);
    printf ("Win Percentage: %.2f%%\n", win_percent);
    printf ("Alphabet Percentage: %.2f%%\n", alphabet_percent);

    // Create a histogram array to store the number of guesses it took to win the game
    int histogram[MAX_GUESSES + 1] = { 0 };
    histogram[num_guesses]++;

    // Print the histogram of guesses
    printf ("\nHistogram of guesses:\n");
    for (int i = 1; i <= MAX_GUESSES; i++)
    {
        printf ("%d guesses: ", i);
        for (int j = 0; j < histogram[i]; j++)
        {
            printf ("*");
        }
        printf ("\n");
    }
    printf ("\n");
}

// Function to start the game
void play_easy_game (int *games_won, int *games_lost, int *num_games, int *num_characters, char *characters_used[])
{
    // Loads a random word into five_letter_word
    char dictionary[DICT_SIZE][WORD_LENGTH + 1];
    load_word_list (dictionary);
    srand (time (NULL)); // Initializes the random seed using the current time
    int random_index = rand () % DICT_SIZE; // Generates a random index between 0 and the size of the dictionary
    char five_letter_word[WORD_LENGTH + 1]; // Initializes a character array to store the five-letter word
    for (int i = 0; dictionary[random_index][i] != '\0'; i++) // Copies the random word from the dictionary to five_letter_word
    {
        five_letter_word[i] = dictionary[random_index][i];
    }
    five_letter_word[WORD_LENGTH] = '\0'; // Adds a null terminator to the end of the word

    printf ("The random word is: %s\n\n", five_letter_word); // Prints the random word

    char guess[WORD_LENGTH + 1] = ""; // Initializes a character array to store the user's guess
    int num_guesses = 0, game_won = 0; // Initializes variables to track the number of guesses and whether the game has been won

    // Asks the user for their guess and continues until the user runs out of guesses or correctly guesses the word
    while (num_guesses < MAX_GUESSES && !game_won)
    {
        printf ("Guess the word (%d guesses left): ", MAX_GUESSES - num_guesses);
        scanf ("%s", guess);
        for (int i = 0; guess[i]; i++)
        {
            guess[i] = (char) tolower (guess[i]); // Converts the guess to lowercase
        }

        // Keeps track of the number of letters used and which letters
        for (int i = 0; i < WORD_LENGTH; i++)
        {
            int already_added = 0;
            for (int j = 0; j < *num_characters; j++)
            {
                if (guess[i] == (*characters_used)[j])
                {
                    already_added = 1;
                    break;
                }
            }
            if (!already_added)
            {
                (*characters_used)[*num_characters] = guess[i];
                (*num_characters)++;
            }
        }

        for (int i = 0; i < WORD_LENGTH; i++)
        {

            // Check if guess is alphanumeric
            if (!isalpha (guess[i])) // Checks if the guess contains only letters
            {
                printf ("Guess must only include letters. Try again.\n");
                break;
            }
                // Check length of guess
            else if (guess[WORD_LENGTH] != '\0') // Checks if the guess is exactly five letters long
            {
                printf ("Guess must be 5 letters long. Try again.\n");
                break;
            }
                // Check if word exists in file
            else
            {
                int word_included = 0;
                for (i = 0; i < DICT_SIZE; i++)
                    if (check_word_exists (guess, dictionary[i])) // Checks if the guess exists in the dictionary
                    {
                        word_included = 1;
                        break;
                    }
                if (!word_included)
                {
                    printf ("Guess doesn't exist in the file. Try again.\n");
                    continue;
                }
            }
            // Check if guess matches
            if (check_match (guess, five_letter_word)) // checks to see if the guess matches the five_letter_word
            {
                printf ("Correct! The word was %s\n", five_letter_word); // prints a congratulatory message if guess is right
                game_won = 1; // set the game_won flag to 1 to break out of the while loop
                num_guesses++; // increments the number of guesses
                (*games_won)++; // increments the number of games won
                (*num_games)++; // increments the number of games played
                printf ("\n"); // prints a newline character
                break; // exits the loop
            }
            else // if the guess isn't correct but is a valid guess
            {
                // Formats the guess, revealing correct letters and positions
                char correct_letters[WORD_LENGTH * 2 + 1] = "";
                print_word_with_guesses (guess, five_letter_word, correct_letters);
                print_correct_letters (guess, five_letter_word, correct_letters);
                num_guesses++;

                // Prints failure message
                if (num_guesses == MAX_GUESSES)
                {
                    printf ("Incorrect. The word was %s. \n", five_letter_word);
                    printf ("\n");
                    (*games_lost)++;
                    (*num_games)++;

                }
            }
        }
    }
    // Calls the stats function to print the statistics report
    stats (*games_won, *games_lost, *num_games, *num_characters, num_guesses);
}

// Function to start the game
void play_hard_game(int *games_won, int *games_lost, int *num_games, int *num_characters, char *characters_used[]) {
    char dictionary[DICT_SIZE][WORD_LENGTH + 1];
    load_word_list(dictionary); // Load the word list from file into the dictionary array
    // Choose a random index in the dictionary and select a five letter word
    srand(time(NULL));
    int random_index = rand() % DICT_SIZE;

    char five_letter_word[WORD_LENGTH + 1];
    for (int i = 0; dictionary[random_index][i] != '\0'; i++) { // copies the random word tp the five_letter_word variable
        five_letter_word[i] = dictionary[random_index][i];
    }
    five_letter_word[WORD_LENGTH] = '\0';

    printf ("The random word is: %s\n\n", five_letter_word); // prints the random word

    // Initialize variables for user's guess and game stats
    char guess[WORD_LENGTH + 1] = "";
    char correct_letters[WORD_LENGTH + 1] = "";
    int num_guesses = 0, game_won = 0;
    int guessed_positions[WORD_LENGTH] = {0};
    int guessed_letters[26] = {0};

    // Loop until user wins or runs out of guesses
    while (num_guesses < MAX_GUESSES && !game_won) {
        // Prompt user to enter a guess
        printf("Guess the word (%d guesses left): ", MAX_GUESSES - num_guesses);
        scanf("%s", guess);
        int valid_guess = 1;

        // Check if the guess is valid and provide feedback to the user
        for (int i = 0; guess[i] != '\0'; i++) {
            int letter_index = guess[i] - 'a';
            if (guessed_letters[letter_index] && !guessed_positions[i]) {
                printf("You already guessed the letter %c\n", guess[i]);
                valid_guess = 0;
                break;
            } else if (guessed_positions[i] && guess[i] != five_letter_word[i]) {
                printf("You already guessed the letter %c in a different position\n", guess[i]);
                valid_guess = 0;
                break;
            } else if (guessed_positions[i] && guess[i] == five_letter_word[i]) {
                continue;
            } else if (guess[i] != five_letter_word[i]) {
                int found = 0;
                for (int j = 0; j < sizeof(correct_letters) / sizeof(correct_letters[0]); j++) {
                    if (guess[i] == correct_letters[j]) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("The letter %c is not correct\n", guess[i]);
                    valid_guess = 0;
                    num_guesses++;
                    break;
                }
            }
        }
        // If the guess is not valid, restart the loop
        if (!valid_guess) {
            continue;
        }

        // Update arrays to keep track of the guessed letters and positions
        for (int i = 0; guess[i] != '\0'; i++) {
            int letter_index = guess[i] - 'a';
            guessed_letters[letter_index] = 1;
            if (guess[i] == five_letter_word[i]) {
                guessed_positions[i] = 1;
            }
        }
        // Check if the user has guessed the correct word and update game stats
        if (check_match(five_letter_word, guess)) {
            game_won = 1;
            printf("Congratulations, you won!\n");
            num_guesses++;
            (*games_won)++;
        } else {
            printf("Incorrect, try again\n");
            print_word_with_guesses(guess, five_letter_word, correct_letters);
            print_correct_letters(guess, five_letter_word, correct_letters);
            num_guesses++;
        }
    }

    if (!game_won) {
        printf("You lost, the word was: %s\n", five_letter_word);
        (*games_lost)++;
    }

    (*num_games)++;
    int count = 0;
    while (characters_used[count] != (void *)0) {
        count++;
    }
    *num_characters += count;

    stats(*games_won, *games_lost, *num_games, *num_characters, num_guesses);
}

// Function to see if they want to play another game
void play_again (int *games_won, int *games_lost, int *num_games, int *num_characters, char *characters_used[],
            char difficulty)
{
    while (1) // loops until the user enters 'y'
    {
        printf ("Do you want to play again? (y/n): "); // prints a message asking if they want to play again
        char another_game; // initialize character to store the play again preference
        scanf (" %c", &another_game); // takes the input from the user
        if (tolower (another_game) == 'y') // if the user inputs 'y'
        {
            printf ("\n");
            // calls the play_game function and determines the difficulty they already had selected
            if (tolower (difficulty) == 'e')
            {
                play_easy_game (games_won, games_lost, num_games, num_characters, characters_used);
            }
            else if (tolower (difficulty) == 'h')
            {
                play_hard_game (games_won, games_lost, num_games, num_characters, characters_used);
            }
        }
        else // if the user doesn't enter 'y'
        {
            break; // exits the loop
        }
    }
}

int main ()
{
    int games_won = 0, games_lost = 0, num_games = 0, num_characters = 0;
    char difficulty;
    char *characters_used = (char *) malloc (26 * sizeof (char));
    printf ("Choose difficulty (Easy 'e' / Hard 'h'): ");
    scanf (" %c", &difficulty);
    if (tolower (difficulty) == 'e')
    {
        play_easy_game (&games_won, &games_lost, &num_games, &num_characters, &characters_used);
        play_again (&games_won, &games_lost, &num_games, &num_characters, &characters_used, difficulty);
    }
    else if (tolower (difficulty == 'h'))
    {
        play_hard_game (&games_won, &games_lost, &num_games, &num_characters, &characters_used);
        play_again (&games_won, &games_lost, &num_games, &num_characters, &characters_used, difficulty);
    }
}
