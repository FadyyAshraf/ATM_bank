#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Transaction.h"
#include "util.h"

// Function to load users from CSV
User* load_users_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    User* head = NULL;
    User* temp = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        User* new_user = (User*)malloc(sizeof(User));
        
        // Reading each field (id, username, pin, balance)
        if (sscanf(line, "%d,%49[^,],%49[^,],%lf", 
                &new_user->id, new_user->username, new_user->pin, 
                &new_user->balance) == 4) {

            new_user->next = NULL;

            // Add to linked list
            if (head == NULL) {
                head = new_user;
            } else {
                temp->next = new_user;
            }
            temp = new_user;
        }
    }
    printf("Data loaded\n");
    fclose(file);
    return head;
}

void save_users_to_csv(const char* filename, User* head) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    User* current = head;
    while (current) {
        fprintf(file, "%d,%s,%s,%.2f\n", current->id, current->username, current->pin, current->balance);
        current = current->next;
    }
    fclose(file);
}

User* authenticate_user(User* head, int user_id, char* pin) {
    User* current = head;
    char encrypted_pin[5];  // Encrypted PIN storage (4 digits + null terminator)

    // Encrypt the entered PIN using Hill cipher for 4-digit PINs
    encrypt_hill(pin, encrypted_pin);

    // Loop through the linked list of users and compare the encrypted PIN
    while (current) {
        if (current->id == user_id && strcmp(current->pin, encrypted_pin) == 0) {
            return current;  // Return the user if PINs match
        }
        current = current->next;
    }

    return NULL;  // Return NULL if no match is found
}

void update_user_password(User* head, int user_id, char* new_pin) {
    User* current = head;
    char encrypted_pin[5];  // Encrypted 4-digit PIN with null terminator

    // Encrypt the PIN using Hill cipher for a 4-digit PIN
    encrypt_hill(new_pin, encrypted_pin);

    // Update the encrypted PIN in the user list
    while (current) {
        if (current->id == user_id) {
            strcpy(current->pin, encrypted_pin);  // Store encrypted PIN
            return;
        }
        current = current->next;
    }
}

void display_balance(User* head, int user_id) {
    User* current = head;
    while (current) {
        if (current->id == user_id) {
            printf("Your balance is: %.2f\n", current->balance);
            return;
        }
        current = current->next;
    }
}

void update_balance(User* head, int user_id, double amount, Transaction** transaction) {
    User* current = head;
    while (current) {
        if (current->id == user_id) {
            current->balance += amount;
            record_transaction(transaction, user_id, user_id, "deposit", amount);
            save_users_to_csv("bank_user_info.csv", head);
            save_transactions_to_csv("transaction_history.csv", *transaction);
            printf("Withdrawal successful! New balance: %.2f\n", current->balance);
            return;
        }
        current = current->next;
    }
}

void withdraw(User* head, int user_id, double amount, Transaction** transactions) {
    User* current = head;

    while (current) {
        if (current->id == user_id) {

            printf("Current balance: %.2f, Withdrawal amount: %.2f\n", current->balance, amount);

            if (current->balance >= amount) {
                current->balance -= amount;
                record_transaction(transactions, user_id, user_id, "withdraw", amount);
                save_users_to_csv("bank_user_info.csv", head);
                save_transactions_to_csv("transaction_history.csv", *transactions);

                printf("Withdrawal successful! New balance: %.2f\n", current->balance);
                return;
            } else {
                printf("Insufficient funds for withdrawal! Current balance: %.2f\n", current->balance);
                return;
            }
        }
        current = current->next;
    }

    // If user not found
    printf("User not found!\n");
}
