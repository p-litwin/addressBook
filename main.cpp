#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Contact {
    int id;
    string name, surname, phone, mail, address;
};

void showMainMenu(vector<Contact> &addressBook);
int generateNewContactIdentifier(vector<Contact> &addressBook);
string getMandatoryData(string fieldName);
string getOptionalData(string fieldName);
int findContactIndex(vector<Contact> &addressBook, int id);
Contact splitStringByDelimiter(string textToSplit, string delimiter);
void readAddressBookFromFile(vector<Contact> &addressBook);
void addContactToAddressBook(vector<Contact> &addressBook);
void searchContactsByName(vector<Contact> &addressBook);
void searchContactsBySurname(vector<Contact> &addressBook);
void saveContactToFile(Contact newContact);
void displayContactDetails(vector<Contact> &addressBook, int addressBookIndex);
void displayAllContats(vector<Contact> &addressBook);
void editContact(vector<Contact> &addressBook);
void editName(vector<Contact> &addressBook, int id);
void editSurname(vector<Contact> &addressBook, int id);
void editPhone(vector<Contact> &addressBook, int id);
void editMail(vector<Contact> &addressBook, int id);
void editAddress(vector<Contact> &addressBook, int id);
void updateLineInFile(vector<Contact> &addressBook, int id);
void removeContact(vector<Contact> &addressBook);
void removeLineFromFile(vector<Contact> &addressBook, int id);
void closeProgram();
void pause();

int main() {
    vector<Contact> addressBook;
    readAddressBookFromFile(addressBook);
    showMainMenu(addressBook);
    return 0;
}

void showMainMenu(vector<Contact> &addressBook) {
    char selectedOption;
    do {
        system("cls");
        cout << "###### KSIAZKA ADRESOWA ######" << endl;
        cout << "1. Dodaj nowa osobe" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkie kontakty" << endl;
        cout << "5. Usun kontakt" << endl;
        cout << "6. Edytuj kontakt" << endl;
        cout << "0. Zakoncz program" << endl;
        cout << "Wybierz opcje: ";
        cin >> selectedOption;

        switch(selectedOption) {
        case '1': {
            addContactToAddressBook(addressBook);
            break;
        }
        case '2': {
            searchContactsByName(addressBook);
            break;
        }
        case '3': {
            searchContactsBySurname(addressBook);
            break;
        }
        case '4': {
            displayAllContats(addressBook);
            pause();
            break;
        }
        case '5': {
            removeContact(addressBook);
            pause();
            break;
        }
        case '6': {
            editContact(addressBook);
            break;
        }
        case '0': {
            closeProgram();
        }
        }
    } while (true);
}

int generateNewContactIdentifier(vector<Contact> &addressBook) {
    int newIdentifier = 0;
    if (addressBook.size() == 0) {
        newIdentifier = 1;
    } else if (addressBook.size() == 1) {
        newIdentifier = addressBook[0].id + 1;
    } else {
        newIdentifier = addressBook.back().id + 1;
    }
    return newIdentifier;
}

string getMandatoryData(string fieldName) {
    string userInput;
    do {
        cout << "Podaj " << fieldName << ": ";
        cin.sync();
        getline(cin, userInput);
        if (userInput == "") {
            cout << "Pole " << fieldName << " nie moze byc puste." << endl;
        }
    } while (userInput == "");
    return userInput;
}

string getOptionalData(string fieldName) {
    string userInput;
    cout << "Podaj " << fieldName << ": ";
    cin.sync();
    getline(cin, userInput);
    return userInput;
}

int findContactIndex(vector<Contact> &addressBook, int id) {
    int found = -1;
    for (size_t i = 0; i < addressBook.size(); i++) {
        if (id == addressBook[i].id) {
            return i;
        }
    }
    return found;
}

void readAddressBookFromFile(vector<Contact> &addressBook) {
    fstream file;
    file.open("addressBook.txt", ios::in);
    string lineOfText;
    Contact newContact;

    if (file.good() == true) {
        while (getline(file, lineOfText)) {
            if (lineOfText.size() > 0) {
                newContact = splitStringByDelimiter(lineOfText, "|");
                addressBook.push_back(newContact);
            }
        }
    }
    file.close();

    if (addressBook.size() > 0) {
        cout << "Wczytano kontakty z pliku w ilosci: " << addressBook.size() << endl;
        Sleep(800);
    } else {
        cout << "Plik ksiazki nie istnieje lub jest pusty!" << endl;
        pause();
    }
}

void addContactToAddressBook(vector<Contact> &addressBook) {

    Contact newContact;
    cin.sync();


    newContact.id = generateNewContactIdentifier(addressBook);
    newContact.name = getMandatoryData("imie");
    newContact.surname = getMandatoryData("nazwisko");

    do {
        cout << "Podaj numer telefonu: ";
        getline(cin, newContact.phone);
        cout << "Podaj adres e-mail: ";
        getline(cin, newContact.mail);
        cout << "Podaj adres: ";
        getline(cin, newContact.address);
        if (newContact.phone == "" && newContact.mail == "" && newContact.address == "") {
            cout << "Podaj przynajmniej jedna z tych danych: numer telefonu, mail lub adres." << endl;
        }
    } while(newContact.phone == "" && newContact.mail == "" && newContact.address == "");

    addressBook.push_back(newContact);
    saveContactToFile(newContact);
    cout << "Nowa osoba zostala dodana";

    Sleep(700);
}

void searchContactsByName(vector<Contact> &addressBook) {
    string name;
    char answer;
    int count = 0;
    if (addressBook.size() != 0) {
        do {
            name = getMandatoryData("imie");
            cout << "###### Wyniki wyszukiwania ######" << endl;
            for (size_t i = 0; i < addressBook.size(); i++) {

                if (addressBook[i].name == name) {
                    displayContactDetails(addressBook, i);
                    count++;
                }
            }
            if (count == 0) {
                cout << "Nie znaleziono kontaktow o imieniu: " << name << endl;
            }
            cout << "Czy chcesz wyszukac inne imie? (T/N)";
            cin >> answer;
        } while(answer != 'N' && answer != 'n');
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl;
        pause();
    }
}

void searchContactsBySurname(vector<Contact> &addressBook) {
    string surname;
    char answer;
    int count = 0;
    if (addressBook.size() != 0) {
        do {
            surname = getMandatoryData("nazwisko");
            cout << "###### Wyniki wyszukiwania ######" << endl;
            for (size_t i = 0; i < addressBook.size(); i++) {

                if (addressBook[i].surname == surname) {
                    displayContactDetails(addressBook, i);
                    count++;
                }
            }
            if (count == 0) {
                cout << "Nie znaleziono kontaktow o nazwisku: " << surname << endl;
            }
            cout << "Czy chcesz wyszukac inne nazwisko? (T/N)";
            cin >> answer;
        } while(answer != 'N' && answer != 'n');
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl;
        pause();
    }
}

void saveContactToFile(Contact newContact) {
    fstream addressBookFile;
    addressBookFile.open("addressBook.txt", ios::out | ios::app);
    if (addressBookFile.good() == true) {
        addressBookFile << newContact.id << "|";
        addressBookFile << newContact.name << "|";
        addressBookFile << newContact.surname << "|";
        addressBookFile << newContact.phone << "|";
        addressBookFile << newContact.mail << "|";
        addressBookFile << newContact.address << "|" << endl;
    }
    addressBookFile.close();
}

void displayContactDetails(vector<Contact> &addressBook, int addressBookIndex) {
    cout << "ID: " << addressBook[addressBookIndex].id << endl;
    cout << "Imie i nazwisko: " << addressBook[addressBookIndex].name << " " << addressBook[addressBookIndex].surname << endl;
    cout << "Telefon: " << addressBook[addressBookIndex].phone << endl;
    cout << "Mail: " << addressBook[addressBookIndex].mail << endl;
    cout << "Adres: " << addressBook[addressBookIndex].address << endl;
    cout << endl;
}

void displayAllContats(vector<Contact> &addressBook) {
    if (addressBook.size() != 0) {
        for (size_t i = 0; i < addressBook.size(); i++) {
            displayContactDetails(addressBook, i);
        }
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl;
    }
}

void closeProgram() {
    char key;
    while(key != 'T' && key != 't' && key != 'N' && key != 'n') {
        cout << "Czy chcesz wyjsc z programu? (T/N)";
        cin.sync();
        key = getchar();
        if (key == 'T' || key == 't') {
            exit(0);
        }
    }
}

void pause() {
    cin.sync();
    cout << "Wcisnij ENTER, aby powrocic do menu glownego.";
    getchar();
}

void editContact(vector<Contact> &addressBook) {
    int id;
    char selectedOption;
    displayAllContats(addressBook);
    cout << "Podaj id kontaktu do zmiany: ";
    cin >> id;
    int contactIndex = findContactIndex(addressBook, id);

    if (contactIndex >= 0) {

        do {
            system("cls");
            cout << "###### Edycja kontaktu ######" << endl;
            displayContactDetails(addressBook, contactIndex);
            cout << "1. Zmien imie" << endl;
            cout << "2. Zmien nazwisko" << endl;
            cout << "3. Zmien numer telefonu" << endl;
            cout << "4. Zmien adres email" << endl;
            cout << "5. Zmien adres" << endl;
            cout << "0. Wroc do menu glownego." << endl;
            cout << "Wybierz opcje: ";
            cin >> selectedOption;
            switch(selectedOption) {
            case '1': {
                editName(addressBook, contactIndex);
                break;
            }
            case '2': {
                editSurname(addressBook, contactIndex);
                break;
            }
            case '3': {
                editPhone(addressBook, contactIndex);
                break;
            }
            case '4': {
                editMail(addressBook, contactIndex);
                break;
            }
            case '5': {
                editAddress(addressBook, contactIndex);
                break;
            }
            }
        } while (true && selectedOption != '0');
    } else {
        cout << "Nie ma kontaktu o takim id!" << endl;
        pause();
    }


}

void editName(vector<Contact> &addressBook, int contactIndex) {
    string newName;
    newName = getMandatoryData("imie");
    addressBook[contactIndex].name = newName;
    updateLineInFile(addressBook, addressBook[contactIndex].id);
}

void editSurname(vector<Contact> &addressBook, int contactIndex) {
    string newSurname;
    newSurname = getMandatoryData("nowe nazwisko");
    addressBook[contactIndex].surname = newSurname;
    updateLineInFile(addressBook, addressBook[contactIndex].id);
}

void editPhone(vector<Contact> &addressBook, int contactIndex) {
    string newPhone;
    newPhone = getOptionalData("nowy numer telefonu (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].phone = newPhone;
    updateLineInFile(addressBook, addressBook[contactIndex].id);
}

void editMail(vector<Contact> &addressBook, int contactIndex) {
    string newMail;
    newMail = getOptionalData("nowy adres email (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].mail = newMail;
    updateLineInFile(addressBook, addressBook[contactIndex].id);
}

void editAddress(vector<Contact> &addressBook, int contactIndex) {
    string newAddress;
    newAddress = getOptionalData("nowy adres (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].address = newAddress;
    updateLineInFile(addressBook, addressBook[contactIndex].id);
}

void removeContact(vector<Contact> &addressBook) {
    int id = 0;
    char key;
    displayAllContats(addressBook);
    cout << "Podaj id kontaktu do usuniecia: ";
    cin >> id;
    int contactIndex = findContactIndex(addressBook, id);
    if (contactIndex >= 0) {
        while(key != 'T' && key != 't' && key != 'N' && key != 'n') {
            cout << "Czy na pewno chcesz usunac kontakt o id " << id << "? (T/N)";
            cin.sync();
            key = getchar();

            if (key == 'T' || key == 't') {
                vector<Contact>::iterator it = addressBook.begin() + contactIndex;
                addressBook.erase(it);
                removeLineFromFile(addressBook, id);
                cout << "Kontakt o id " << id << " zostal usuniety z ksiazki adresowej." << endl;
            }
        }
    } else {
        cout << "Nie ma kontaktu o takim id!" << endl;
    }
}

void updateLineInFile(vector<Contact> &addressBook, int id) {

    fstream addressBookFile;
    fstream tempFile;
    string lineOfText = "";
    size_t delimiterPosition;
    int contactIndex = findContactIndex(addressBook, id);

    addressBookFile.open("addressBook.txt");
    tempFile.open("temp.txt", ios::out);

    if (addressBookFile.good() == true) {
        while (getline(addressBookFile, lineOfText)) {
            if (lineOfText.size() > 0) {
                delimiterPosition = lineOfText.find("|");
                if (id != stoi(lineOfText.substr(0, delimiterPosition))) {
                    tempFile << lineOfText << endl;
                } else  {
                    tempFile << addressBook[contactIndex].id << "|";
                    tempFile << addressBook[contactIndex].name << "|";
                    tempFile << addressBook[contactIndex].surname << "|";
                    tempFile << addressBook[contactIndex].phone << "|";
                    tempFile << addressBook[contactIndex].mail << "|";
                    tempFile << addressBook[contactIndex].address << "|" << endl;
                }
            } else {
                tempFile << "";
            }
        }
    }
    addressBookFile.close();
    tempFile.close();
    remove("addressBook.txt");
    rename("temp.txt", "addressBook.txt");

}

void removeLineFromFile(vector<Contact> &addressBook, int id) {

    fstream addressBookFile;
    fstream tempFile;
    string lineOfText = "";
    size_t delimiterPosition = 0;

    addressBookFile.open("addressBook.txt");
    tempFile.open("temp.txt", ios::out);

    if (addressBookFile.good() == true) {

        while (getline(addressBookFile, lineOfText)) {
            delimiterPosition = lineOfText.find("|");
            if (id != stoi(lineOfText.substr(0, delimiterPosition)) && lineOfText.size()>0) {
                tempFile << lineOfText << endl;
            } else {
                tempFile << "";
            }
        }
    }
    addressBookFile.close();
    tempFile.close();
    remove("addressBook.txt");
    rename("temp.txt", "addressBook.txt");
}

Contact splitStringByDelimiter(string textToSplit, string delimiter) {
    size_t delimiterPosition = 0;
    Contact newContact;
    int contactField = 1;
    while ((delimiterPosition = textToSplit.find(delimiter)) != string::npos) {
        switch(contactField) {
        case 1:
            newContact.id = stoi(textToSplit.substr(0, delimiterPosition)) ;
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 2:
            newContact.name = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 3:
            newContact.surname = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 4:
            newContact.phone = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 5:
            newContact.mail = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 6:
            newContact.address = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        }
    }
    return newContact;
}
