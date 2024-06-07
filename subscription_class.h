#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include "user_class.h"

#define MAX_LINE_LENGTH 10000
#define COLS_SUBSCRIPTIONS 6
#define SUBSCRIPTIONS_FILE "subscriptions.csv"

enum frequencyType { E_WEEKLY = 0, E_MONTHLY = 1};
enum packageType { E_STANDARD = 20, E_PREMIUM = 30};


class Subscription {
private:
    static int id;
    static Subscription *headSubscriptions;

    int subscription_id;
    frequencyType frequency;
    packageType package;
    Flower *flower;
    User *user;
    int quantity;

public:
    Subscription *next;
    Subscription *prev;

    static void setHeadSubscriptions(Subscription *new_head){
        headSubscriptions = new_head;
    }

    static Subscription * getHeadSubscriptions(){
        return headSubscriptions;
    }

    static void readFromCSV() {
        FILE *file = fopen(SUBSCRIPTIONS_FILE, "r");
        if (file == nullptr) {
            printf("Error opening subscriptions file.\n");
            return;
        }

        delete headSubscriptions;

        headSubscriptions = nullptr;
        Subscription *lastSubscription = nullptr;
        char line[MAX_LINE_LENGTH];


        // Read each line and split into fields
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            int numFields = 0 ;
            Subscription *row = nullptr;
            char *token = strtok(line, ",");
            while (token != nullptr && numFields < COLS_SUBSCRIPTIONS) {
                if (numFields == 0) {
                    row = new Subscription(atoi(token));
                }
                if (numFields == 1) {
                    row->setFrequency ((frequencyType)atoi(token));
                }
                if (numFields == 2) {
                    row->setPackage ((packageType)atoi(token));
                }
                if (numFields == 3) {
                    row->setFlower(Flower::find_By_Id(atoi(token)));

                }
                if (numFields == 4) {
                    row->setUser(User::find_By_Id(atoi(token)));

                }
                if (numFields == 5) {
                    row->setQuantity(atoi(token));

                }
                token = strtok(nullptr, ",");
                numFields++;
            }
            if (headSubscriptions == nullptr) {
                headSubscriptions = row;
            } else {
                lastSubscription->next = row;
                row->prev = lastSubscription;
            }
            lastSubscription = row;
        }
        fclose(file);
    }

    static void writeToCSV() {
        FILE *file = fopen(SUBSCRIPTIONS_FILE, "w");
        if (file == nullptr) {
            printf("Error opening subscriptions file for writing.\n");
            return;
        }

        // Write each row to the file
        Subscription *current = headSubscriptions;
        while (current != nullptr) {
            fprintf(file, "%d,%d,%d,%d,%d,%d\n", current->getSubscriptionId(), current->getFrequency(), current->getPackage(), current->getFlower()->getFlowerId(), current->getUser()->getUserId(), current->getQuantity());
            current = current->next;
        }

        fclose(file);
    }

    static void update_id(int last_id)
    {
        if (last_id > id)
            id = last_id;
    }

    static void free_all_subscriptions() {
        Subscription *current = headSubscriptions;
        while (current != nullptr) {
            Subscription *temp = current;
            current = current->next;
            delete temp;
        }
    }
    static void show_all_subscriptions() {
        if (headSubscriptions!=nullptr) {
            Subscription *current = headSubscriptions;
            while (current != nullptr) {
                printf("%s\n",current->toString());
                current = current->next;
            }
            pause("");
        }
        else
            pause("There are no subscriptions in the database ");
    }

    static void show_user_subscriptions(User *selected_user) {
        if (headSubscriptions!=nullptr) {
            Subscription *current = headSubscriptions;
            while (current != nullptr)  {
                if ((current->user!=nullptr) && (current->user->getUserId()==selected_user->getUserId()))
                    printf("%s\n",current->toString());
                current = current->next;
            }
            pause("");
        }
        else
            pause("There are no subscriptions in the database ");
    }

    static void add(Subscription *subscription) {
        if (headSubscriptions==nullptr)
            headSubscriptions = subscription;
        else
        {
            Subscription *current = headSubscriptions;
            while (current->next!=nullptr) {
                current = current->next;
            }
            current->next = subscription;
        }
    }

    Subscription(){
        id++;
        subscription_id = id;
        next = nullptr;
        prev = nullptr;
        user = nullptr;
    }

    Subscription(int new_subscription_id){
        subscription_id = new_subscription_id;
        update_id(subscription_id);
        next = nullptr;
        prev = nullptr;
        user = nullptr;
    }

    ~Subscription(){
        //printf("Destructing %d", subscription_id);

        if (prev!=nullptr) {
            prev->next = next;

        }
        else
            headSubscriptions = next;

        if (next!=nullptr) {
            next->prev = prev;
        }


    }

    int getSubscriptionId() {
        return subscription_id;
    }

    void setSubscriptionId(int new_subscription_id) {
        subscription_id = new_subscription_id;
    }

    frequencyType getFrequency() {
        return frequency;
    }

    void setFrequency(frequencyType new_frequency) {
        frequency = new_frequency;
    }

    const char * getFrequencyString() {
        switch (frequency){
            case E_WEEKLY:
                return "WEEKLY";
            case E_MONTHLY:
                return "MONTHLY";
            default:
                return "";
        }
    }

    void setPackage(packageType new_package) {
        package = new_package;
    }

    packageType getPackage() {
        return package;
    }

    const char * getPackageString() {
        switch (package){
            case E_STANDARD:
                return "STANDARD";
            case E_PREMIUM:
                return "PREMIUM";
            default:
                return "";
        }
    }

    Flower *getFlower() {
        return flower;
    }

    User *getUser() {
        return user;
    }

    void setFlower(Flower *new_flower) {
        flower = new_flower;
    }

    void setUser(User *new_user) {
        user = new_user;
    }

    char * toString() {
        char* outputstring = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
        if (outputstring == nullptr) {
            std::cerr << "Memory allocation failed" << std::endl;
            return nullptr;
        }

        std::string flower_str;
        if (flower != nullptr) {
            flower_str = flower->toString();
        }

        snprintf(outputstring, MAX_LINE_LENGTH, "%d;%s;%s;%s;%d",
                 subscription_id, getFrequencyString(), getPackageString(), flower_str.c_str(), getQuantity());
        return outputstring;
    }

    int getQuantity() {
        return quantity;
    }

    void setQuantity(int new_quantity) {
        quantity = new_quantity;
    }
};

