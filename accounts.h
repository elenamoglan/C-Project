#ifndef C_PROJECT_ACCOUNTS_H
#define C_PROJECT_ACCOUNTS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include "user_class.h"

#define ACCOUNTS_FILE "accounts.csv"
#define MAX_LINE_LENGTH 10000


class Account {
private:
    static int id;
    static Account *headAccounts;

    int account_id;
    std::string IBAN;
    User *user; // User object
    double balance;

public:
    Account *next;
    Account *prev;

    // Constructor and Destructor
    Account();
    Account(int new_account_id);
    ~Account();

    // Static methods for managing the linked list

    static void readFromCSV();
    static void writeToCSV();
    static void update_id(int last_id);
    static void free_all_accounts();
    static void show_all_accounts();
    static void show_user_accounts(User *selected_user);
    static void add(Account *account);
    static Account * find_By_user_and_sum(int user_id, double sum);

    // Getters and Setters
    int getAccountId() const;
    void setAccountId(int new_account_id);

    std::string getIBAN() const;
    void setIBAN(const std::string& new_IBAN);

    User* getUser() const;
    void setUser(User *new_user);

    double getBalance() const;
    void setBalance(double new_balance);

    void displayAccountInfo() const;



};

int Account::id = 0;
Account* Account::headAccounts = nullptr;

Account::Account() : account_id(++id), user(nullptr), balance(0.0), next(nullptr), prev(nullptr) {}

Account::Account(int new_account_id) : account_id(new_account_id), user(nullptr), balance(0.0), next(nullptr), prev(nullptr) {
    update_id(new_account_id);
}

Account::~Account() {
    if (prev != nullptr) {
        prev->next = next;
    } else {
        headAccounts = next;
    }

    if (next != nullptr) {
        next->prev = prev;
    }
}

Account * Account::find_By_user_and_sum(int user_id, double sum){
    Account *current = headAccounts;
    while ((current != nullptr) &&  ((current->getUser()->getUserId()!=user_id) || (current->getBalance()<sum))) {
        current = current->next;
    }
    return current;
}


void Account::readFromCSV() {
    FILE *file = fopen(ACCOUNTS_FILE, "r");
    if (file == nullptr) {
        printf("Error opening accounts file.\n");
        return;
    }

    free_all_accounts();

    char line[MAX_LINE_LENGTH];
    Account *lastAccount = nullptr;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        int numFields = 0;
        Account *row = nullptr;
        char *token = strtok(line, ",");
        while (token != nullptr && numFields < 4) {
            if (numFields == 0) {
                row = new Account(atoi(token));
            }
            if (numFields == 1) {
                row->setIBAN(token);
            }
            if (numFields == 2) {
                row->setUser(User::find_By_Id(atoi(token))); // Setting user by ID
            }
            if (numFields == 3) {
                row->setBalance(atof(token));
            }
            token = strtok(nullptr, ",");
            numFields++;
        }
        if (headAccounts == nullptr) {
            headAccounts = row;
        } else {
            lastAccount->next = row;
            row->prev = lastAccount;
        }
        lastAccount = row;
    }
    fclose(file);
}

void Account::writeToCSV() {
    FILE *file = fopen(ACCOUNTS_FILE, "w");
    if (file == nullptr) {
        printf("Error opening accounts file for writing.\n");
        return;
    }

    Account *current = headAccounts;
    while (current != nullptr) {
        fprintf(file, "%d,%s,%d,%.2f\n", current->getAccountId(), current->getIBAN().c_str(), current->getUser()->getUserId(), current->getBalance());
        current = current->next;
    }

    fclose(file);
}

void Account::update_id(int last_id) {
    if (last_id > id) {
        id = last_id;
    }
}

void Account::free_all_accounts() {
    Account *current = headAccounts;
    while (current != nullptr) {
        Account *temp = current;
        current = current->next;
        delete temp;
    }
    headAccounts = nullptr;
}

void Account::show_all_accounts() {
    if (headAccounts != nullptr) {
        Account *current = headAccounts;
        while (current != nullptr) {
            current->displayAccountInfo();
            current = current->next;
        }
    } else {
        std::cout << "There are no accounts in the database." << std::endl;
    }
}

void Account::show_user_accounts(User *selected_user) {
    if (headAccounts != nullptr) {
        Account *current = headAccounts;
        while (current != nullptr) {
            if ((current->getUser() != nullptr) && (current->getUser()->getUserId() == selected_user->getUserId())) {
                current->displayAccountInfo();
            }
            current = current->next;
        }
        pause("");
    } else {
        std::cout << "There are no accounts in the database." << std::endl;
    }
}

void Account::add(Account *account) {
    if (headAccounts == nullptr) {
        headAccounts = account;
    } else {
        Account *current = headAccounts;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = account;
        account->prev = current;
    }
}

int Account::getAccountId() const {
    return account_id;
}

void Account::setAccountId(int new_account_id) {
    account_id = new_account_id;
}

std::string Account::getIBAN() const {
    return IBAN;
}

void Account::setIBAN(const std::string& new_IBAN) {
    IBAN = new_IBAN;
}

User* Account::getUser() const {
    return user;
}

void Account::setUser(User *new_user) {
    user = new_user;
}

double Account::getBalance() const {
    return balance;
}

void Account::setBalance(double new_balance) {
    balance = new_balance;
}

void Account::displayAccountInfo() const {
    std::cout << "Account ID: " << account_id << "\nIBAN: " << IBAN
              << "\nUser: " << (user != nullptr ? user->getUserId() : 0)
              << "\nBalance: " << balance << std::endl;
}

#endif //C_PROJECT_ACCOUNTS_H
