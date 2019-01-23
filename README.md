# Cryptography

Algorithms and implementations for CSE-4990-Cryptography

# RC-4 Analysis

Analyzes the first 100 bytes of RC-4 output stream to identify deviation from a
purely random stream.

Instructions for Assignment:

Implement a function frc4(int IV, int n, int* out) which takes a 32 bit IV as
input and outputs first n bytes of the key stream. Internal to the function
frc4() use your own fixed sequence of 12 bytes as key. The 16-byte seed  used
by RC4 is now 16 bytes (12 bytes key + 4 bytes IV).

Initialize a 2-D array count[100][256] with all zeros.

Call frc4(IV, n,ptr) 256 million times (with different IV input each time.)

Depending on the output increment the corresponding count values.
