#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include "flower_class.h"
#include "subscription_class.h"
#include "accounts.h"
#include "user_class.h"


#define MAX_LINE_LENGTH 10000


//Flower *headFlowers1;

User* User::headUsers = nullptr;
int User::id = 0;
int User::admin = 0;
User* User::selected = nullptr;

int Flower::id = 0;
Flower* Flower::headFlowers = nullptr;


int Subscription::id = 0;
Subscription* Subscription::headSubscriptions = nullptr;

//here start the functions

void close()
{
    std::cout << "\n\n\n\nGood bye!" << std::endl;
}


void add_flower() {
    std::cout <<  "Enter flower name:" << std::endl;
    char *name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    std::cin >> name;

    std::cout << "Enter flower description:" << std::endl;
    char *description = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    std::cin >> description;

    int quantity = 0;
    do
    {
        std::cout << "Enter flower quantity:" << std::endl;
        std::cin >> quantity;
    }
    while (quantity<=0);

    double price = 0;
    do
    {
        std::cout << "Enter flower price:" << std::endl;
        std::cin >> price;
    }
    while (price<=0);

    Flower *flower = new Flower;
    flower->setName(name);
    flower->setDescription(description);
    flower->setQuantity(quantity);
    flower->setPrice(price);


    Flower::add(flower);
    Flower::writeToCSV();

    pause("A new flower was successfully added.") ;
}


Subscription *add_subscription() {
    int frequency;
    std::cout << "Choose frequency type (0 - WEEKLY, 1 - MONTHLY): " << std::endl;
    std::cin >> frequency;

    int package;
    std::cout << "Choose package type (20 - STANDARD, 30 - PREMIUM): " << std::endl;
    std::cin >> package;


    std::cout << "Enter flower id to subscribe: "  << std::endl;
    //char *name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    int flower_id;
    std::cin >> flower_id;


    Flower *found_Flower = Flower::find_By_Id(flower_id);
    if (found_Flower==nullptr) {
        printf("The flower with id = %d does not exist.", flower_id);
        pause("");
        return nullptr;
    }
    else
    {

        int quantity;
        std::cout << "Enter quantity: " << std::endl;
        std::cin >> quantity;

        if (quantity>found_Flower->getQuantity()) {
            printf("The flower with id = %d does not have enough quantity.", found_Flower->getFlowerId());
            pause("");
            return nullptr;
        }

        double final_sum = quantity * found_Flower->getPrice() + (int) package;

        Account *found_account = Account::find_By_user_and_sum(User::getSelectedUser()->getUserId(), final_sum);
        if (found_account== nullptr) {
            printf("The user does not have enough money");
            pause("");
            return nullptr;
        }

        found_account->setBalance(found_account->getBalance()-final_sum);
        found_Flower->setQuantity(found_Flower->getQuantity()-quantity);
        Subscription *subscription = new Subscription;
        subscription->setFrequency((frequencyType) frequency);
        subscription->setPackage((packageType) package);
        subscription->setFlower(found_Flower);
        subscription->setQuantity(quantity);
        subscription->setUser(User::getSelectedUser());

        Subscription::add(subscription);
        Subscription::writeToCSV();
        Account::writeToCSV();
        Flower::writeToCSV();

        printf("A new subscription with %lf amount was successfully added.", final_sum);
        pause("");
        return subscription;
    }
}

Account *add_account() {
    std::cout <<  "Enter IBAN:" << std::endl;
    char *IBAN = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    std::cin >> IBAN;

    double balance;
    std::cout << "Enter account balance:" << std::endl;
    std::cin >> balance;


    Account *account = new Account;
    account->setIBAN(IBAN);
    account->setBalance(balance);
    account->setUser(User::getSelectedUser());

    Account::add(account);
    Account::writeToCSV();


    pause("A new account was successfully added.");
    return account;
}

void show_flower_info(Flower *flower){
    if (flower!=nullptr){
        printf("Selected flower information: %s", flower->toString());
    }
    pause("");
}

void edit_flower_name(Flower *flower){
    if (flower!=nullptr) {
        printf("%s", "Enter flower name: ");
        char *name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", name);
        flower->setName(name);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        Flower::writeToCSV();
        pause("Flower name was changed.");
    }
    else pause("");
}

void edit_flower_description(Flower *flower){
    if (flower!=nullptr) {
        printf("%s", "Enter flower description: ");
        char *description = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", description);
        flower->setDescription(description);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        Flower::writeToCSV();
        pause("Flower description was changed.");
    }
    else pause("");

}

void edit_flower_quantity(Flower *flower){
    if (flower!=nullptr) {
         int quantity = 0;
         do
         {
            printf("%s", "Enter flower quantity: ");
            scanf("%d", &quantity);
         }
         while (quantity<=0);
         flower->setQuantity(quantity);
         //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
         Flower::writeToCSV();
         pause("Flower quantity was changed.");
    }
    else pause("");

}

void edit_flower_price(Flower *flower){
    if (flower!=nullptr) {
        double price = 0;
        do
        {
            printf("%s", "Enter flower price: ");
            scanf("%lf", &price);
        }
        while (price<=0);
        flower->setPrice(price);
        Flower::writeToCSV();
        pause("Flower price was changed.");
    }
    else pause("");

}

int showFlowerMenu(Flower *currentFlower) {
    int choice;
    //system("cls");
	printf("\n\n\t\t\t          FLOWER SUBMENU\n");
    printf("\t\t[1] SHOW SELECTED FLOWER INFO\n");
	printf("\t\t[2] EDIT NAME\n");
	printf("\t\t[3] EDIT DESCRIPTION\n");
	printf("\t\t[4] EDIT QUANTITY\n");
    printf("\t\t[5] EDIT PRICE\n");


	printf("\t\t[0] RETURN TO MAIN MENU\n\n\n");
	printf("\t\tEnter your choice:");
    scanf("%d",&choice);

    //system("cls");

	switch(choice)
    {
        case 1:show_flower_info(currentFlower);
        break;
        case 2:edit_flower_name(currentFlower);
        break;
        case 3:edit_flower_description(currentFlower);
        break;
        case 4:edit_flower_quantity(currentFlower);
        break;
        case 5:edit_flower_price(currentFlower);
        break;
    }

    return choice;
}

void edit_flower() {
    printf("%s", "Enter flower id for editing:");
    //char *name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    int flower_id;
    scanf("%d", &flower_id);


    Flower *found_Flower = Flower::find_By_Id(flower_id);
    if (found_Flower==nullptr) {
        printf("The flower with id = %d does not exists.",flower_id);
        pause("");
    }
    else
    {
        show_flower_info(found_Flower);
        while (showFlowerMenu(found_Flower)!=0){}
    }
}

void delete_flower() {
    printf("%s", "Enter flower id for deleting:");
    //char *name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    int flower_id;
    scanf("%d", &flower_id);


    Flower *found_Flower = Flower::find_By_Id(flower_id);
    if (found_Flower==nullptr) {
        printf("The flower with id = %d does not exist.",flower_id);
        pause("");
    } else {
        //printf("Deleting...%d", found_Flower->getFlowerId());
        delete found_Flower;
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        Flower::writeToCSV();
        pause("The selected flower was successfully deleted.") ;
    }
}

void edit_username(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new username:");
        char *username = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", username);
        user->setUserName(username);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("Username was changed.");
    }
    else pause("");
}

void edit_first_name(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new first name:");
        char *first_name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", first_name);
        user->setFirstName(first_name);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("First name was changed.");
    }
    else pause("");
}

void edit_last_name(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new last name:");
        char *last_name = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", last_name);
        user->setLastName(last_name);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("Last name was changed.");
    }
    else pause("");
}

void edit_address(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new address:");
        char *address = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", address);
        user->setAddress(address);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("Address was changed.");
    }
    else pause("");
}

void edit_phone(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new phone number:");
        char *phone = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", phone);
        user->setPhoneNumber(phone);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("Phone number was changed.");
    }
    else pause("");
}

void edit_password(User *user){
    if (user!=nullptr) {
        printf("%s", "Enter new password:");
        char *password = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        scanf("%s", password);
        user->setPassword(password);
        //writeCSVFlowersRows(FLOWERS_FILE, Flower::getHeadFlowers());
        User::writeToCSV();
        pause("Password was changed.");
    }
    else pause("");
}

int showUserEditMenu(User *currentUser) {
    int choice;
    //system("cls");
    printf("\n\n\t\t\t          USER SUBMENU\n");
    printf("\t\t[1] SHOW USER INFO\n");
    printf("\t\t[2] EDIT USERNAME\n");
    printf("\t\t[3] EDIT FIRST NAME\n");
    printf("\t\t[4] EDIT LAST NAME\n");
    printf("\t\t[5] EDIT ADDRESS\n");
    printf("\t\t[6] EDIT PHONE\n");
    printf("\t\t[7] EDIT PASSWORD\n");

    printf("\t\t[0] RETURN TO MAIN MENU\n\n\n");
    printf("\t\tEnter your choice:");
    scanf("%d",&choice);

    //system("cls");

    switch(choice)
    {
        case 1:User::showUserInfo(currentUser);
            break;
        case 2:edit_username(currentUser);
            break;
        case 3:edit_first_name(currentUser);
            break;
        case 4:edit_last_name(currentUser);
            break;
        case 5:edit_address(currentUser);
            break;
        case 6:edit_phone(currentUser);
            break;
        case 7:edit_password(currentUser);
            break;
    }

    return choice;
}

void edit_user(User* user) {
    //User *found_User = User::find_By_Id(user->getUserId());

    while (showUserEditMenu(user)!=0){}

}

int showAdminMenu()
{
    int choice;
    //system("cls");
    printf("\n\n\t\t\t          FLOWERS MANAGEMENT SYSTEM");
    printf("\n\n\n\t\t\t-------------- WELCOME TO THE MAIN MENU --------------\n\n");
    printf("\t\t[1] SHOW FLOWERS\n");
    printf("\t\t[2] ADD FLOWER\n");
    printf("\t\t[3] EDIT FLOWER\n");
    printf("\t\t[4] DELETE FLOWER\n");
    printf("\t\t[5] SHOW USERS\n");

    printf("\t\t[0] EXIT\n\n\n");
    printf("\t\tEnter your choice:");
    scanf("%d",&choice);

    //system("cls");

    switch(choice)
    {
        case 1:Flower::show_all_flowers();
            break;

        case 2:add_flower();
            break;

        case 3:edit_flower();
            break;

        case 4:delete_flower();
            break;

        case 5:User::showAllUsers();
            break;

        case 0:close();
            break;
    }

    return choice;
}

int showUserMenu(User *selected_user)
{
	int choice;
    //system("cls");
	printf("\n\n\t\t\t          FLOWERS MANAGEMENT SYSTEM");
    printf("\n\n\n\t\t\t-------------- WELCOME TO THE MAIN MENU --------------\n\n");
    printf("\t\t[1] SHOW FLOWERS\n");
	printf("\t\t[2] SHOW SUBSCRIPTIONS\n");
    printf("\t\t[3] EDIT USER\n");
	printf("\t\t[4] ADD SUBSCRIPTION\n");
    printf("\t\t[5] SHOW ACCOUNTS\n");
    printf("\t\t[6] ADD ACCOUNT\n");
	printf("\t\t[0] EXIT\n\n\n");
	printf("\t\tEnter your choice:");
    std::cin >> choice;

    //system("cls");
	
	switch (choice)
    {
        case 1:Flower::show_all_flowers();
            break;
        
        case 2:Subscription::show_user_subscriptions(selected_user);
            break;

        case 3:edit_user(selected_user);
            break;
        
        case 4:add_subscription();
            break;

        case 5:Account::show_user_accounts(selected_user);
            break;

        case 6:add_account();
            break;

        case 0:close();
            break;
    }
    
    return choice;
}

int showInitialMenu(){
    int choice;
    printf("\n\n\t\t\t          FLOWERS MANAGEMENT SYSTEM");
    printf("\n\n\n\t\t\t-------------- WELCOME TO THE LOGIN MENU --------------\n\n");
    printf("\t\t[1] CREATE ACCOUNT\n");
    printf("\t\t[2] LOGIN\n");
    printf("\t\t[0] EXIT\n\n\n");
    printf("\t\tEnter your choice:");
    std::cin >> choice;

    switch (choice) {
        case 1:
            User::setSelectedUser(User::registerUser());
            break;
        case 2:
            User::login();
            break;
        case 0:
            close();
            break;
        default:
            std::cerr << "Invalid choice." << std::endl;
            break;
    }
    return choice;
}
int main() {
    User::loadFromCSV();

    User::setAdmin(0);
    User::setSelectedUser(nullptr);

    int choice;
    do {
        choice = showInitialMenu();
    }
    while ((choice!=0) && ((User::getAdmin()==0) && (User::getSelectedUser()== nullptr)));

    if (choice!=0) {
        std::cout << "Welcome to the Flower Subscription Service!" << std::endl;
        Flower::readFromCSV();
        Subscription::readFromCSV();
        Account::readFromCSV();

        if (User::getAdmin()==1)
            while (showAdminMenu()!=0){
            }
        else
            while (showUserMenu(User::getSelectedUser())!=0){
            }
        Account::free_all_accounts();
        Subscription::free_all_subscriptions();
        Flower::free_all_flowers();
    }

    User::free_all_users();
    return 0;
}
