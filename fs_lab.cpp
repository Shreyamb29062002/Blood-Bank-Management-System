#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <algorithm>  // For transform() function
#include <cctype>     // For tolower() function
#include <ctime>      // For generating unique IDs based on current time

using namespace std;

const int MAX_RECORDS = 100;  // Maximum number of records

class BloodBank {
    char name[50], contact[15], addr[50], bgrp[20], gender[8];
    int id, age;

public:
    BloodBank() {
        id = -1;
    }

    void accept() {
        cin.ignore();
        cout << "\nEnter the name of the donor: ";
        cin.getline(name, sizeof(name));

        cout << "Enter the address: ";
        cin.getline(addr, sizeof(addr));

        bool isUniqueNumber = false;
        while (!isUniqueNumber) {
            cout << "Enter the contact number: ";
            cin.getline(contact, sizeof(contact));

            if (!isValidPhoneNumber(contact)) {
                cout << "Invalid phone number format. Please enter a valid number.\n";
            } else {
                // Validate uniqueness of the phone number
                ifstream file("Data.DAT", ios::in | ios::binary);
                bool isPhoneNumberUnique = true;
                BloodBank obj; // Declare an instance of the BloodBank class

                while (file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
                    if (strcmp(obj.contact, contact) == 0) {
                        isPhoneNumberUnique = false;
                        break;
                    }
                }

                file.close();

                if (!isPhoneNumberUnique) {
                    cout << "Phone number is already in use. Please enter a different number.\n";
                } else {
                    isUniqueNumber = true;
                }
            }
        }

        cout << "Enter the gender: ";
        cin.getline(gender, sizeof(gender));

        cout << "Enter the age: ";
        while (!(cin >> age) || age < 0) {
            cout << "Invalid input. Please enter a non-negative integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter the blood group: ";
        cin.ignore();
        cin.getline(bgrp, sizeof(bgrp));

        transform(bgrp, bgrp + sizeof(bgrp), bgrp, [](unsigned char c) { return tolower(c); });

        // Check if the blood group is valid using case-insensitive comparison
        while (strcasecmp(bgrp, "o positive") != 0 && strcasecmp(bgrp, "o negative") != 0 &&
               strcasecmp(bgrp, "a positive") != 0 && strcasecmp(bgrp, "a negative") != 0 &&
               strcasecmp(bgrp, "b positive") != 0 && strcasecmp(bgrp, "b negative") != 0 &&
               strcasecmp(bgrp, "ab positive") != 0 && strcasecmp(bgrp, "ab negative") != 0) {
            cout << "Invalid blood group. Please enter a valid blood group: ";
            cin.getline(bgrp, sizeof(bgrp));
            transform(bgrp, bgrp + sizeof(bgrp), bgrp, [](unsigned char c) { return tolower(c); });
        }
    }

    void display() {
        cout << "\nName            : " << name;
        cout << "\nBlood Group     : " << bgrp;
        cout << "\nGender          : " << gender;
        cout << "\nAge             : " << age;
        cout << "\nID Number       : " << id;
        cout << "\nAddress         : " << addr;
        cout << "\nContact Number  : " << contact << endl;
    }

    void setID(int uniqueID) {
        id = uniqueID;
    }

    int getId() {
        return id;
    }

    char* getBloodGroup() {
        return bgrp;
    }

private:
    bool isValidPhoneNumber(const string& phoneNumber) {
        // Phone number validation criteria (example):
        // - Must consist of only digits
        // - Must have a certain length

        // Validate length
        if (phoneNumber.length() != 10) {
            return false;
        }

        // Validate digit characters
        for (char c : phoneNumber) {
            if (!isdigit(c)) {
                return false;
            }
        }

        return true;
    }
};

// Rest of the code remains the same...


int hashFunction(int id) {
    return id % MAX_RECORDS;  // Simple modulo-based hashing
}

int generateUniqueID() {
    srand(time(0));  // Seed the random number generator with current time
    return rand() % 10000 + 1;  // Generate a random ID between 1 and 10000
}

void createRecord() {
    int n;
    BloodBank obj;

    cout << "\nEnter the number of donors: ";
    while (!(cin >> n) || n < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(); // Ignore the newline character left in the input buffer

    ofstream file("Data.DAT", ios::out | ios::binary);

    for (int i = 0; i < n; i++) {
        cout << "\nEnter data for Donor #" << (i + 1) << ":";
        obj.accept();

        int uniqueID = generateUniqueID();
        obj.setID(uniqueID);

        int index = hashFunction(obj.getId());
        file.seekp(index * sizeof(obj), ios::beg);
        file.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
    }

    file.close();
    cout << "Record(s) created successfully." << endl;
}

void displayAllRecords() {
    BloodBank obj;

    ifstream file("Data.DAT", ios::in | ios::binary);

    bool isEmpty = true;  // Flag to check if the file is empty

    while (file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
        if (obj.getId() != -1) {
            cout << "\nRecord details:" << endl;
            obj.display();
            isEmpty = false;
        }
    }

    file.close();

    if (isEmpty) {
        cout << "\nNo records found." << endl;
    }
}

void addRecord() {
    BloodBank obj;

    ofstream file("Data.DAT", ios::app | ios::binary);

    cout << "\nEnter the data for the new donor:";
    obj.accept();

    int uniqueID = generateUniqueID();
    obj.setID(uniqueID);

    int index = hashFunction(obj.getId());
    file.seekp(index * sizeof(obj), ios::beg);
    file.write(reinterpret_cast<const char*>(&obj), sizeof(obj));

    file.close();
    cout << "Record added successfully." << endl;
}

void searchRecord() {
    int id_search;
    BloodBank obj;

    cout << "\nEnter the ID number to search for: ";
    while (!(cin >> id_search) || id_search < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    ifstream file("Data.DAT", ios::in | ios::binary);

    while (file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
        if (obj.getId() == id_search && obj.getId() != -1) {
            cout << "\nRecord found:";
            obj.display();
            file.close();
            return;
        }
    }

    file.close();
    cout << "\nNo record found with the given ID number." << endl;
}

void updateRecord() {
    int id_update;
    BloodBank obj;

    cout << "\nEnter the ID number to update the record: ";
    while (!(cin >> id_update) || id_update < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    fstream file("Data.DAT", ios::in | ios::out | ios::binary);

    while (file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
        if (obj.getId() == id_update && obj.getId() != -1) {
            cout << "\nCurrent record:";
            obj.display();

            cout << "\nEnter the new data for the donor:";
            obj.accept();

            int pos = file.tellg();
            file.seekp(pos - sizeof(obj), ios::beg);
            file.write(reinterpret_cast<const char*>(&obj), sizeof(obj));

            file.close();
            cout << "Record updated successfully." << endl;
            return;
        }
    }

    file.close();
    cout << "\nNo record found with the given ID number." << endl;
}

void deleteRecord() {
    int id_delete;
    BloodBank obj;

    cout << "\nEnter the ID number to delete the record: ";
    while (!(cin >> id_delete) || id_delete < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    fstream file("Data.DAT", ios::in | ios::out | ios::binary);

    while (file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
        if (obj.getId() == id_delete && obj.getId() != -1) {
            cout << "\nRecord to be deleted:";
            obj.display();

            obj.setID(-1);  // Mark the record as deleted

            int pos = file.tellg();
            file.seekp(pos - sizeof(obj), ios::beg);
            file.write(reinterpret_cast<const char*>(&obj), sizeof(obj));

            file.close();
            cout << "Record deleted successfully." << endl;
            return;
        }
    }

    file.close();
    cout << "\nNo record found with the given ID number." << endl;
}

int main() {
    int ch;

    while (true) {
        cout << "\nBlood Bank Management System\n";
        cout << "-----------------------------\n";
        cout << "1. Create new record(s)\n";
        cout << "2. Display all records\n";
        cout << "3. Add a record\n";
        cout << "4. Search for a record\n";
        cout << "5. Update a record\n";
        cout << "6. Delete a record\n";
        cout << "7. Exit\n";
        cout << "-----------------------------\n";
        cout << "Enter your choice (1-7): ";

        while (!(cin >> ch) || ch < 1 || ch > 7) {
            cout << "Invalid input. Please enter an integer from 1 to 7: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (ch) {
            case 1:
                createRecord();
                break;
            case 2:
                displayAllRecords();
                break;
            case 3:
                addRecord();
                break;
            case 4:
                searchRecord();
                break;
            case 5:
                updateRecord();
                break;
            case 6:
                deleteRecord();
                break;
            case 7:
                return 0;
        }
    }

    return 0;
    }
