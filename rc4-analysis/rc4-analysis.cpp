/*/
Analysis of RC-4 Stream Cipher by Stephen Creel

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
/*/

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

// Initialize Mersenne Twister Random Number Generator
std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

/*/ Return Random Number in Range ( min , max ) /*/
int randomNum(int min, int max) {
    std::uniform_int_distribution<int> ran(min, max);
    return ran(rng);
}

/*/ Generate a 32-bit Initial Value Stored as an Array /*/
unsigned int * generateIV() {
    static unsigned int IV[32];
    for(int i = 0; i < 32; ++i) IV[i] = randomNum(0, 1);
    return IV;
}

/*/ RC-4 Cryptographic Algorithm /*/
void frc4(unsigned int * IV, unsigned int n, unsigned int * out) {

    // Initialize Fixed Sequence Key
    unsigned int key[96] = {1, 1, 0, 0, 1, 0, 1, 0, 1, 1,
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
    unsigned int seed[128];
    for(int j = 0; j < 32; ++j) seed[j] = *(IV + j);
    for(int j = 0; j < 96; ++j) seed[j + 32] = key[j];

    // Key-Scheduling Algorithm
    int S[256], J = 0, I = 0;
    for(int i = 0; i < 256; ++i) S[i] = i;
    for(int i = 0; i < 256; ++i) {
        J = (J + S[i] + seed[i % 128]) % 256;
        std::swap(S[i], S[J]);
    }

    // Pseudo-Random Generation Algorithm
    J = 0;
    for(int i = 0; i < n; ++i) {
        I = (I + 1) % 256;
        J = (J + S[I]) % 256;
        std::swap(S[I], S[J]);

        // Append Stream Integer to Output
        out[i] = S[(S[I] + S[J]) % 256];
    }
}

int main() {

    unsigned int streamNum = 100;
    unsigned int attempts = 256000000;

    // Initialize Counter Array
    unsigned int count[100][256];
    for(int i = 0; i < 100; ++i)
        for(int j = 0; j < 256; ++j) count[i][j] = 0;

    // Run Attempts
    std::cout << "Running " << attempts << " attempts of RC-4 Analysis." << std::endl;
    unsigned int run = 1;
    for(unsigned int i = 0; i < attempts; ++i) {
        
        // Track and Output Attempts
        if(i + 1 == run * 1000000) {
            std::cout << run << " million attempts done." << std::endl;
            ++run;
        }
        
        // Generate RC-4 Stream
        unsigned int stream[streamNum];
        frc4(generateIV(), streamNum, stream);

        // Adjust Counter for New Stream
        for(int i = 0; i < streamNum; ++i) ++count[i][stream[i]];
    }

    // Output Counter Data to File
    std::cout << "Outputting to File." << std::endl;
    std::ofstream outFile;
    outFile.open ("rc4-analysis-output.txt");
    for(int i = 0; i < 100; ++i) {
            outFile << "\nINDEX " << i << std::endl;
        for(int j = 0; j < 256; ++j) {
            outFile << j << ": " << count[i][j] << "\t\t";
            if(j % 4 == 3) outFile << std::endl;
        }
    }
    return 0;
}
