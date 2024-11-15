#include "Transaction.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void record_transaction(Transaction** head, int transaction_id, int user_id, const char* type, double amount) {
    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    transaction->transaction_id = transaction_id;
    transaction->user_id = user_id;
    strcpy(transaction->type, type);
    transaction->amount = amount;
    transaction->next = *head;
    *head = transaction;
}

void save_transactions_to_csv(const char* filename, Transaction* head) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    Transaction* current = head;
    while (current) {
        fprintf(file, "%d,%d,%s,%.2f\n", current->transaction_id, current->user_id, current->type, current->amount);
        current = current->next;
    }
    fclose(file);
}

Transaction* load_transactions_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Transaction* head = NULL;
    Transaction* current = NULL;
    while (!feof(file)) {
        Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
        if (fscanf(file, "%d,%d,%9[^,],%lf\n", &transaction->transaction_id, &transaction->user_id, transaction->type, &transaction->amount) != 4) {
            free(transaction);
            break;
        }
        transaction->next = NULL;
        if (!head) head = transaction;
        else current->next = transaction;
        current = transaction;
    }
    fclose(file);
    return head;
}

void print_transactions_for_user(Transaction* head, int user_id) {
    Transaction* current = head;
    while (current) {
        if (current->user_id == user_id) {
            printf("Transaction ID: %d, Type: %s, Amount: %.2f\n", current->transaction_id, current->type, current->amount);
        }
        current = current->next;
    }
}
