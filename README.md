User manual for Flower Subscription System

Programming project by Elena Moglan and Hyerahn Kim
Submission Date 07.06.2024

1. Introduction
This console-based application allows customers to make orders for flower subscription and manage their account while allowing the shop administrator to manage the flowers.

2. The application consists of these files:
- flowers_class.h
It contains class definitions of flowers and some of the attributes and functions related to flowers.
The flower class contains flower id, name, description, quantity and the price.

- subscription_class.h
It contains the class definition of subscription and some of the attributes and functions related to subscriptions.

- user_class.h
It contains the class definition of user and some of the attributes related to user management.- accounts.h
It contains class accounts, information about user’s payment, and some functions that are relevant to the payment.

- main.cpp
The entry point of the application. This file initializes the system and it includes many crucial functions that are necessary to run the application.

- accounts.csv
This CSV file contains the user's payment information. it saves the account id, IBAN, user(as an object) and the balance they own in their account.

- subscriptions.csv
This CSV file contains the subscription information, in other words it is the recurring order from customers.
It consists of the subscription id, frequency type either they want weekly or monthly, package type for normal or premium product, what kind of flower, the user as an object and the quantity.

- user.csv
A CSV file that stores data about users registered in the system. It includes user IDs, usernames,first name, last name, address, phone number and the password.
When a new user creates an account, it is saved here. A user can login using username and password saved in here.

 
