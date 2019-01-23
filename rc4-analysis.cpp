/*/
Analysis of RC-4 Stream Cipher by Stephen Creel

Analyzes the first n bytes of RC-4 output stream to identify deviation from a
purely random stream.

Instructions for Assignment:

Implement a function frc4(int IV, int n, int* out) which takes a 32 bit IV as
input and outputs first n bytes of the key stream. Internal to the function
frc4() use your own fixed sequence of 12 bytes as key. The 16-byte seed  used
by RC4 is now 16 bytes (12 bytes key + 4 bytes IV).

Initialize a 2-D array count[100][256] with all zeros.

Call frc4(IV, n,ptr) 256 million times (with different IV input each time.)

Depending on the output increment the corresponding count values.
/*/

#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

// Initialize Mersenne Twister Random Number Generator
std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

/*/ Return Random Number in Range ( min , max ) /*/
int randomNum(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

/*/ Generate a 32-bit Initial Value Stored as an Array /*/
int * generateIV() {
    static int IV[32];
    for(int i = 0; i < 32; ++i) IV[i] = randomNum(0, 1);
    return IV;
}

/*/ RC-4 Cryptographic Algorithm /*/
void frc4(int * IV, int n, int * out) {

    // Initialize Fixed Sequence Key
    int key[96] = {1, 1, 0, 0, 1, 0, 1, 0, 1, 1,
                    0, 1, 0, 0, 1, 1, 1, 0, 0, 1,
                    0, 1, 1, 0, 1, 0, 1, 0, 0, 0,
                    0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
                    1, 1, 0, 0, 1, 1, 1, 0, 0, 1,
                    0, 1, 0, 1, 1, 0, 1, 1, 1, 0,
                    0, 1, 0, 0, 1, 1, 1, 0, 0, 1,
                    1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
                    0, 1, 0, 1, 1, 0, 1, 1, 1, 0,
                    0, 1, 0, 0, 1, 0};

    // Populate Seed with Key and Initial Value
    int seed[n];
    for(int j = 0; j < 96; ++j) seed[j] = key[j];
    for(int j = 0; j < 32; ++j) seed[j + 96] = *(IV + j);

    // Key-Scheduling Algorithm
    int S[256], J = 0;
    for(int i = 0; i < 256; ++i) S[i] = i;
    for(int i = 0; i < 256; ++i) {
        J = (J + S[i] + seed[i % 128]) % 256;
        std::swap(S[i], S[J]);
    }

    // Pseudo-Random Generation Algorithm
    int I = 0; J = 0;
    for(int i = 0; i < n; ++i) {
        I = (I + 1) % 256;
        J = (J + S[I]) % 256;
        std::swap(S[I], S[J]);

        // Append Stream Integer to Output
        out[i] = S[(S[I] + S[J]) % 256];
    }
}

int main() {

    int streamNum = 100;
    unsigned int attempts = 256000000;

    // Initialize Counter Array
    int count[streamNum][256];
    for(int i = 0; i < streamNum; ++i)
        for(int j = 0; j < 256; ++j) count[i][j] = 0;

    // Run Attempts
    for(unsigned int i = 0; i < attempts; ++i) {
        std::cout << "Running attempt " << i + 1 << std::endl;

        // Generate RC-4 Stream
        int stream[streamNum];
        frc4(generateIV(), streamNum, stream);

        // Adjust Counter for New Stream
        for(int i = 0; i < streamNum; ++i) ++count[i][stream[i]];
    }

    // Output Counter Data
    for(int i = 0; i < 100; ++i) {
            std::cout << "\n\n----------\n--- INDEX " << i << "----------\n\n";
        for(int j = 0; j < 256; ++j) {
            std::cout << "[ " << j << ": " << count[i][j] << " ] ";
        }
    }
    return 0;
}
