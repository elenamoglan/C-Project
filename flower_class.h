#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cstring>

#ifndef FLOWER_CLASS_H
#define FLOWER_CLASS_H
#define COLS_FLOWERS 5
#define FLOWERS_FILE "flowers.csv"
#define MAX_LINE_LENGTH 10000

void pause(const char *mess) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {};
    printf("%s\n",mess);
    printf("%s", "Press Enter to continue...");
    std::cin.get();
}

class Flower {
private:
    static int id;
    static Flower *headFlowers;

    int flower_id;
    char *name{};
    char *description{};
    int quantity{};
    double price;
    Flower *next;
    Flower *prev;

public:

    static void setHeadFlowers(Flower *new_head){
        headFlowers = new_head;
    }

    static Flower * getHeadFlowers(){
        return headFlowers;
    }

    static void add(Flower *flower) {
        if (headFlowers==nullptr)
            headFlowers = flower;
        else
        {
            Flower *current = headFlowers;
            while (current->next!=nullptr) {
                current = current->next;
            }
            current->next = flower;
        }
    }

    static void free_all_flowers() {
        Flower *current = headFlowers;
        while (current != nullptr) {
            Flower *temp = current;
            current = current->next;
            delete temp;
        }
    }

    static void show_all_flowers() {
        if (headFlowers!=nullptr) {
            Flower *current = headFlowers;
            printf("%10s|%20s|%20s|%10s|%10s\n", "id","name","description","quantity","price");
            printf("--------------------------------------------------------------------------\n");
            while (current != nullptr) {
                printf("%s\n",current->toString());
                current = current->next;
            }
            pause("");
        }
        else
            pause("There are no flowers in the database ");
    }


    static Flower * find_By_Id(int flower_id){
        Flower *current = Flower::getHeadFlowers();
        while ((current != nullptr) && (current->getFlowerId()!=flower_id)) {
            current = current->next;
        }
        return current;
    }


    static void readFromCSV() {
        FILE *file = fopen(FLOWERS_FILE, "r");
        if (file == nullptr) {
            printf("Error opening flowers file.\n");
            return;
        }

        delete headFlowers;

        headFlowers = nullptr;
        Flower *lastFlower = nullptr;
        char line[MAX_LINE_LENGTH];


        // Read each line and split into fields
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            int numFields = 0 ;
            Flower *row = nullptr;
            char *token = strtok(line, ",");
            while (token != nullptr && numFields < COLS_FLOWERS) {
                if (numFields == 0) {
                    row = new Flower(atoi(token));
                }
                if (numFields == 1) {
                    row->setName (token);
                }
                if (numFields == 2) {
                    row->setDescription (token);
                }
                if (numFields == 3) {
                    row->setQuantity(atoi(token));

                }
                if (numFields == 4) {
                    row->setPrice(atof(token));

                }
                token = strtok(nullptr, ",");
                numFields++;
            }
            if (headFlowers == nullptr) {
                headFlowers = row;
            } else {
                lastFlower->next = row;
                row->prev = lastFlower;
            }
            lastFlower = row;
        }
        fclose(file);
    }

    static void writeToCSV() {
        FILE *file = fopen(FLOWERS_FILE, "w");
        if (file == nullptr) {
            printf("Error opening flowers file for writing.\n");
            return;
        }

        // Write each row to the file
        Flower *current = headFlowers;
        while (current != nullptr) {
            fprintf(file, "%d,%s,%s,%d,%lf\n", current->getFlowerId(), current->getName(), current->getDescription(), current->getQuantity(), current->getPrice());
            current = current->next;
        }

        fclose(file);
    }

    static void update_id(int last_id)
    {
        if (last_id > id)
            id = last_id;
    }

    static int get_id() {
        return id;
    }


    Flower(){
        id++;
        flower_id = id;
        next = nullptr;
        prev = nullptr;
    }

    Flower(int new_flower_id){
        flower_id = new_flower_id;
        update_id(flower_id);
        next = nullptr;
        prev = nullptr;
    }

    ~Flower(){
        //printf("Destructing %d", flower_id);
        free(name);
        free(description);

        if (prev!=nullptr) {
            prev->next = next;

        }
        else
            headFlowers = next;

        if (next!=nullptr) {
            next->prev = prev;
        }


    }

    int getFlowerId() {
        return flower_id;
    }

    void setFlowerId(int new_flower_id) {
        flower_id=new_flower_id;
    }

    char * getName() {
        return name;
    }

    void setName(char* new_name) {
        name = (char*) malloc(sizeof(new_name)+1);
        strcpy(name, new_name);
    }

    char * getDescription() {
        return description;
    }

    void setDescription(char *new_description) {
        description = (char*) malloc((sizeof(new_description)+1));
        strcpy(description, new_description);
    }

    int getQuantity() {
        return quantity;
    }

    void setQuantity(int new_quantity) {
        quantity = new_quantity;
    }

    double getPrice() {
        return price;
    }

    void setPrice(double new_price) {
        price=new_price;
    }

    char * toString() {
        char *outputstring=(char*) malloc(MAX_LINE_LENGTH*sizeof(char));
        sprintf(outputstring, "%10d|%20s|%20s|%10d|%10.2lf",flower_id, name, description, quantity, price);
        return outputstring;
    }

};

#endif