#include "User.h"
#include "Transaction.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int login(User *users)
{
    int user_id;
    char password[50];

    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    // Check if user exists
    User *user = NULL;
    for (User *current = users; current != NULL; current = current->next)
    {
        if (current->id == user_id)
        {
            user = current;
            break;
        }
    }

    if (!user)
    {
        printf("User not found.\n");
        return -1;
    }

    // Check if PIN is "0" to enter new PIN
    if (strcmp(user->pin, "0") == 0)
    {
        printf("Your PIN is uninitialized. Please set a new 4-digit PIN: ");
        scanf("%s", password);
        update_user_password(users, user_id, password);
        save_users_to_csv("bank_user_info.csv", users);
        printf("PIN updated successfully.\n");
    }

    else
    {
        // If password is not "0", prompt for the existing password
        printf("Enter your password: ");
        scanf("%s", password);

        while (authenticate_user(users, user_id, password) == NULL)
        {
            printf("Invalid credentials. enter your password:");
            scanf("%s", password);
            if (authenticate_user(users, user_id, password) != NULL)
            {
                break;
            }
        }
    }

    return user_id;
}

void show_menu(User *users, int user_id)
{
    Transaction *transactions = load_transactions_from_csv("transaction_history.csv");
    int choice;
    double amount;

    do
    {
        printf("*********************\n");
        printf("\n1. Balance Inquiry\n2. Deposit\n3. Withdraw\n4. Transaction History\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            display_balance(users, user_id);
            break;

        case 2:
            printf("Enter amount to deposit: ");
            scanf("%lf", &amount);
            update_balance(users, user_id, amount, &transactions);
            
            break;

        case 3:
            printf("Enter amount to withdraw: ");
            scanf("%lf", &amount);
            withdraw(users, user_id, amount, &transactions); // Pass transactions by reference

            break;

        case 4:
            print_transactions_for_user(transactions, user_id);
            break;

        case 5:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 5);
}

int main()
{
    User *users = load_users_from_csv("bank_user_info.csv");

    while (1)
    {
        int user_id = login(users);
        if (user_id == -1)
        {
            // If user login failed, exit the loop
            break;
        }

        // If login was successful, show the menu
        show_menu(users, user_id);
    }

    return 0;
}
