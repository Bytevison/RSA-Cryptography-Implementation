#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 5000   // Maximum length of ciphertext
#define ALPHABET_SIZE 26     // Number of letters in the alphabet
#define MAPPING_FILE "mappings.txt"  // File containing letter mappings
#define CIPHERTEXT_FILE "ciphertext.txt"  // File containing the encrypted text

// Function to read letter mappings from a file
void load_mappings(char substitution[]) {
    FILE *file = fopen(MAPPING_FILE, "r");
    if (!file) {
        printf("Error: Could not open mappings file (%s)\n", MAPPING_FILE);
        return;
    }

    // Initialize mapping array to identity (A -> A, B -> B, ...)
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        substitution[i] = 'A' + i;
    }

    char original, replacement;
    while (fscanf(file, " %c %c", &original, &replacement) == 2) {
        original = toupper(original);
        replacement = toupper(replacement);
        if (original >= 'A' && original <= 'Z' && replacement >= 'A' && replacement <= 'Z') {
            substitution[original - 'A'] = replacement;
        }
    }

    fclose(file);
}

// Function to read ciphertext from a file
void load_ciphertext(char *ciphertext) {
    FILE *file = fopen(CIPHERTEXT_FILE, "r");
    if (!file) {
        printf("Error: Could not open ciphertext file (%s)\n", CIPHERTEXT_FILE);
        return;
    }

    // Read the entire file content
    size_t length = fread(ciphertext, sizeof(char), MAX_TEXT_SIZE - 1, file);
    ciphertext[length] = '\0'; // Ensure null termination

    fclose(file);
}

// Function to apply letter substitutions to decrypt ciphertext
void decrypt_text(char *ciphertext, char substitution[]) {
    char decrypted[MAX_TEXT_SIZE];  // Buffer for decrypted text
    int i = 0;

    // Process each character in ciphertext
    while (ciphertext[i] != '\0' && i < MAX_TEXT_SIZE - 1) {  // Prevent overflow
        if (isalpha(ciphertext[i])) { // Only decrypt letters
            char original = toupper(ciphertext[i]);
            decrypted[i] = substitution[original - 'A'];
        } else { // Keep spaces and other characters unchanged
            decrypted[i] = ciphertext[i];
        }
        i++;
    }

    decrypted[i] = '\0'; // Ensure null termination

    // Show decryption result
    printf("\nDecrypted Text:\n%s\n", decrypted);
}

int main() {
    char ciphertext[MAX_TEXT_SIZE];
    char substitution[ALPHABET_SIZE];

    // Read letter mappings from file
    load_mappings(substitution);

    // Load ciphertext from file
    load_ciphertext(ciphertext);

    // Check if ciphertext was read successfully
    if (strlen(ciphertext) == 0) {
        printf("Error: No ciphertext found in file.\n");
        return 1;
    }

    // Apply decryption
    decrypt_text(ciphertext, substitution);

    return 0;
}
