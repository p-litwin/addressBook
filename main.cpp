#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct User {
    int id;
    string userName, password;
};

struct Contact {
    int id, userId;
    string name, surname, phone, mail, address;
};

vector<User> users;
vector<Contact> addressBook;

void loadUsersFromFile();
void showLogonMenu();
int userLogin();
//Adding new user functions
void registerNewUser();
bool checkIfUserNameIsAvailable(string userName);
int generateNewUserId();
void addNewUser(string userName, string password);
void saveUserToFile(User newUser);
void displayAddingUserCanceledMessage();
//Adding new user functions
void showMainMenu(int loggedUserId);
string getMandatoryData(string fieldName);
string getOptionalData(string fieldName);
int findContactIndex(int id);
User splitUserDataByDelimiter(string textToSplit, string delimiter);
Contact splitContactDataByDelimiter(string textToSplit, string delimiter);
int readAddressBookFromFile(int loggedUserId);
int addContactToAddressBook(int loggedUserId, int lastContactId);
void searchContactsByName();
void searchContactsBySurname();
void saveContactToFile(Contact newContact);
void displayContactDetails(int addressBookIndex);
void displayAllContats();
void editContact();
void editName(int contactIndex);
void editSurname(int contactIndex);
void editPhone(int contactIndex);
void editMail(int contactIndex);
void editAddress(int contactIndex);
void updateLineInFile(int id);
void removeContact();
void removeLineFromFile(int id);
void closeProgram();
void pause();
void displayAbortMessage();
void logout();

int main() {
    loadUsersFromFile();
    showLogonMenu();
    return 0;
}

void loadUsersFromFile() {
    fstream file;
    file.open("users.txt", ios::in);
    string lineOfText;
    User newUser;

    if (file.good()) {
        while (getline(file, lineOfText)) {
            if (lineOfText.size() > 0) {
                newUser = splitUserDataByDelimiter(lineOfText, "|");
                users.push_back(newUser);
            }
        }
    }
    file.close();
}

void showLogonMenu() {
    int loggedUserId = 0;
    char selectedOption;

    do {
        system("cls");
        cout << "###### KSIAZKA ADRESOWA ######" << endl;
        cout << "Aby korzystac z  programu, nalezy sie zalogowac." << endl;
        cout << "1. Logowanie" << endl;
        cout << "2. Rejestracja" << endl;
        cout << "0. Zakoncz program" << endl;
        cout << "Wybierz opcje: ";
        cin >> selectedOption;

        switch(selectedOption) {
        case '1': {
            loggedUserId = userLogin();
            if (loggedUserId != 0) {
                showMainMenu(loggedUserId);
            } else {
                cout << "Dane logowanie nieprawidlowe." << endl;
                system("pause");
            }
            break;
        }
        case '2': {
            registerNewUser();
            break;
        }
        case '0': {
            closeProgram();
        }
        }
    } while (true);

}

int userLogin() {
    string userName, password;
    cout << "Nazwa uzytkownika: ";
    cin.sync();
    getline(cin, userName);
    cout << "Haslo: ";
    cin.sync();
    getline(cin, password);
    for (unsigned int i = 0; i < users.size(); i++) {
        if (userName == users[i].userName && password == users[i].password) {
            return users[i].id;
        }
    }
    return 0;
}


void showMainMenu(int loggedUserId) {
    int lastContactId = 0;
    lastContactId = readAddressBookFromFile(loggedUserId);
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
        cout << "7. Zmien haslo" << endl;
        cout << "0. Wyloguj sie" << endl;
        cout << "Wybierz opcje: ";
        cin >> selectedOption;

        switch(selectedOption) {
        case '1': {
            lastContactId = addContactToAddressBook(loggedUserId, lastContactId);
            break;
        }
        case '2': {
            searchContactsByName();
            break;
        }
        case '3': {
            searchContactsBySurname();
            break;
        }
        case '4': {
            displayAllContats();
            pause();
            break;
        }
        case '5': {
            removeContact();
            pause();
            break;
        }
        case '6': {
            editContact();
            break;
        }
        case '0': {
            logout();
        }
        }
    } while (true);
}

string getMandatoryData(string fieldName) {
    string userInput;
    do {
        cout << fieldName << ": ";
        cin.sync();
        getline(cin, userInput);
        if (userInput == "") {
            cout << "Pole '" << fieldName << "' nie moze byc puste." << endl;
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

int findContactIndex(int id) {
    int found = -1;
    for (size_t i = 0; i < addressBook.size(); i++) {
        if (id == addressBook[i].id) {
            return i;
        }
    }
    return found;
}

int readAddressBookFromFile(int loggedUserId) {
    fstream file;
    file.open("addressBook.txt", ios::in);
    string lineOfText;
    Contact newContact;
    int lastContactId = 0;

    if (file.good() == true) {
        while (getline(file, lineOfText)) {
            if (lineOfText.size() > 0) {
                newContact = splitContactDataByDelimiter(lineOfText, "|");
                if (newContact.userId == loggedUserId) {
                    addressBook.push_back(newContact);
                }
            }
            lastContactId = newContact.id;
        }
    }
    file.close();

    if (!addressBook.empty()) {
        cout << "Wczytano kontakty z pliku w ilosci: " << addressBook.size() << endl;
        Sleep(600);
    } else {
        cout << "Ksiazka adresowa jest pusta!" << endl;
        pause();
    }
    return lastContactId;
}

int addContactToAddressBook(int loggedUserId, int lastContactId) {

    Contact newContact;
    cin.sync();
    lastContactId++;
    newContact.id = lastContactId;
    newContact.userId = loggedUserId;
    newContact.name = getMandatoryData("Imie");
    newContact.surname = getMandatoryData("Nazwisko");

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
    return lastContactId;
}

void searchContactsByName() {
    string name;
    char answer;
    int count = 0;
    if (addressBook.size() != 0) {
        do {
            name = getMandatoryData("imie");
            cout << "###### Wyniki wyszukiwania ######" << endl;
            for (size_t i = 0; i < addressBook.size(); i++) {

                if (addressBook[i].name == name) {
                    displayContactDetails(i);
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

void searchContactsBySurname() {
    string surname;
    char answer;
    int count = 0;
    if (!addressBook.empty()) {
        do {
            surname = getMandatoryData("nazwisko");
            cout << "###### Wyniki wyszukiwania ######" << endl;
            for (size_t i = 0; i < addressBook.size(); i++) {

                if (addressBook[i].surname == surname) {
                    displayContactDetails(i);
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
        addressBookFile << newContact.userId << "|";
        addressBookFile << newContact.name << "|";
        addressBookFile << newContact.surname << "|";
        addressBookFile << newContact.phone << "|";
        addressBookFile << newContact.mail << "|";
        addressBookFile << newContact.address << "|" << endl;
    }
    addressBookFile.close();
}

void displayContactDetails(int addressBookIndex) {
    cout << "ID: " << addressBook[addressBookIndex].id << endl;
    cout << "Imie i nazwisko: " << addressBook[addressBookIndex].name << " " << addressBook[addressBookIndex].surname << endl;
    cout << "Telefon: " << addressBook[addressBookIndex].phone << endl;
    cout << "Mail: " << addressBook[addressBookIndex].mail << endl;
    cout << "Adres: " << addressBook[addressBookIndex].address << endl;
    cout << endl;
}

void displayAllContats() {
    if (addressBook.size() != 0) {
        for (size_t i = 0; i < addressBook.size(); i++) {
            displayContactDetails(i);
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

void editContact() {
    int id;
    char selectedOption;
    displayAllContats();
    cout << "Podaj id kontaktu do zmiany: ";
    cin >> id;
    int contactIndex = findContactIndex(id);

    if (contactIndex >= 0) {

        do {
            system("cls");
            cout << "###### Edycja kontaktu ######" << endl;
            displayContactDetails(contactIndex);
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
                editName(contactIndex);
                break;
            }
            case '2': {
                editSurname(contactIndex);
                break;
            }
            case '3': {
                editPhone(contactIndex);
                break;
            }
            case '4': {
                editMail(contactIndex);
                break;
            }
            case '5': {
                editAddress(contactIndex);
                break;
            }
            }
        } while (true && selectedOption != '0');
    } else {
        cout << "Nie ma kontaktu o takim id!" << endl;
        pause();
    }


}

void editName(int contactIndex) {
    string newName;
    newName = getMandatoryData("Imie");
    addressBook[contactIndex].name = newName;
    updateLineInFile(addressBook[contactIndex].id);
}

void editSurname(int contactIndex) {
    string newSurname;
    newSurname = getMandatoryData("Nowe nazwisko");
    addressBook[contactIndex].surname = newSurname;
    updateLineInFile(addressBook[contactIndex].id);
}

void editPhone(int contactIndex) {
    string newPhone;
    newPhone = getOptionalData("Nowy numer telefonu: (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].phone = newPhone;
    updateLineInFile(addressBook[contactIndex].id);
}

void editMail(int contactIndex) {
    string newMail;
    newMail = getOptionalData("Nowy adres email (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].mail = newMail;
    updateLineInFile(addressBook[contactIndex].id);
}

void editAddress(int contactIndex) {
    string newAddress;
    newAddress = getOptionalData("Nowy adres (lub wcisnij ENTER aby usunac)");
    addressBook[contactIndex].address = newAddress;
    updateLineInFile(addressBook[contactIndex].id);
}

void removeContact() {
    int id = 0;
    char key;
    displayAllContats();
    cout << "Podaj id kontaktu do usuniecia: ";
    cin >> id;
    int contactIndex = findContactIndex(id);
    if (contactIndex >= 0) {
        while(key != 'T' && key != 't' && key != 'N' && key != 'n') {
            cout << "Czy na pewno chcesz usunac kontakt o id " << id << "? (T/N)";
            cin.sync();
            key = getchar();

            if (key == 'T' || key == 't') {
                vector<Contact>::iterator it = addressBook.begin() + contactIndex;
                addressBook.erase(it);
                removeLineFromFile(id);
                cout << "Kontakt o id " << id << " zostal usuniety z ksiazki adresowej." << endl;
            }
        }
    } else {
        cout << "Nie ma kontaktu o takim id!" << endl;
    }
}

void updateLineInFile(int id) {

    fstream addressBookFile;
    fstream tempFile;
    string lineOfText = "";
    size_t delimiterPosition;
    int contactIndex = findContactIndex(id);

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
                    tempFile << addressBook[contactIndex].userId << "|";
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

void removeLineFromFile(int id) {

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

User splitUserDataByDelimiter(string textToSplit, string delimiter) {
    size_t delimiterPosition = 0;
    User newUser;
    int userField = 1;
    while ((delimiterPosition = textToSplit.find(delimiter)) != string::npos) {
        switch(userField) {
        case 1:
            newUser.id = stoi(textToSplit.substr(0, delimiterPosition)) ;
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            userField++;
            break;
        case 2:
            newUser.userName = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            userField++;
            break;
        case 3:
            newUser.password = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            userField++;
            break;
        }
    }
    return newUser;
}

Contact splitContactDataByDelimiter(string textToSplit, string delimiter) {
    size_t delimiterPosition = 0;
    Contact newContact;
    int contactField = 1;
    while ((delimiterPosition = textToSplit.find(delimiter)) != string::npos) {
        switch(contactField) {
        case 1:
            newContact.id = stoi(textToSplit.substr(0, delimiterPosition));
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 2:
            newContact.userId = stoi(textToSplit.substr(0, delimiterPosition));
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 3:
            newContact.name = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 4:
            newContact.surname = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 5:
            newContact.phone = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 6:
            newContact.mail = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        case 7:
            newContact.address = textToSplit.substr(0, delimiterPosition);
            textToSplit.erase(0, delimiterPosition + delimiter.length());
            contactField++;
            break;
        }
    }
    return newContact;
}
// Adding new user functions
void registerNewUser() {
    string userName, password;
    bool userNameAlreadyTaken;

    do {
        displayAbortMessage();
        userName = getMandatoryData("Nazwa uzytkownika");
        userNameAlreadyTaken = checkIfUserNameIsAvailable(userName);
        if (userNameAlreadyTaken) {
            cout << "Nazwa uzytkownika zajeta, podaj inna." << endl;
        }
    } while (userNameAlreadyTaken && userName != "0");

    if (userName != "0") {
        displayAbortMessage();
        password = getMandatoryData("Haslo");
        if (password != "0") {
            addNewUser(userName, password);
            pause();
        }
    }
    if (userName == "0" || password == "0") {
        displayAddingUserCanceledMessage();
        pause();
    }
}

bool checkIfUserNameIsAvailable(string userName) {
    for (unsigned int i = 0; i < users.size(); i++) {
        if (userName == users[i].userName) {
            return true;
        }
    }
    return false;
}

int generateNewUserId() {
    int newId = 0;
    if (users.size() == 0) {
        newId = 1;
    } else {
        newId = users.back().id + 1;
    }
    return newId;
}

void addNewUser(string userName, string password) {
    User newUser;
    newUser.id = generateNewUserId();
    newUser.userName = userName;
    newUser.password = password;
    users.push_back(newUser);
    saveUserToFile(newUser);
    cout << "Nowy uzytkownik zostal dodany!" << endl;
}

void saveUserToFile(User newUser) {
    fstream usersFile;
    usersFile.open("users.txt", ios::out | ios::app);
    if (usersFile.good()) {
        usersFile << newUser.id << "|";
        usersFile << newUser.userName << "|";
        usersFile << newUser.password << "|" << endl;
    }
    usersFile.close();
}

void displayAddingUserCanceledMessage() {
    cout << "Dodawanie nowego uzytkonika zostalo przerwane." << endl;
}

void displayAbortMessage() {
    cout << "Wpisz '0' aby anulowac." << endl;
}

void logout() {
    addressBook.clear();
    showLogonMenu();
}
