#ifndef USER_CLASS_H
#define USER_CLASS_H

#include <iostream>
#include <fstream>
#include <cstring>
#define PERSONS_FILE "users.csv"
#define MAX_LINE_LENGTH 10000

class User {
private:
    int user_id;
    char *user_name;
    char* first_name;
    char* last_name;
    char* address;
    char* phone_number;
    char* password;

    User* next;
    User* prev;
    static User* headUsers;
    static User *selected;
    static int id;
    static int admin;

public:
    static void setSelectedUser(User *user){
        selected = user;
    }

    static User * getSelectedUser(){
        return selected;
    }

    static void setAdmin(int _admin){
        admin = _admin;
    }

    static int getAdmin(){
        return admin;
    }

    static void update_id(int last_id)
    {
        if (last_id > id)
            id = last_id;
    }

    User() {
        id++;
        user_id = id;
        next = nullptr;
        prev = nullptr;
    }

    User(int new_user_id){
        user_id = new_user_id;
        update_id(user_id);
        next = nullptr;
        prev = nullptr;
    }

    ~User() {
        free(first_name);
        free(last_name);
        free(address);
        free(phone_number);
        free(password);

        if (prev != nullptr) {
            prev->next = next;
        } else {
            headUsers = next;
        }

        if (next != nullptr) {
            next->prev = prev;
        }
    }

    static void add(User *user) {
        if (headUsers==nullptr)
            headUsers = user;
        else
        {
            User *current = headUsers;
            while (current->next!=nullptr) {
                current = current->next;
            }
            current->next = user;
        }
    }

    static void free_all_users() {
        User *current = headUsers;
        while (current != nullptr) {
            User *temp = current;
            current = current->next;
            delete temp;
        }
    }

    int getUserId() { return user_id; }

    void setUserId(int new_user_id) { user_id = new_user_id; }

    char * getUserName() { return user_name; }
    void setUserName(const char* new_username) {
        user_name = (char*) malloc(sizeof(new_username)+1);
        strcpy(user_name, new_username);
    }

    char* getFirstName() { return first_name; }
    void setFirstName(const char* new_first_name) {
        first_name = (char*) malloc(strlen(new_first_name) + 1);
        strcpy(first_name, new_first_name);
    }

    char* getLastName() { return last_name; }
    void setLastName(const char* new_last_name) {
        last_name = (char*) malloc(strlen(new_last_name) + 1);
        strcpy(last_name, new_last_name);
    }

    char* getAddress() { return address; }
    void setAddress(const char* new_address) {
        address = (char*) malloc(strlen(new_address) + 1);
        strcpy(address, new_address);
    }

    char* getPhoneNumber() { return phone_number; }
    void setPhoneNumber(const char* new_phone_number) {
        phone_number = (char*) malloc(strlen(new_phone_number) + 1);
        strcpy(phone_number, new_phone_number);
    }

    char* getPassword() { return password; }
    void setPassword(const char* new_password) {
        password = (char*) malloc(strlen(new_password) + 1);
        strcpy(password, new_password);
    }

    static User * find_By_Id(int user_id){
        User *current = headUsers;
        while ((current != nullptr) && (current->getUserId()!=user_id)) {
            current = current->next;
        }
        return current;
    }

    static User * find_By_Username(char *username, char *password){
        User *current = headUsers;
        while ((current != nullptr) && ((strcmp(current->getUserName(),username)!=0 ) || (strcmp(current->getPassword(),password)!=0 ))) {
            current = current->next;
        }
        return current;
    }


    static void writeToCSV() {
        std::ofstream file(PERSONS_FILE);
        User* current = headUsers;
        while (current != nullptr) {
            file << current->getUserId() << "," << current->getUserName() << "," << current->getFirstName() << "," << current->getLastName() << "," << current->getAddress() << "," << current->getPhoneNumber() << "," << current->getPassword() << std::endl;
            current = current->next;
        }
        file.close();
    }

    /*
    static void loadFromCSV() {
        std::ifstream file(PERSONS_FILE);
        char line[MAX_LINE_LENGTH];
        while (file.getline(line, MAX_LINE_LENGTH)) {
            char* token = strtok(line, ",");
            int user_id = atoi(token);

            token = strtok(nullptr, ",");
            const char* user_name = token;

            token = strtok(nullptr, ",");
            const char* first_name = token;

            token = strtok(nullptr, ",");
            const char* last_name = token;

            token = strtok(nullptr, ",");
            const char* address = token;

            token = strtok(nullptr, ",");
            const char* phone_number = token;

            token = strtok(nullptr, ",");
            const char* password = token;

            User* user = new User(user_id);
            user->setUserName(user_name);
            user->setFirstName(first_name);
            user->setLastName(last_name);
            user->setAddress(address);
            user->setPhoneNumber(phone_number);
            user->setPassword(password);

            if (headUsers == nullptr) {
                headUsers = user;
            } else {
                User* current = headUsers;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = user;
                user->prev = current;
            }
        }
        file.close();
    }
*/

    static void loadFromCSV() {
        FILE *file = fopen(PERSONS_FILE, "r");
        if (file == nullptr) {
            printf("Error opening users file.\n");
            return;
        }

        delete headUsers;

        headUsers = nullptr;
        User *lastUser = nullptr;
        char line[MAX_LINE_LENGTH];


        // Read each line and split into fields
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            int numFields = 0 ;
            User *row = nullptr;
            char *token = strtok(line, ",");
            while (token != nullptr && numFields < 7) {
                if (numFields == 0) {
                    row = new User(atoi(token));
                }
                if (numFields == 1) {
                    row->setUserName (token);
                }

                if (numFields == 2) {
                    row->setFirstName (token);
                }
                if (numFields == 3) {
                    row->setLastName (token);
                }

                if (numFields == 4) {
                    row->setAddress (token);

                }
                if (numFields == 5) {
                    row->setPhoneNumber(token);

                }

                if (numFields == 6) {
                    size_t ln = strlen(token) - 1;
                    if (*token && token[ln] == '\n')
                        token[ln] = '\0';
                    row->setPassword (token);
                }

                token = strtok(nullptr, ",");
                numFields++;
            }
            if (headUsers == nullptr) {
                headUsers = row;
            } else {
                lastUser->next = row;
                row->prev = lastUser;
            }
            lastUser = row;
        }
        fclose(file);
    }

    static int get_id() {
        return id;
    }

    static User * registerUser() {
        char user_name[MAX_LINE_LENGTH];
        char first_name[MAX_LINE_LENGTH];
        char last_name[MAX_LINE_LENGTH];
        char address[MAX_LINE_LENGTH];
        char phone_number[MAX_LINE_LENGTH];
        char password[MAX_LINE_LENGTH];

        printf("Enter user name:");
        scanf("%s", user_name);

        printf("Enter password:");
        scanf("%s", password);

        User *found_user = User::find_By_Username(user_name, password);
        if (found_user != nullptr) {
            printf("Username %s already exists.\n", user_name);
            return found_user;
        }

        printf("Enter first name:");
        scanf("%s", first_name);

        printf("Enter last name:");
        scanf("%s", last_name);

        printf("Enter address:");
        scanf(" %[^\n]s", address); // Allows spaces in address

        printf("Enter phone number:");
        scanf("%s", phone_number);



        User* user = new User();
        user->setUserName(user_name);
        user->setPassword(password);
        user->setFirstName(first_name);
        user->setLastName(last_name);
        user->setAddress(address);
        user->setPhoneNumber(phone_number);


        User::add(user);

        User::writeToCSV();
        pause("Registration successful.");
        return user;
        }

    static bool login() {
        char *user_name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));;
        char *password = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));

        printf("Enter user name:");
        scanf("%s", user_name);


        printf("Enter password:");
        scanf("%s", password);

        User *found_user = nullptr;

        if ((strcmp(user_name, "admin")==0) && (strcmp(password, "admin")==0))
        {
            User::setAdmin(1);
            printf("Login successful as administrator.");
        }
        else
        {
            User::setAdmin(0);

            found_user = User::find_By_Username(user_name, password);
            if (found_user==nullptr) {
                printf("Invalid username or password.");
            }
            else
                printf("Login successful as user: %s.",user_name);
        }
        pause("");
        User::setSelectedUser(found_user);
        return found_user;
    }

    static void showUserInfo(User* user) {
        if (user != nullptr) {
            printf("User ID: %d\n", user->getUserId());
            printf("Username: %s\n", user->getUserName());
            printf("First Name: %s\n", user->getFirstName());
            printf("Last Name: %s\n", user->getLastName());
            printf("Address: %s\n", user->getAddress());
            printf("Phone Number: %s\n", user->getPhoneNumber());
            pause("");
        } else {
            pause("User not found.\n");
        }
    }

    static void showAllUsers() {
        if (headUsers!= nullptr) {
            User *current = headUsers;
            while (current != nullptr) {
                showUserInfo(current);
                current = current->next;
            }
            //pause("");
        } else {
            pause("There are no users in the database.");
        }
    }

};



#endif
