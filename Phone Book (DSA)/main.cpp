#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;

class ConInfo {
public:
    string name;
    string address;
    string relation;

    ConInfo(const string& name, const string& address, const string& relation)
        : name(name), address(address), relation(relation) {}
};

class HashTable {
private:
    static const int hashGroups = 10;
    list<pair<string, ConInfo>> table[hashGroups];

public:
    int getHashGroups()const {return hashGroups; }
    const list<pair<string, ConInfo>>& getTable(int index) const {return table[index];}
    bool isEmpty();
    int hashFunction(const string& key);
    void insertItem(const string& key, const ConInfo& contactInformation);
    bool removeItem(string& name);
    const ConInfo* searchTable(const string& key);
    void printTable();
};

bool HashTable::isEmpty() {
    int sum{};
    for (int i{}; i < hashGroups; i++) {
        sum += table[i].size();
    }

    return sum == 0;
}

int HashTable::hashFunction(const string& key) {
    int num = 0;
    for (char ch : key) {
        num += ch;
    }
    return num % hashGroups;
}

void HashTable::insertItem(const string& key, const ConInfo& contactInformation) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;
    for (; bItr != end(cell); bItr++) {
        if (bItr->second.name == key) {
            keyExists = true;
            *bItr = make_pair(key, contactInformation);
            cout << "Updated Contact!.." << endl;
            break;
        }
    }

    if (!keyExists) {
        cell.emplace_back(key, contactInformation);
    }
}

bool HashTable::removeItem(string& name) {
    for(int i=0;i<hashGroups;i++){
        auto& cell = table[i];
        for (auto it = cell.begin(); it != cell.end(); ++it){
            if(it-> second.name==name){
                cell.erase(it);
                return true;
            }
        }
    }
    return false;

}
const ConInfo* HashTable::searchTable(const string& key) {
    for (int i = 0; i < hashGroups; i++) {
        for (const auto& pair : table[i]) {
            if (pair.second.name == key) {
                return &(pair.second);
            }
        }
    }
    return nullptr;
}

void HashTable::printTable() {

    bool load = true;

    for (int i = 0; i < hashGroups; i++) {
        if (!table[i].empty()) {
            load = false;
            break;
        }
    }

    if (load) {
        cout << "No Contacts found, start to add Contacts now!\n";
        return;
    }

    const int padding = 2;

    int maxnumWidth = 15;
    int maxnameWidth = 20;
    int maxaddressWidth = 20;
    int maxrelationWidth = 15;

    for (int i = 0; i < hashGroups; i++) {
        for (const auto& pair : table[i]) {
            maxnumWidth = max(maxnumWidth, static_cast<int>(pair.first.length()));
            maxnameWidth = max(maxnameWidth, static_cast<int>(pair.second.name.length()));
            maxaddressWidth = max(maxaddressWidth, static_cast<int>(pair.second.address.length()));
            maxrelationWidth = max(maxrelationWidth, static_cast<int>(pair.second.relation.length()));
        }
    }

    cout << "------------------------------------------------------------------" << endl;
    cout << left;
    cout << setw(maxnumWidth + padding) << setfill(' ') << "Contact No."
        << setw(maxnameWidth + padding) << setfill(' ') << "Name"
        << setw(maxaddressWidth + padding) << setfill(' ') << "Address"
        << setw(maxrelationWidth + padding) << setfill(' ') << "Relation" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    cout << left;
    for (int i = 0; i < hashGroups; i++) {
        for (const auto& pair : table[i]) {
            cout << setw(maxnumWidth + padding) << setfill(' ') << pair.first
                << setw(maxnameWidth + padding) << setfill(' ') << pair.second.name
                << setw(maxaddressWidth + padding) << setfill(' ') << pair.second.address
                << setw(maxrelationWidth + padding) << setfill(' ') << pair.second.relation << endl;
        }
    }
    cout << "-----------------------------------------------------------------" << endl;
}
void saveContactsToFile(const HashTable& phoneBook, const string& fileName) {
    ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        int hashGroups = phoneBook.getHashGroups();
        for (int i = 0; i < hashGroups; i++) {
            const list<pair<string, ConInfo>>& cell = phoneBook.getTable(i);
            for (const auto& pair : cell) {
                outputFile << pair.first << ","
                           << pair.second.name << ","
                           << pair.second.address << ","
                           << pair.second.relation << "\n";
            }
        }
        outputFile.close();
        cout << "Phone book data saved to file: " << fileName << endl;
    } else {
        cout << "Error: Unable to open the file " << fileName << endl;
    }
}
void loadContactsFromFile(HashTable& phoneBook, const string& fileName) {
    ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            size_t commaPos1 = line.find(",");
            size_t commaPos2 = line.find(",", commaPos1 + 1);
            size_t commaPos3 = line.find(",", commaPos2 + 1);
            if (commaPos1 != string::npos && commaPos2 != string::npos && commaPos3 != string::npos) {
                string contactNumber = line.substr(0, commaPos1);
                string name = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                string address = line.substr(commaPos2 + 1, commaPos3 - commaPos2 - 1);
                string relation = line.substr(commaPos3 + 1);
                ConInfo contact(name, address, relation);
                phoneBook.insertItem(contactNumber, contact);
            }
        }
        inputFile.close();
        cout << "Phone book data loaded from file: " << fileName << endl;
    } else {
        cout << "File not found. Creating a new file when saving contacts." << endl;
    }
}

int main() {
    HashTable HT;
    bool start = true;
    // Load data from a file if it exists
    loadContactsFromFile(HT, "PhoneBook Contacts.txt");

    while (start) {
        cout << "================================================" << endl;
        cout << "Phone Book: Save Contacts, Connect with Others!\n";
        cout << "================================================" << endl;
        cout << "1. Add Contacts\n";
        cout << "2. Edit Contacts\n";
        cout << "3. Delete Contacts\n";
        cout << "4. Search Contacts\n";
        cout << "5. Display all Contacts\n";
        cout << "6. Save and Exit\n";

        int choice;
        cout << "----------------------------------" << endl;
        cout << "Enter the number of your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("cls");
                string contactNumber;
                string name, address, relation;
                cout << "============================" << endl;
                cout << "Let's create a new contact!\n";
                cout << "============================" << endl;
                cout << "Name: ";
                cin.ignore();
                getline(cin, name);
                if (name.empty()) {
                    cout << "[ERROR] Blank input is not allowed.\n";
                    break;
                }
                cout << "Contact number: ";
                getline(cin, contactNumber);
                if (contactNumber.empty()) {
                    cout << "[ERROR] Blank input is not allowed.\n";
                    break;
                }
                cout << "Address: ";
                getline(cin, address);
                if (address.empty()) {
                    cout << "[ERROR] Blank input is not allowed.\n";
                    break;
                }
                cout << "Relation: ";
                getline(cin, relation);
                if (relation.empty()) {
                    cout << "[ERROR] Blank input is not allowed.\n";
                    break;
                }

                ConInfo contactInformation(name, address, relation);
                HT.insertItem(contactNumber, contactInformation);
                cout << "New Contact Added!\n";
                break;
            }
            case 2: {
                system("cls");
                string name;
                cout << "============================" << endl;
                cout << "Edit Contact Information\n";
                cout << "============================" << endl;
                cout << "Enter the Name to be edited: ";
                cin.ignore();
                getline(cin, name);

                const ConInfo* contact = HT.searchTable(name);
                if (contact) {
                    HT.removeItem(name);
                    string newConNum, newAddress, newRelation;
                    cout << "Enter new Contact Number: ";
                    cin >> newConNum;
                    cout << "Enter new address: ";
                    cin.ignore(); // Added this to clear the input buffer
                    getline(cin, newAddress);
                    cout << "Enter new relation: ";
                    getline(cin, newRelation);

                    ConInfo updatedContact(name, newAddress, newRelation);
                    HT.insertItem(newConNum, updatedContact);
                    cout << "Contact information updated!\n";
                } else {
                    cout << "Contact not found!\n";
                }
                break;
            }
            case 3: {
                system("cls");
                string name;
                cout << "============================" << endl;
                cout << "Delete Contact Information\n";
                cout << "============================" << endl;
                cout << "Enter the Name to be deleted: ";
                cin.ignore();
                getline(cin, name);

                if (HT.removeItem(name)) {
                    cout << "Contact Successfully Deleted!\n";
                } else {
                    cout << "Contact not found!\n";
                }
                break;
            }
            case 4: {
                system("cls");
                string name;
                cout << "============================" << endl;
                cout << "Search Contact Information\n";
                cout << "============================" << endl;
                cout << "Enter the Name to search: ";
                cin.ignore();
                getline(cin, name);

                if (name.empty()) {
                    cout << "[ERROR] Blank input is not allowed.\n";
                    break;
                }

                const ConInfo* contact = HT.searchTable(name);
                if (contact) {
                    cout << "---------------------------------" << endl;
                    cout << "CONTACT FOUND:\n";
                    cout << "Name: " << contact->name << endl;
                    cout << "Address: " << contact->address << endl;
                    cout << "Relation: " << contact->relation << endl;
                    cout << "---------------------------------" << endl;

                } else {
                    cout << "Contact not found!\n";
                }
                break;
            }
            case 5: {
                system("cls");
                HT.printTable();
                break;
            }
            case 6: {
                start = false;
                cout << "Exiting the program...\n";
                // Save data to a file when the user chooses to exit
                saveContactsToFile(HT, "PhoneBook Contacts.txt");
                break;
            }
            default: {
                cout << "Invalid Choice, Please Try Again\n";
                break;
            }
        }
    }
    return 0;
}
