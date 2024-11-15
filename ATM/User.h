#ifndef USER_H
#define USER_H

#include <stdio.h>
#include "Transaction.h"  // Include for transaction handling
#include "util.h"          // Include for encryption functions

// Define the User struct with linked list capability
typedef struct User {
    int id;
    char username[50];
    char pin[50];         // Updated to represent encrypted PIN storage
    double balance;
    struct User* next;    // Linked list pointer
} User;

// Function declarations

// Load users from CSV file and initialize linked list
User* load_users_from_csv(const char* filename);

// Save updated user data to CSV file
void save_users_to_csv(const char* filename, User* head);

// Authenticate user by encrypting input PIN and comparing with stored encrypted PIN
User* authenticate_user(User* head, int user_id, char* pin);

// Update user's PIN after encryption and save to list
void update_user_password(User* head, int user_id, char* new_pin);

// Display current account balance
void display_balance(User* head, int user_id);

// Update account balance (used for deposit or withdrawal)
void update_balance(User* head, int user_id, double amount, Transaction** transactions);

// Handle withdrawal operation and record transaction
void withdraw(User* head, int user_id, double amount, Transaction** transactions);

#endif
