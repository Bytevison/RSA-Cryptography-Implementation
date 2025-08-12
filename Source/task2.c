#include <stdio.h>
#include <string.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a) {
    char *number_str = BN_bn2hex(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

BIGNUM* read_key_from_file(const char *filename, const char *key_prefix) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    char line[256];
    BIGNUM *key = BN_new();
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, key_prefix)) {
            char *hex_str = strchr(line, ' ') + 1; // Skip prefix (e.g., "e: ")
            hex_str[strcspn(hex_str, "\n")] = '\0'; // Remove newline
            BN_hex2bn(&key, hex_str);
            break;
        }
    }
    fclose(fp);
    return key;
}

void hex_to_ascii(const char *hex_str, char *output) {
    int len = strlen(hex_str);
    for (int i = 0; i < len; i += 2) {
        int val;
        sscanf(hex_str + i, "%2x", &val);
        output[i/2] = (char)val;
    }
    output[len/2] = '\0';
}

int main() {
    BN_CTX *ctx = BN_CTX_new();

    // Step 1: Read keys from keys.txt
    BIGNUM *n = read_key_from_file("keys.txt", "N: ");
    BIGNUM *e = read_key_from_file("keys.txt", "e: ");
    BIGNUM *d = read_key_from_file("keys.txt", "d: ");

    // Step 2: Convert message "group_SB" to BIGNUM
    const char *msg_ascii = "group_SB";
    char msg_hex[128] = {0};
    for (int i = 0; i < strlen(msg_ascii); i++) {
        sprintf(msg_hex + 2*i, "%02X", msg_ascii[i]);
    }
    BIGNUM *M = BN_new();
    BN_hex2bn(&M, msg_hex);
    printBN("Message as BIGNUM (hex) = ", M); // Original message in hex

    // Step 3: Encrypt C = M^e mod N
    BIGNUM *C = BN_new();
    BN_mod_exp(C, M, e, n, ctx);
    printBN("Encrypted Ciphertext C = ", C); // Encrypted ciphertext

    // Step 4: Decrypt M = C^d mod N
    BIGNUM *M_decrypted = BN_new();
    BN_mod_exp(M_decrypted, C, d, n, ctx);
    printBN("Decrypted Message (hex) = ", M_decrypted); // Decrypted message in hex

    // Step 5: Convert hex back to ASCII
    char *decrypted_hex = BN_bn2hex(M_decrypted);
    char decrypted_ascii[128];
    hex_to_ascii(decrypted_hex, decrypted_ascii);
    printf("Decrypted Message (ASCII) = %s\n", decrypted_ascii); // Final ASCII output

    // Free memory
    BN_free(n);
    BN_free(e);
    BN_free(d);
    BN_free(M);
    BN_free(C);
    BN_free(M_decrypted);
    BN_CTX_free(ctx);
    OPENSSL_free(decrypted_hex);

    return 0;
}
