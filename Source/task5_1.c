
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 5000  // Maximum allowed text length
#define ALPHABET_SIZE 26    // Number of English letters

// Struct to store letter frequencies
typedef struct {
    char letter;
    int count;
    double percentage;
} LetterFrequency;

// Function to calculate and display letter frequencies in descending order
void calculate_frequencies(char *text) {
    int letter_count[ALPHABET_SIZE] = {0};
    int total_letters = 0;
    LetterFrequency freq[ALPHABET_SIZE];

    // Count occurrences of each letter
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = toupper(text[i]);  // Convert to uppercase for uniformity
        if (ch >= 'A' && ch <= 'Z') {
            letter_count[ch - 'A']++;  
            total_letters++;
        }
    }

    // Store frequencies in array
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i].letter = 'A' + i;
        freq[i].count = letter_count[i];
        freq[i].percentage = (total_letters > 0) ? ((double)letter_count[i] / total_letters) * 100 : 0.0;
    }

    // Sort frequencies in descending order using Bubble Sort
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (freq[j].count < freq[j + 1].count) {
                LetterFrequency temp = freq[j];
                freq[j] = freq[j + 1];
                freq[j + 1] = temp;
            }
        }
    }

    // Display results
    printf("\nLetter Frequency Analysis (Sorted Descending):\n");
    printf("----------------------------------------------\n");
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (freq[i].count > 0) {
            printf("%c: %d (%.2f%%)\n", freq[i].letter, freq[i].count, freq[i].percentage);
        }
    }
}

int main() {
    char ciphertext[MAX_TEXT_SIZE];
    FILE *file = fopen("ciphertext.txt", "r");
    
    if (file == NULL) {
        printf("Error: Unable to open file ciphertext.txt\n");
        return 1;
    }
    
    // Read ciphertext from file
    fread(ciphertext, sizeof(char), MAX_TEXT_SIZE - 1, file);
    ciphertext[MAX_TEXT_SIZE - 1] = '\0'; // Ensure null-termination
    fclose(file);
    
    // Calculate and display frequencies in descending order
    calculate_frequencies(ciphertext);

    return 0;
}
