# RSA-Cryptography-Implementation

"Implementation of RSA key generation, encryption, and digital signatures in C with OpenSSL."



A C program demonstrating \*\*RSA key generation\*\*, encryption/decryption, and digital signatures using OpenSSL. Developed for an M.Sc. module in Cybersecurity.





\##Features

\## Features

\- ✅ RSA key pair generation (`p`, `q`, `φ(n)`, `e`, `d`).

\- 🔒 Message encryption/decryption (e.g., `group\_SB`).

\- 📜 Digital signing/verification (OpenSSL).

\- 🔍 Frequency analysis (substitution cipher cracking).



\## How to Run

1\. Compile C files with OpenSSL:

&nbsp;  ```bash

&nbsp;  gcc task1.c -o task1 -lcrypto

&nbsp;  ./task1

2\. For signing/verification (task3.c, task4.c):

&nbsp;	gcc task3.c -o task3 -lcrypto ./task3

