#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 50

// ex1
typedef struct NodeD
{
    unsigned short digit;
    struct NodeD *next;
} NodeD;

void reverseList(NodeD** head)
{
    NodeD* next = NULL;
    NodeD* prev = NULL;
    NodeD* temp = *head;
    while(temp)
    {
        next = temp->next;
        temp->next = prev;
        prev = temp;
        temp = next;
    }
    *head = prev;

}

void addAtEnd(NodeD** head, unsigned short digit)
{
    NodeD* temp = *head;
    NodeD* newNode = (NodeD*)malloc(sizeof(NodeD));
    if(!newNode)
        return;
    newNode->digit = digit;
    newNode->next = NULL;
    if(!*head)
    {
        *head = newNode;
        return;
    }

    while(temp->next)
    {
        temp = temp->next;
    }
    temp->next = newNode;


}

NodeD* AddBigNumbers(NodeD* N1, NodeD* N2)
{
    NodeD* ptr1 = N1;
    NodeD* ptr2 = N2;
    NodeD* N3 = NULL;
    int sum = 0, carry = 0;
    while(ptr1 || ptr2)
    {
        sum = 0;
        if(ptr1)
            sum += ptr1->digit;
        if(ptr2)
            sum += ptr2->digit;
        sum+= carry;
        carry = sum/10;
        sum = sum%10;
        addAtEnd(&N3,sum);
        if(ptr1)
            ptr1 = ptr1->next;
        if(ptr2)
            ptr2 = ptr2->next;

    }
    if(carry)
        addAtEnd(&N3,carry);
    return N3;
}

void display(NodeD* head)
{
    if(!head)
        return;
    printf("%d -> ", head->digit);
    display(head->next);

}




// ex2



typedef struct Transaction
{
    int id;
    char date[size];
    float amount;
    int type;
} Transaction;

typedef struct NodeT
{
    Transaction transaction;
    struct NodeT *next;
} NodeT;

typedef struct Account
{
    int id;
    char name[size];
    float balance;
    NodeT *transactions;
} Account;

typedef struct NodeA
{
    Account account;
    struct NodeA* next;
} NodeA;

typedef struct Bank
{
    char name[size];
    char address[size];
    int numBranches;
    int numAccounts;
    NodeA* accounts;
} Bank;

float getBalance(Bank *B, int id)
{
    NodeA* current = B->accounts;
    while(current)
    {
        if(current->account.id == id)
            return current->account.balance;
        current = current->next;
    }
    return -1.0;// id not found
}

int CreateAccount(Bank *B, Account A)
{
    NodeA* current = B->accounts;
    while(current)
    {
        if(strcmp(current->account.name, A.name) == 0)
            return -1;
        current = current->next;
    }
    NodeA* newNode = (NodeA*)malloc(sizeof(NodeA));
    if (!newNode)
    {
        printf("Memory allocation failed");
        return -1;
    }
    newNode->account = A;
    newNode->next = B->accounts;
    B->accounts = newNode;
    B->numAccounts++;
    return 0; // success.
}

float PerformTransaction(Bank *B, Transaction T, int id)
{
    NodeA* current = B->accounts;
    while(current)
    {
        if(current->account.id == id)
            if(T.type == 1)
            {
                current->account.balance += T.amount;
                return current->account.balance;
            }

            else if(T.type == 0)
            {
                current->account.balance -= T.amount;
                return current->account.balance;

            }
        else
            return -1.0; // not found
        current = current->next;
    }



}

void SaveAccounts(Bank *B){
    FILE *file = fopen("bank.txt","w");
    if(!file)
        return;
    NodeA *currentAccount = B->accounts;
    while (currentAccount) {
        // account information
        fprintf(file, "Account ID: %d\n", currentAccount->account.id);
        fprintf(file, "Account Name: %s\n", currentAccount->account.name);
        fprintf(file, "Account Balance: %.2f\n", currentAccount->account.balance);

        //  transactions info
        NodeT *currentTransaction = currentAccount->account.transactions;
        while (currentTransaction) {
            fprintf(file, "Transaction ID: %d\n", currentTransaction->transaction.id);
            fprintf(file, "Transaction Date: %s\n", currentTransaction->transaction.date);
            fprintf(file, "Transaction Amount: %.2f\n", currentTransaction->transaction.amount);
            fprintf(file, "Transaction Type: %d\n", currentTransaction->transaction.type);
            fprintf(file, "\n"); // Separate transactions

            currentTransaction = currentTransaction->next;
        }

        fprintf(file, "\n"); // Separate accounts

        currentAccount = currentAccount->next;
    }
    if(fclose(file))
    {
        printf("Not able to save the file");
        return;
    }

}

int main()
{
    Bank bank;
    strcpy(bank.name, "MyBank");
    strcpy(bank.address, "123 Main Street");
    bank.numBranches = 1;
    bank.numAccounts = 0;
    bank.accounts = NULL; // Initialize accounts list to NULL

    // Create some accounts
    Account account1, account2;
    account1.id = 1001;
    strcpy(account1.name, "Alice");
    account1.balance = 5000.0;
    account1.transactions = NULL; // Initialize transactions list to NULL

    account2.id = 1002;
    strcpy(account2.name, "Bob");
    account2.balance = 3000.0;
    account2.transactions = NULL; // Initialize transactions list to NULL

    // Add accounts to the bank
    if (CreateAccount(&bank, account1) == 0) {
        printf("Account for Alice created successfully.\n");
        bank.numAccounts++;
    } else {
        printf("Failed to create account for Alice.\n");
    }

    if (CreateAccount(&bank, account2) == 0) {
        printf("Account for Bob created successfully.\n");
        bank.numAccounts++;
    } else {
        printf("Failed to create account for Bob.\n");
    }

    // Perform transactions
    Transaction transaction1, transaction2;
    transaction1.id = 1;
    strcpy(transaction1.date, "2024-01-01");
    transaction1.amount = 1000.0;
    transaction1.type = 1; // Deposit

    transaction2.id = 2;
    strcpy(transaction2.date, "2024-01-02");
    transaction2.amount = 500.0;
    transaction2.type = 0; // Withdrawal

    float balance1 = PerformTransaction(&bank, transaction1, account1.id);
    if (balance1 >= 0) {
        printf("Transaction 1 for Alice completed successfully. New balance: %.2f\n", balance1);
    } else {
        printf("Transaction 1 for Alice failed.\n");
    }

    float balance2 = PerformTransaction(&bank, transaction2, account2.id);
    if (balance2 >= 0) {
        printf("Transaction 2 for Bob completed successfully. New balance: %.2f\n", balance2);
    } else {
        printf("Transaction 2 for Bob failed.\n");
    }

    // Save account information to file
    SaveAccounts(&bank);

    return 0;
}



