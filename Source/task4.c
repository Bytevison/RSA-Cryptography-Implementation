#include <openssl/bn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* bn_to_ascii(BIGNUM* bn) {
    char* hex = BN_bn2hex(bn);
    size_t hex_len = strlen(hex);
    char* msg = malloc(hex_len / 2 + 1);
    for (size_t i = 0; i < hex_len / 2; i++) {
        sscanf(hex + 2*i, "%2hhx", &msg[i]);
    }
    msg[hex_len / 2] = '\0';
    OPENSSL_free(hex);
    return msg;
}

int main() {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* N = BN_new();
    BIGNUM* e = BN_new();
    BIGNUM* S = BN_new();
    BIGNUM* M_prime = BN_new();
    char n_hex[1024];
    char e_hex[1024];
    char s_hex[1024];
    char message[1024];

    // Step 1: Read inputs
    printf("Enter modulus (N) in hex: ");
    fgets(n_hex, sizeof(n_hex), stdin);
    n_hex[strcspn(n_hex, "\n")] = '\0';
    BN_hex2bn(&N, n_hex);

    printf("Enter public exponent (e) in hex: ");
    fgets(e_hex, sizeof(e_hex), stdin);
    e_hex[strcspn(e_hex, "\n")] = '\0';
    BN_hex2bn(&e, e_hex);

    printf("Enter signature (S) in hex: ");
    fgets(s_hex, sizeof(s_hex), stdin);
    s_hex[strcspn(s_hex, "\n")] = '\0';
    BN_hex2bn(&S, s_hex);

    /*printf("Enter the original message (M): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';*/

    // Step 2: Verify the signature
    BN_mod_exp(M_prime, S, e, N, ctx);
    char* hex_mprime = BN_bn2hex(M_prime);
    char* recovered_message = bn_to_ascii(M_prime);
    printf("Recomputed Message (M') = %s\n", hex_mprime);
    printf("Decrypted Message (ASCII) = %s\n", recovered_message);

    /*if (strcmp(recovered_message, message) == 0) {
        printf("Signature is valid.\n");
    } else {
        printf("Signature is invalid.\n");
    }*/

    // Cleanup
    OPENSSL_free(hex_mprime);
    free(recovered_message);
    BN_free(N);
    BN_free(e);
    BN_free(S);
    BN_free(M_prime);
    BN_CTX_free(ctx);

    return 0;
}
