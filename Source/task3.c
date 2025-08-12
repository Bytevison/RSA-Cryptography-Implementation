#include <openssl/bn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_hex(const char* label, BIGNUM* bn) {
    char* hex = BN_bn2hex(bn);
    printf("%s %s\n", label, hex);
    OPENSSL_free(hex);
}

BIGNUM* convert_message_to_bignum(const char* message) {
    size_t len = strlen(message);
    char* hex_str = malloc(len * 2 + 1);
    for (size_t i = 0; i < len; i++) {
        sprintf(hex_str + i * 2, "%02X", (unsigned char)message[i]);
    }
    BIGNUM* bn = BN_new();
    BN_hex2bn(&bn, hex_str);
    free(hex_str);
    return bn;
}

int main() {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* N = BN_new();
    BIGNUM* d = BN_new();
    BIGNUM* M = NULL;
    BIGNUM* S = BN_new();
    char n_hex[1024];
    char d_hex[1024];
    char message[1024];

    // Step 1: Read inputs
    printf("Enter modulus (N) in hex: ");
    fgets(n_hex, sizeof(n_hex), stdin);
    n_hex[strcspn(n_hex, "\n")] = '\0';
    BN_hex2bn(&N, n_hex);

    printf("Enter private exponent (d) in hex: ");
    fgets(d_hex, sizeof(d_hex), stdin);
    d_hex[strcspn(d_hex, "\n")] = '\0';
    BN_hex2bn(&d, d_hex);

    // Step 2: Original message
    printf("Enter the message to sign: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    // Convert original message to BIGNUM
    M = convert_message_to_bignum(message);
    print_hex("Message as BIGMMU (hex):", M);

    // Step 3: Generate signature
    BN_mod_exp(S, M, d, N, ctx);
    print_hex("Generated Signature S =", S);

    // Cleanup
    BN_free(N);
    BN_free(d);
    BN_free(M);
    BN_free(S);
    BN_CTX_free(ctx);

    return 0;
}
