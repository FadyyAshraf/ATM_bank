#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// 2x2 Key Matrix for encryption
int key_matrix[2][2] = {
    {3, 3},
    {2, 5}
};

// Function to encrypt a 4-digit numeric PIN using Hill Cipher
void encrypt_hill(const char* input, char* output) {
    int input_matrix[2][1];
    int encrypted_matrix[2][1];

    // Encrypt first two digits
    for (int i = 0; i < 2; i++) {
        input_matrix[i][0] = input[i] - '0';  // Convert char to int
    }

    for (int i = 0; i < 2; i++) {
        encrypted_matrix[i][0] = 0;
        for (int j = 0; j < 2; j++) {
            encrypted_matrix[i][0] += key_matrix[i][j] * input_matrix[j][0];
        }
        encrypted_matrix[i][0] %= 10;  // Keep within single digit
    }

    // Store encrypted first two digits in output
    output[0] = encrypted_matrix[0][0] + '0';
    output[1] = encrypted_matrix[1][0] + '0';

    // Encrypt next two digits
    for (int i = 0; i < 2; i++) {
        input_matrix[i][0] = input[i + 2] - '0';
    }

    for (int i = 0; i < 2; i++) {
        encrypted_matrix[i][0] = 0;
        for (int j = 0; j < 2; j++) {
            encrypted_matrix[i][0] += key_matrix[i][j] * input_matrix[j][0];
        }
        encrypted_matrix[i][0] %= 10;
    }

    // Store encrypted last two digits in output
    output[2] = encrypted_matrix[0][0] + '0';
    output[3] = encrypted_matrix[1][0] + '0';
}
