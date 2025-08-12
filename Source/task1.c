#include <stdio.h>
#include <openssl/bn.h>

#define NBITS 256

void printBN(char *msg, BIGNUM *a) {
    char *number_str = BN_bn2hex(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main() {
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *n = BN_new();
    BIGNUM *phi = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *p_minus_1 = BN_new();
    BIGNUM *q_minus_1 = BN_new();

    // Generate 256-bit primes p and q
    BN_generate_prime_ex(p, NBITS, 1, NULL, NULL, NULL);
    BN_generate_prime_ex(q, NBITS, 1, NULL, NULL, NULL);
    printBN("Generated prime p = ", p);
    printBN("Generated prime q = ", q);

    // Compute n = p*q
    BN_mul(n, p, q, ctx);
    printBN("Modulus N = ", n);

    // Compute φ(n) = (p-1)*(q-1)
    BN_sub(p_minus_1, p, BN_value_one());
    BN_sub(q_minus_1, q, BN_value_one());
    BN_mul(phi, p_minus_1, q_minus_1, ctx);
    printBN("Euler's Totient φ(N) = ", phi);

    // Set public exponent e = 65537
    BN_hex2bn(&e, "010001");
    printBN("Public exponent e = ", e);

    // Compute private exponent d = e⁻¹ mod φ(n)
    BN_mod_inverse(d, e, phi, ctx);
    printBN("Private exponent d = ", d);

    // Write keys to keys.txt
    FILE *fp = fopen("keys.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(fp, "Public Key:\ne: ");
    char *e_str = BN_bn2hex(e);
    fprintf(fp, "%s\nN: ", e_str);
    OPENSSL_free(e_str);
    char *n_str = BN_bn2hex(n);
    fprintf(fp, "%s\nPrivate Key:\nd: ", n_str);
    OPENSSL_free(n_str);
    char *d_str = BN_bn2hex(d);
    fprintf(fp, "%s\nN: %s\n", d_str, n_str);
    OPENSSL_free(d_str);
    fclose(fp);

    // Free memory
    BN_free(p);
    BN_free(q);
    BN_free(n);
    BN_free(phi);
    BN_free(e);
    BN_free(d);
    BN_free(p_minus_1);
    BN_free(q_minus_1);
    BN_CTX_free(ctx);

    return 0;
}
