#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>

typedef struct Transaction {
    int transaction_id;
    int user_id;
    char type[10];  // deposit or withdraw
    double amount;
    struct Transaction* next;  // Linked list pointer
} Transaction;

// Function declarations
void record_transaction(Transaction** head, int transaction_id, int user_id, const char* type, double amount);
void save_transactions_to_csv(const char* filename, Transaction* head);
Transaction* load_transactions_from_csv(const char* filename);
void print_transactions_for_user(Transaction* head, int user_id);

#endif
