/*
** Program: Project 12
** Description: Build upon project 11 and use exception handling
** Course: CMSC226 CRN 32842
** Professor: Huseyin Aygun
** Student: Izzat Ismailov
** Due Date: 05/11/2025
*/

#ifdef _MSC_VER //making my code portable so this define is skipped outside of microsoft visual studio compilation
#define _CRT_SECURE_NO_WARNINGS //my microsoft visual studio is saying i cant use strncpy or strcpy so i defined this so it can also compile in linux (and not just mcvs) it was saying i had to use strncpy_s but that wouldn't compile in linux or anywhere else
#endif
#include <iostream>
#include <new>
#include <iomanip> 
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype> 
#include <fstream>
#include <filesystem>
using namespace std; //put this before the include directive
#include "mainmenu_Pr12.h" //this works 

fstream inventory; //global file stream object inventory 
string fileOpenError = "ERROR! Cannot Open The Inventory File! Terminating Program!"; //global string that contains file open error message 

//now single file program so must combine all cpps when submitting 
class BookData { //BookData base class 
private: //private member variables 

    char bookTitle[51];  //51 represents the max char (50 + null terminator)               
    char isbn[14];
    char author[31];
    char publisher[31];


public: //public member functions 
    void setTitle(char* stringPtr) { //the following inline set functions are for copying the user input/arguments to the respective member variables 
        strncpy(bookTitle, stringPtr, 51);
    }

    void setISBN(char* stringPtr) {
        strncpy(isbn, stringPtr, 14);
    }

    void setAuthor(char* stringPtr) {
        strncpy(author, stringPtr, 31);
    }

    void setPub(char* stringPtr) {
        strncpy(publisher, stringPtr, 31);
    }

    const char* getTitle() const { //following are just accessor functions for getting/returning the member variables to the lookup function etc (display)
        return bookTitle; //const after the function paranthesis makes sure the function doesn't modify the member variables
        //const char* before getTitle makes sure that the returned value is not modified (only for display)
    }

    const char* getISBN() const {
        return isbn;
    }

    const char* getAuthor() const {
        return author;
    }

    const char* getPub() const {
        return publisher;
    }
};

class InventoryBook : public BookData { //derived from BookData class 
private:
    int qtyOnHand;
    double wholesale;
    double retail;
    char dateAdded[11];

public:
    void setDateAdded(char* stringPtr) {
        strncpy(dateAdded, stringPtr, 11);
    }

    void setQty(int userQuantity) {
        qtyOnHand = userQuantity;
    }

    void setWholesale(double userWholesale) {
        wholesale = userWholesale;
    }

    void setRetail(double userRetail) {
        retail = userRetail;
    }

    int isEmpty(int subscript) const; //prototype here

    void removeBook(int subscript); //protoype here 

    bool bookMatch(string userPartialTitle, int subscript) const; //prototype

    const char* getDateAdded() const {
        return dateAdded;
    }

    int getQty() const {
        return qtyOnHand;
    }

    double getWholesale() const {
        return wholesale;
    }

    double getRetail() const {
        return retail;
    }
};

InventoryBook book[20]; //the derived class declaration 

bool InventoryBook::bookMatch(string userPartialTitle, int subscript) const { //checking if there is no book there, made const bc it doesnt change anything and is not supposed to 
    const char* titlePtr = nullptr; //pointer variable to test the books with strstr, made constant now so it doesnt modify what it points to 
    titlePtr = strstr(book[subscript].getTitle(), userPartialTitle.c_str()); //converting to c string because of the string parameter being passed (doesn't matter if it returns const char*, we are not changing it)

    if (titlePtr != nullptr) { //checking if titlePtr points to something (return true if yes)
        return true;
    }

    else {
        return false;
    }
}

int InventoryBook::isEmpty(int subscript) const { //checking if a book does not exist there 
    if (book[subscript].getTitle()[0] == '\0')
        return 1;
    else
        return 0;
}

void InventoryBook::removeBook(int subscript) { //removing the book, declaring outside because I am getting errors when trying to write to the file using 'book' as it is not declared yet
    char setNull[1] = {}; //array with just null terminator 
    book[subscript].setTitle(setNull); //passing null terminator so book gets removed/first char is null terminator

    inventory.seekp(sizeof(book[subscript]) * subscript, ios::beg); //write to the file
    inventory.write(reinterpret_cast<char*>(&book[subscript]), sizeof(book[subscript]));
}

class SoldBook : public InventoryBook { //SoldBook class for cashier, derived from inventory book class 
private:
    static double taxRate; //following private variables are for calculating a sale 
    int qtySold = 0;
    double tax = 0;
    double subtotal = 0;
    static double total;

public:
    void setQtySold(int userQty) { //following member functions for setting and getting the member variables 
        qtySold = userQty;
    }

    void setTax(int subscript) {
        tax = taxRate * qtySold * book[subscript].getRetail();
    }

    void setSubtotal(double userSubtotal) {
        subtotal = userSubtotal + tax;
    }

    void setTotal() {
        total += subtotal;
    }

    double getTaxRate() const {
        return taxRate;
    }

    double getSubtotal() const {
        return subtotal;
    }

    double getTotal() const {
        return total;
    }

    double getTax() const {
        return tax;
    }

    int getQtySold() const {
        return qtySold;
    }
};

double SoldBook::taxRate = 0.06; //initializing the static member variables 
double SoldBook::total = 0;


class Menu { //class for displaying the various menus 
public:
    void displayMainMenu() { //member functions display the main menu, inv menu, and reports menu
        cout << "\n" << setw(30) << "Serendipity Booksellers" << endl;
        cout << setw(22) << "Main Menu" << endl << endl;
        cout << setw(23) << "1. Cashier Module" << endl;
        cout << setw(34) << "2. Inventory Database Module" << endl;
        cout << setw(22) << "3. Report Module" << endl;
        cout << setw(13) << "4. Exit" << endl << endl;
        cout << setw(25) << "Enter Your Choice: ";
    }

    void displayInvMenu() {
        cout << setw(30) << "Serendipity Booksellers" << endl; //displaying menu 
        cout << setw(27) << "Inventory Database" << endl << endl;
        cout << setw(23) << "1. Look up a Book" << endl;
        cout << setw(19) << "2. Add a Book" << endl;
        cout << setw(29) << "3. Edit a Book's record" << endl;
        cout << setw(22) << "4. Delete a Book" << endl;
        cout << setw(32) << "5. Return to the Main Menu" << endl << endl;
        cout << setw(25) << "Enter Your Choice: ";
    }

    void displayReports() {
        cout << setw(30) << "Serendipity Booksellers" << endl; //display menu 
        cout << setw(22) << "Reports" << endl << endl;
        cout << setw(26) << "1. Inventory Listing" << endl;
        cout << setw(34) << "2. Inventory Wholesale Value" << endl;
        cout << setw(31) << "3. Inventory Retail Value" << endl;
        cout << setw(28) << "4. Listing by Quantity" << endl;
        cout << setw(24) << "5. Listing by Cost" << endl;
        cout << setw(23) << "6. Listing by Age" << endl;
        cout << setw(28) << "7. Return to Main Menu" << endl << endl;
        cout << setw(25) << "Enter your Choice: ";
    }
};

Menu menus; //declaring class object 

class InputValidation { //class for validating user input 
private:
    int mainMenuChoice; //various user choices for each menu
    int invMenuChoice;
    int reportsChoice;

public:
    void setMainMenuChoice(int userChoice) { //setter functions 
        mainMenuChoice = userChoice;
    }

    void setInvMenuChoice(int userChoice) {
        invMenuChoice = userChoice;
    }

    void setReportsChoice(int userChoice) {
        reportsChoice = userChoice;
    }

    void validateMainMenuChoice() { //following member functions validate the user input 
        while (mainMenuChoice != 4) { //repeating the menu while the user does not choose to exit 
            while (mainMenuChoice < 1 || mainMenuChoice > 4) { //validating input 
                cout << endl;
                cout << setw(62) << "Invalid choice! Choose a number between 1-4 (inclusive)." << endl << endl;
                menus.displayMainMenu(); //display main menu
                cin >> mainMenuChoice;
            }

            cout << endl;

            switch (mainMenuChoice) { //for choices 1-3 (the user is not exiting, so call functions to take user to different pages) 
            case 1:
                cashier();
                break;

            case 2:
                invMenu();
                break;

            case 3:
                reports();
                break;

            case 4:
                return; //return back to the main function to exit the program
                //no need for default, input is already validated before
            }

            //asking for menu again without quitting loop because user has not chosen to exit yet 
            cout << endl;
            menus.displayMainMenu(); //display main menu
            cin >> mainMenuChoice;
        }
    }

    void validateInvMenuChoice() {
        while (invMenuChoice != 5) { //while user has chosen not to exit 
            while (invMenuChoice < 1 || invMenuChoice > 5) { //validate user input and display menu again 
                cout << endl;
                cout << setw(62) << "Invalid choice! Choose a number between 1-5 (inclusive)." << endl << endl;
                menus.displayInvMenu(); //display inv menu 
                cin >> invMenuChoice;
            }

            cout << endl;

            switch (invMenuChoice) { //switch while user has not chosen exit and call stub functions 
            case 1:
                lookUpBook();
                break;

            case 2:
                addBook();
                break;

            case 3:
                editBook();
                break;

            case 4:
                deleteBook();
                break;

            case 5:
                return; //return back to the inventory menu function 
                //no need for default, input is already validated before
            }

            //displaying menu again without quitting loop (user has not chosen to quit yet)
            cout << endl;
            menus.displayInvMenu(); //displaying inv menu 
            cin >> invMenuChoice;
        }
    }

    void validateReportsChoice() {
        while (reportsChoice != 7) { //while the user has not chosen to exit 
            while (reportsChoice < 1 || reportsChoice > 7) { //validating user input and displaying menu again 
                cout << endl;
                cout << setw(62) << "Invalid choice! Choose a number between 1-7 (inclusive)." << endl << endl;
                menus.displayReports(); //display reports menu
                cin >> reportsChoice;
            }

            cout << endl;

            switch (reportsChoice) { //switch for choices (1-6), calling stub functions
            case 1:
                repListing();
                break;

            case 2:
                repWholesale();
                break;

            case 3:
                repRetail();
                break;

            case 4:
                repQty();
                break;

            case 5:
                repCost();
                break;

            case 6:
                repAge();
                break;

            case 7:
                return; //return to reports menu 
                //no need for default, input is already validated before
            }

            //displaying menu again without quitting loop
            cout << endl;
            menus.displayReports(); //display reports menu 
            cin >> reportsChoice;

        }
    }
};

InputValidation validate; //declaring class object 

//stub functions for the inventory database page
void addBook() { //project 12
    char addTitle[51], addISBN[14], addAuthor[31], addPublisher[31], addDate[11]; //now that member variables are private and not accessible, these are arrays used only to hold the user input initially        
    int j = 0, addQty = 0; //add qty, add wholesale and add retail only for holding the user input one at a time 
    double addWholesale = 0, addRetail = 0;

    cout << setw(28) << "You selected Add Book." << endl;
    cout << "\n";

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool emptyElementFound = false; //variable to check if any empty elements have been found

    for (int i = 0; i < 20; i++) { //searching file
        if (book[i].isEmpty(i)) {  //if a book has been found 
            cin.ignore(); //following statements asking for the book information
            cout << "Enter the Book title: ";
            cin.getline(addTitle, 51); //getting the input 
            strUpper(addTitle); //making uppercase
            book[i].setTitle(addTitle);

            cout << "Enter the ISBN number: ";
            cin.getline(addISBN, 14);
            strUpper(addISBN);
            book[i].setISBN(addISBN);

            cout << "Enter the Author's name: ";
            cin.getline(addAuthor, 31);
            strUpper(addAuthor);
            book[i].setAuthor(addAuthor);

            cout << "Enter the Publisher's name: ";
            cin.getline(addPublisher, 31);
            strUpper(addPublisher);
            book[i].setPub(addPublisher);

            cout << "Enter The date the book was added to the inventory (MM-DD-YYYY): ";
            cin.getline(addDate, 11);
            book[i].setDateAdded(addDate);

            cout << "Enter The quantity of the book being added: ";
            cin >> addQty;
            book[i].setQty(addQty);
            cin.ignore();

            cout << "Enter The wholesale cost of the book (for one copy): ";
            cin >> addWholesale;
            book[i].setWholesale(addWholesale);
            cin.ignore();

            cout << "Enter The retail price of the book (for one copy): ";
            cin >> addRetail;
            book[i].setRetail(addRetail);
            cin.ignore();

            emptyElementFound = true;

            inventory.seekp((sizeof(book[i]) * i), ios::beg); //write the specific input to the appropriate place in the file
            inventory.write(reinterpret_cast<char*>(&book[i]), sizeof(book[i]));

            break; //break out of the loop once the user has entered a book, they can enter again buy requesting to add another book
        }
    }

    if (emptyElementFound == false) { //array is full to the brim 
        cout << "No more books may be added to the inventory." << endl;
    }

    inventory.close(); //close file 
}

void lookUpBook() { //project 12
    int j = 0;

    cout << setw(32) << "You selected Look Up Book." << endl;
    cout << "\n";
    cin.ignore();

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states
    inventory.close(); //close

    char partialTitle[51]; //array for the user input/uppercase conversion
    string title, rightBook;  //the book the user is looking for/they will enter it and if the correct book they want is displayed
    string strPartialTitle; //the string version of the partial title array (necessary to pass to the string parameter of the bookMatch function)
    bool bookFound = false; //if the book is found in the inventory

    cout << "Enter the first word/part of the book title you are looking up: ";
    cin.getline(partialTitle, 51); //getting the partial title and converting to uppercase as the titles have been converted to uppercase already 
    strUpper(partialTitle);
    strPartialTitle.assign(partialTitle); //assigning the user input to a string variable because the bookMatch function requires a string argument

    for (int i = 0; i < 20; i++) { //searching the file
        if (book[i].bookMatch(strPartialTitle, i)) { //if a book has been found/passing the string argument 
            cout << "\nIs this the book you want? " << book[i].getTitle() << " (Y/N) ";
            cin >> rightBook;
            bookFound = true; //a book has been found 
            cout << endl;

            if (rightBook == "Y" || rightBook == "y") { //if that's the user's desired book, if not then keep searching 
                bookInfo(book[i].getISBN(), book[i].getTitle(), book[i].getAuthor(), book[i].getPub(), book[i].getDateAdded(), book[i].getQty(), book[i].getWholesale(), book[i].getRetail()); //call book info 
                break; //break out of the loop once the book has been found 
            }
        }
    }

    if (bookFound == false) { //the book has not been found/is not in the inventory
        cout << partialTitle << ", and any other books you entered were not in the inventory." << endl;
    }
}

void editBook() { //project 12
    char addTitle[51], addISBN[14], addAuthor[31], addPublisher[31], addDate[11]; //arrays to hold one user input at a time 
    int j = 0, addQty = 0; //the add variables to hold user input one at a time 
    double addWholesale = 0, addRetail = 0;

    cout << setw(29) << "You selected Edit Book." << endl;
    cout << "\n";
    cin.ignore();

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    string title, rightBook;  //the book the user is looking for/they will enter it and if the correct book they want is displayed
    bool bookFound = false; //if the book is found in the inventory
    char partialTitle[51]; //array for the user input/uppercase conversion
    string strPartialTitle; //the string version of the partial title array (necessary to pass to the string parameter of the bookMatch function)
    string userISBN, userBookTitle, userAuthor, userPublisher, userDate, userQty, userWholesale, userRetail; //variables for whether or not the user wants to edit those details for the book 

    cout << "Enter the first word/part of the book title you wish to edit: ";
    cin.getline(partialTitle, 51); //getting the user input and converting to uppercase for comparison 
    strUpper(partialTitle);
    strPartialTitle.assign(partialTitle); //assigning the user input to a string variable because the bookMatch function requires a string argument

    for (int i = 0; i < 20; i++) { //searching the file
        if (book[i].bookMatch(strPartialTitle, i)) { //if a book has been found/passing the string argument 
            cout << "\nIs this the book you want? " << book[i].getTitle() << " (Y/N) ";
            cin >> rightBook;
            bookFound = true; //a book has been found 
            cout << endl;

            if (rightBook == "Y" || rightBook == "y") { //if the user's desired book has been found, if not keep searching 
                bookInfo(book[i].getISBN(), book[i].getTitle(), book[i].getAuthor(), book[i].getPub(), book[i].getDateAdded(), book[i].getQty(), book[i].getWholesale(), book[i].getRetail()); //calling book info 
                cout << "\n";
                //cin.ignore();
                //below asking the user what they want to change about the book's details
                cout << "Do you wish to change the ISBN number ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userISBN;
                cout << "Do you wish to change the Book title ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userBookTitle;
                cout << "Do you wish to change the author ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userAuthor;
                cout << "Do you wish to change the publisher ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userPublisher;
                cout << "Do you wish to change the date the book was added to the inventory ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userDate;
                cout << "Do you wish to change the quantity of the book ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userQty;
                cout << "Do you wish to change the wholesale cost of the book ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userWholesale;
                cout << "Do you wish to change the retail price of the book ('Yes'/'No')? "; //these statements are for seeing what the user wants to change
                cin >> userRetail;
                cin.ignore();
                cout << "\n";

                if (userISBN == "Yes" || userISBN == "yes") { //nested if statements are for the user entering/editing the new values for the arrays 
                    cout << "What do you want the new ISBN number to be? ";
                    cin.getline(addISBN, 14); //getting the new isbn and converting to uppercase  
                    strUpper(addISBN);
                    book[i].setISBN(addISBN); //calling the set functions 
                }

                if (userBookTitle == "Yes" || userBookTitle == "yes") {
                    cout << "What do you want the new Book title to be? ";
                    cin.getline(addTitle, 51);
                    strUpper(addTitle);
                    book[i].setTitle(addTitle);
                }

                if (userAuthor == "Yes" || userAuthor == "yes") {
                    cout << "What do you want the new author to be? ";
                    cin.getline(addAuthor, 31);
                    strUpper(addAuthor);
                    book[i].setAuthor(addAuthor);
                }

                if (userPublisher == "Yes" || userPublisher == "yes") {
                    cout << "What do you want the new publisher to be? ";
                    cin.getline(addPublisher, 31);
                    strUpper(addPublisher);
                    book[i].setPub(addPublisher);
                }

                if (userDate == "Yes" || userDate == "yes") {
                    cout << "What do you want the new date added of the book to be? ";
                    cin.getline(addDate, 11);
                    book[i].setDateAdded(addDate);

                }

                if (userQty == "Yes" || userQty == "yes") {
                    cout << "What do you want the new quantity of the book to be? ";
                    cin >> addQty;
                    book[i].setQty(addQty);
                    cin.ignore();
                }

                if (userWholesale == "Yes" || userWholesale == "yes") {
                    cout << "What do you want the new wholesale cost to be? ";
                    cin >> addWholesale;
                    book[i].setWholesale(addWholesale);
                    cin.ignore();
                }

                if (userRetail == "Yes" || userRetail == "yes") {
                    cout << "What do you want the new retail price to be? ";
                    cin >> addRetail;
                    book[i].setRetail(addRetail);
                    cin.ignore();
                }

                inventory.seekp((sizeof(book[i]) * i), ios::beg); //write to the appropriate position
                inventory.write(reinterpret_cast<char*>(&book[i]), sizeof(book[i]));

                break; //break out of the loop once the edits are finished
            }
        }
    }

    if (bookFound == false) { //the book has not been found/is not in the inventory
        cout << partialTitle << ", and any other books you entered were not in the inventory." << endl;
    }

    inventory.close(); //close
}

void deleteBook() { //project 12
    int j = 0;

    cout << setw(31) << "You selected Delete Book." << endl;
    cout << "\n";
    cin.ignore();

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    string title, userDeleteBook, rightBook; //the book the user is looking for to delete and whether or not they want to delete it and if the correct book they want to delete is displayed
    bool bookFound = false; //if the book is found in the inventory or not 
    char partialTitle[51]; //partial lookup title array 
    string strPartialTitle; //the string version of the partial title array (necessary to pass to the string parameter of the bookMatch function)

    cout << "Enter the first word/part of the book title you wish to delete: ";
    cin.getline(partialTitle, 51); //getting partial title and converting to uppercase 
    strUpper(partialTitle);
    strPartialTitle.assign(partialTitle); //assigning the user input to a string variable because the bookMatch function requires a string argument

    for (int i = 0; i < 20; i++) { //searching the file
        if (book[i].bookMatch(strPartialTitle, i)) { //if a book has been found/passing the string argument 
            cout << "\nIs this the book you want? " << book[i].getTitle() << " (Y/N) ";
            cin >> rightBook;
            bookFound = true;
            cout << endl;

            if (rightBook == "Y" || rightBook == "y") { //if the user's desired book has been found 
                bookInfo(book[i].getISBN(), book[i].getTitle(), book[i].getAuthor(), book[i].getPub(), book[i].getDateAdded(), book[i].getQty(), book[i].getWholesale(), book[i].getRetail()); //calling book info 

                cout << "Do you really want to delete this book? ('Yes'/'No')? ";
                cin >> userDeleteBook;

                if (userDeleteBook == "Yes" || userDeleteBook == "yes") { //if the user truly wishes to delete this book 
                    book[i].removeBook(i); //removing the book
                    break; //break out of the loop once the user deletes the book
                }

                else if (userDeleteBook != "Yes" && userDeleteBook != "yes") { //break out of the loop anyway if the user says thats the book they want but they change their mind about deletion
                    break;
                }
            }
        }
    }

    if (bookFound == false) { //the book has not been found/is not in the inventory
        cout << partialTitle << ", and any other books you entered were not in the inventory." << endl;
    }

    inventory.close(); //close 

}

void strUpper(char* lowerToUpper) { //function for making every element in an array uppercase project 12
    int i = 0; //index variable 
    while (lowerToUpper[i] != '\0') { //while there are still elements
        lowerToUpper[i] = toupper(lowerToUpper[i]); //making them uppercase
        i++;
    }
}

//stub functions for the reports page 
void repListing() { //project 12
    int j = 0;

    cout << setw(37) << "You selected Inventory Listing." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    int enteredOnce = false;

    for (int i = 0; i < 20; i++) { //searching the file 
        if (book[i].getTitle()[0] != '\0') { //if there is actually a book title there/no null terminator 

            cout << "\nBook #" << (i + 1) << " listing report as of 03/09/25" << endl << endl;
            cout << "Title: " << book[i].getTitle() << endl;
            cout << "ISBN Number: " << book[i].getISBN() << endl;
            cout << "Author: " << book[i].getAuthor() << endl;
            cout << "Publisher: " << book[i].getPub() << endl;
            cout << "Date Added: " << book[i].getDateAdded() << endl;
            cout << "Quantity on Hand: " << book[i].getQty() << endl;
            cout << "Wholesale Cost: " << book[i].getWholesale() << endl;
            cout << "Retail Price: " << book[i].getRetail() << endl;

            if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                cout << "Press enter to continue to the next screen:";
                cin.ignore();
                cin.get();
            }

            else {
                cout << "Press enter to continue to the next screen:";
                cin.get();
            }

            enteredOnce = true;
        }
    }
    inventory.close(); //close file
}

void repWholesale() { //project 12
    int j = 0;

    cout << setw(45) << "You selected Inventory Wholesale Value." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool enteredOnce = false;
    double totalWholesaleValue = 0; //sum of all wholesale costs * quantity of books

    for (int i = 0; i < 20; i++) { //searching the file
        if (book[i].getTitle()[0] != '\0') { //if there is actually a book title there/no null terminator 
            cout << "\nBook #" << (i + 1) << " wholesale report as of 03/09/25" << endl << endl;
            cout << "Title: " << book[i].getTitle() << endl;
            cout << "ISBN Number: " << book[i].getISBN() << endl;
            cout << "Quantity on Hand: " << book[i].getQty() << endl;
            cout << "Wholesale Cost: " << book[i].getWholesale() << endl;

            totalWholesaleValue += book[i].getWholesale() * book[i].getQty();  //getting the total quantities and wholesale costs  

            if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                cout << "Press enter to continue to the next screen:";
                cin.ignore();
                cin.get();
            }

            else {
                cout << "Press enter to continue to the next screen:";
                cin.get();
            }

            enteredOnce = true;

        }

    }

    cout << "Total Wholesale Value: $" << setprecision(2) << showpoint << fixed << totalWholesaleValue << endl; //outputting to two decimals 
    inventory.close(); //close file
}

void repRetail() { //project 12
    int j = 0;

    cout << setw(42) << "You selected Inventory Retail Value." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool enteredOnce = false;
    double totalRetailValue = 0; //sum of all retail prices * quantity of books

    for (int i = 0; i < 20; i++) { //searching file
        if (book[i].getTitle()[0] != '\0') { //if there is actually a book title there/no null terminator 
            cout << "\nBook #" << (i + 1) << " retail report as of 03/09/25" << endl << endl;
            cout << "Title: " << book[i].getTitle() << endl;
            cout << "ISBN Number: " << book[i].getISBN() << endl;
            cout << "Quantity on Hand: " << book[i].getQty() << endl;
            cout << "Retail Price: " << book[i].getRetail() << endl;

            totalRetailValue += book[i].getRetail() * book[i].getQty();

            if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                cout << "Press enter to continue to the next screen:";
                cin.ignore();
                cin.get();
            }

            else {
                cout << "Press enter to continue to the next screen:";
                cin.get();
            }

            enteredOnce = true;
        }

    }

    cout << "Total Retail Value: $" << setprecision(2) << showpoint << fixed << totalRetailValue << endl; //outputting to two decimals 
    inventory.close(); //close file
}

void repQty() { //project 12
    int j = 0;

    cout << setw(39) << "You selected Listing By Quantity." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool enteredOnce = false;
    bool doNotOutputTwice[20] = {}; //bool array to make sure duplicate quantites from two diff books don't get outputted twice 
    int quantitySorted[20] = {}; //sorted quantities from highest to lowest
    int index = 0;

    for (int i = 0; i < 20; i++) { //copying all quantites of each book to the quantity sorted list
        if (book[i].getTitle()[0] != '\0') { //if there is actually a book there 
            quantitySorted[i] = book[i].getQty();
        }
    }

    for (int j = 19; j > 0; j--) { //bubble sorting the quantity sorted list
        for (int i = 0; i < 19; i++) {
            if (quantitySorted[i] < quantitySorted[i + 1]) { //checking if the quantity should be moved down 
                int swap = quantitySorted[i]; //swap variable
                quantitySorted[i] = quantitySorted[i + 1];
                quantitySorted[i + 1] = swap;
            }
        }
    }

    for (int outerIndex = 0; outerIndex < 20; outerIndex++) {
        for (int innerIndex = 0; innerIndex < 20; innerIndex++) { //displaying information in order from highest to lowest (quantity)
            if (book[innerIndex].getTitle()[0] != '\0' && book[innerIndex].getQty() == quantitySorted[outerIndex] && doNotOutputTwice[innerIndex] != 1) { //if book title array is not null terminator and if qtyonhand = sorted one 
                cout << "\nBook #" << (innerIndex + 1) << " quantity report as of 03/09/25" << endl << endl;
                cout << "Title: " << book[innerIndex].getTitle() << endl;
                cout << "ISBN Number: " << book[innerIndex].getISBN() << endl;
                cout << "Quantity on Hand: " << book[innerIndex].getQty() << endl;

                doNotOutputTwice[innerIndex] = 1; //user has already entered this index 

                if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                    cout << "Press enter to continue to the next screen:";
                    cin.ignore();
                    cin.get();
                }

                else {
                    cout << "Press enter to continue to the next screen:";
                    cin.get();
                }

                enteredOnce = true;
            }
        }
    }
    inventory.close(); //close file
}

void repCost() { //project 12
    int j = 0;

    cout << setw(35) << "You selected Listing By Cost." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool enteredOnce = false; //if the user has already pressed enter once to go to next screen

    double wholesaleSorted[20] = {}; //sorted wholesale costs 
    bool doNotOutputTwice[20] = {}; //for outputting the same costs they don't output twice 

    for (int i = 0; i < 20; i++) { //copying all wholesale costs of each book to the wholesale sorted list
        if (book[i].getTitle()[0] != '\0') { //if there is actually a book there 
            wholesaleSorted[i] = book[i].getWholesale();
        }
    }

    for (int j = 19; j > 0; j--) { //bubble sorting the wholesale costs sorted list
        for (int i = 0; i < 19; i++) {
            if (wholesaleSorted[i] < wholesaleSorted[i + 1]) { //if the wholesale sorted is less than the one on the right 
                double swap = wholesaleSorted[i]; //swap variable 
                wholesaleSorted[i] = wholesaleSorted[i + 1];
                wholesaleSorted[i + 1] = swap;
            }
        }
    }

    for (int outerIndex = 0; outerIndex < 20; outerIndex++) {
        for (int innerIndex = 0; innerIndex < 20; innerIndex++) { //displaying information in order from highest to lowest (wholesale)
            if (book[innerIndex].getTitle()[0] != '\0' && book[innerIndex].getWholesale() == wholesaleSorted[outerIndex] && doNotOutputTwice[innerIndex] != 1) { //if book title array is not null terminator and if wholesale = sorted one 
                cout << "\nBook #" << (innerIndex + 1) << " wholesale report as of 03/09/25" << endl << endl;
                cout << "Title: " << book[innerIndex].getTitle() << endl;
                cout << "ISBN Number: " << book[innerIndex].getISBN() << endl;
                cout << "Quantity on Hand: " << book[innerIndex].getQty() << endl;
                cout << "Wholesale Cost: " << book[innerIndex].getWholesale() << endl;

                doNotOutputTwice[innerIndex] = 1; //has been displayed already so no dupliate values are outputted more than once 

                if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                    cout << "Press enter to continue to the next screen:";
                    cin.ignore();
                    cin.get();
                }

                else {
                    cout << "Press enter to continue to the next screen:";
                    cin.get();
                }

                enteredOnce = true;

            }
        }
    }
    inventory.close(); //close file
}

void repAge() { //project 12
    int j = 0;

    cout << setw(34) << "You selected Listing By Age." << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    bool doNotOutputTwice[20] = {};
    bool enteredOnce = false;
    string dateAddedCopy = ""; //string variable for holding the individual elements in the date added array (such as one date)
    string dateAddedSortedOriginalFormat[20]; //string array sorted with oldest to newest dates of mm-dd-yyyy format to output in proper order 
    string dateAddedSortedNewFormat[20]; //string array sorted with oldest to newest dates of yyyy-mm-dd format for proper comparison
    string swap; //swap variable 

    for (int i = 0; i < 20; i++) {
        if (book[i].getTitle()[0] != '\0') { //if there is a book there 
            dateAddedCopy = book[i].getDateAdded(); //copying the date added string values individually (one date at a time)

            dateAddedSortedNewFormat[i] += dateAddedCopy[6]; //rearranging the format of the dates from mm-dd-yyyy to yyyy-mm-dd because we can compare them directly that way
            dateAddedSortedNewFormat[i] += dateAddedCopy[7]; //adding each character in the proper order to the sorted array in yyyy-mm-dd format 
            dateAddedSortedNewFormat[i] += dateAddedCopy[8];
            dateAddedSortedNewFormat[i] += dateAddedCopy[9];
            dateAddedSortedNewFormat[i] += dateAddedCopy[5];
            dateAddedSortedNewFormat[i] += dateAddedCopy[0];
            dateAddedSortedNewFormat[i] += dateAddedCopy[1];
            dateAddedSortedNewFormat[i] += dateAddedCopy[2];
            dateAddedSortedNewFormat[i] += dateAddedCopy[3];
            dateAddedSortedNewFormat[i] += dateAddedCopy[4];
        }
    }

    for (int j = 19; j > 0; j--) { //bubble sorting the new array with format yyyy-mm-dd to put oldest dates in the front of the array
        for (int i = 0; i < 19; i++) {
            if (dateAddedSortedNewFormat[i] > dateAddedSortedNewFormat[i + 1]) { //making sure the comparison is accurate
                swap = dateAddedSortedNewFormat[i]; //swap variable
                dateAddedSortedNewFormat[i] = dateAddedSortedNewFormat[i + 1];
                dateAddedSortedNewFormat[i + 1] = swap;
            }
        }
    }

    for (int i = 0; i < 20; i++) { //converting back into mm-dd-yyyy format and storing the conversion in the second sorted array  
        if (dateAddedSortedNewFormat[i].length() > 0) { //if there's a date there
            dateAddedCopy = dateAddedSortedNewFormat[i]; //copying the sorted values which are in yyyy-mm-dd format to loop through chars individually

            dateAddedSortedOriginalFormat[i] += dateAddedCopy[5]; //rearranging into mm-dd-yyyy format for the output to the user (adding individual characters)
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[6];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[7];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[8];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[9];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[4];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[0];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[1];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[2];
            dateAddedSortedOriginalFormat[i] += dateAddedCopy[3];
        }
    }

    for (int j = 0; j < 20; j++) { //displaying dates from oldest to newest (mm-dd-yyyy)
        for (int i = 0; i < 20; i++) {
            if (book[i].getTitle()[0] != '\0' && book[i].getDateAdded() == dateAddedSortedOriginalFormat[j] && doNotOutputTwice[i] != 1) { //if a book exists there and if the date added equals the new sorted one 
                cout << "Book #" << (i + 1) << " age report as of 03/09/25" << endl << endl;
                cout << "Title: " << book[i].getTitle() << endl;
                cout << "ISBN Number: " << book[i].getISBN() << endl;
                cout << "Quantity on Hand: " << book[i].getQty() << endl;
                cout << "Date Added: " << book[i].getDateAdded() << endl; //displaying in order 

                doNotOutputTwice[i] = 1;

                if (!enteredOnce) { //these if statements are because my get statements are messing up and the user has to enter twice after first iteration to go to next screen 
                    cout << "Press enter to continue to the next screen:";
                    cin.ignore();
                    cin.get();
                }

                else {
                    cout << "Press enter to continue to the next screen:";
                    cin.get();
                }

                enteredOnce = true;
            }
        }
    }
    inventory.close(); //close file
}

//main menu project 12/main function 
int main() {
    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::out | ios::binary | ios::app); //make sure the file exists by not overwriting contents
        //note this combination causes the file to be made if it hasnt already, and not overwritten if it already exists (bc of append)
        //in main so it can be accessible to all functions

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.close(); //close file before other functions open 

    int choice;
    menus.displayMainMenu(); //display main menu
    cin >> choice;

    validate.setMainMenuChoice(choice); //set and validate user choice 
    validate.validateMainMenuChoice();

    cout << "\n" << setw(26) << "You selected item 4." << endl; //when while loop ends b/c of exit being chosen display this 
    return 0;
}

//inv menu project 12
void invMenu() {
    int choice;
    menus.displayInvMenu(); //display inv menu 
    cin >> choice;

    validate.setInvMenuChoice(choice); //set and validate user choice 
    validate.validateInvMenuChoice();

    cout << "\n" << setw(26) << "You selected item 5." << endl; //when while loop ends b/c of exit being chosen 
}

//reports project 12
void reports() {
    int choice;
    menus.displayReports(); //display reports menu 
    cin >> choice;

    validate.setReportsChoice(choice); //set and validate reports choice 
    validate.validateReportsChoice();

    cout << "\n" << setw(26) << "You selected item 7." << endl; //when while loop ends b/c of return to menu being chosen
}

//cashier.cpp project 12
void cashier() {
    int j = 0;

    cout << setw(30) << "Serendipity Booksellers" << endl; //display menu 
    cout << setw(25) << "Cashier Module" << endl << endl;

    try { //try/catch construct for file open error
        inventory.open("database.dat", ios::in | ios::out | ios::binary); //open for input/output

        if (!inventory) { //checking if file even opens
            throw fileOpenError; //throwing an error
        }
    }

    catch (string error) {
        cout << error; //display error
        exit(1); //exit program/terminate 
    }

    inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    while (!inventory.eof()) { //while there are still lines left read the file
        j++;
        inventory.read(reinterpret_cast<char*>(&book[j]), sizeof(book[j]));
    }

    inventory.clear(); //clear any error states

    char userISBN[14]; //isbn array to hold user isbn input one at a time 
    string date, title, processTransactionAgain; //all the payment variables and book information variables 
    int quantity, isbnLocatedIndex; //quantity and where ISBN was located in the array 
    double subtotal = 0, addedTax = 0;
    string reEnterIsbnNum; //if the user wishes to look up the ISBN number again after it failed to match ISBN
    bool isbnLocated = false; //to check whether or not the user entered isbn has been located in the array 
    int numTitles;

    cout << "How many titles would you like to purchase? ";
    cin >> numTitles;

    SoldBook* cashierTitle = nullptr; //setting to null first 

    try { //try/catch construct for handling the bad_alloc exception 
        cashierTitle = new SoldBook[numTitles]; //dynamically allocating memory
    }

    catch (bad_alloc) {
        cout << "ERROR! Not enough memory to handle your request! Terminating Program!";
        exit(1);
    }

    for (int i = 0; i < numTitles; i++) { //looping for each title 
        do { //Isbn lookup loop 
            reEnterIsbnNum = ""; //resetting
            cout << setw(12) << "ISBN for title " << (i + 1) << ": ";
            cin.ignore();
            cin.getline(userISBN, 14);
            strUpper(userISBN);

            for (int i = 0; i < 20; i++) { //searching file
                if (strcmp(book[i].getISBN(), userISBN) == 0 && book[i].getTitle()[0] != '\0') {
                    cout << endl;
                    cout << "This is the book associated with that ISBN: " << book[i].getTitle() << endl;
                    cout << "Retail Price: " << book[i].getRetail() << endl;
                    isbnLocatedIndex = i; //index where the isbn was located
                    isbnLocated = true;
                    reEnterIsbnNum = "No"; //break out of the do while 
                    break; //once Isbn has been located
                }

                else {
                    isbnLocatedIndex = -1; //if no isbn has been found 
                }
            }

            if (isbnLocatedIndex == -1) {
                cout << "ISBN number not found!" << endl;
                cout << "If you do not re-enter the proper ISBN, you will move on to entering the next title." << endl;
                cout << "Do you wish to re-enter the ISBN number? ('Yes'/'No): ";
                cin >> reEnterIsbnNum;
            }

        } while (reEnterIsbnNum == "yes" || reEnterIsbnNum == "Yes"); //repeat while user wishes to re-enter isbn

        if (isbnLocatedIndex >= 0 && strcmp(book[isbnLocatedIndex].getISBN(), userISBN) == 0 && book[isbnLocatedIndex].getTitle()[0] != '\0') { //if the ISBN has been found 
            cout << setw(12) << "Date: "; //get date added and quantity of the book 
            cin >> date;
            cout << setw(24) << "Quantity of Book: ";
            cin >> quantity;

            cashierTitle[i].setQtySold(quantity);

            if (book[isbnLocatedIndex].getQty() >= cashierTitle[i].getQtySold()) { //if there are enough copies to fulfill the order

                book[isbnLocatedIndex].setQty(book[isbnLocatedIndex].getQty() - cashierTitle[i].getQtySold()); //subtract the quantity from quantity on hand

                inventory.seekp((sizeof(book[isbnLocatedIndex]) * isbnLocatedIndex), ios::beg); //write quantity to the appropriate position
                inventory.write(reinterpret_cast<char*>(&book[isbnLocatedIndex]), sizeof(book[isbnLocatedIndex]));

                cashierTitle[i].setTax(isbnLocatedIndex);
                cashierTitle[i].setSubtotal(book[isbnLocatedIndex].getRetail() * cashierTitle[i].getQtySold()); //increase subtotal
                cashierTitle[i].setTotal();

                //displaying cashier slip/data of the purchase
                cout << "\nCashier slip for this specific book: " << endl;
                cout << "\nDate: " << date << endl << endl; //data formatting and outputting specifics 
                cout << "Qty ISBN" << setw(16) << "Title" << setw(30) << "Price (for one book)" << setw(15) << endl; //here they mean retail price 
                cout << "----------------------------------------------------------------------------------------" << endl;
                cout << cashierTitle[i].getQtySold() << " " << book[isbnLocatedIndex].getISBN() << " " << book[isbnLocatedIndex].getTitle() << setprecision(2) << fixed << showpoint << setw(12) << "$" << setw(6) << book[isbnLocatedIndex].getRetail() << endl;
                cout << endl << endl << endl;

                cout << "Subtotal (for this one title + tax): $" << cashierTitle[i].getSubtotal() << endl << endl;

            }

            else {
                cout << "Not enough copies in stock to fulfill your order. Going to the next Book order if there are more." << endl;
            }
        }
    }

    inventory.close(); //close the file once done processing transactions

    if (isbnLocated == true) { //only if at least one valid isbn was found, if not just return to main menu
        //display all the calculations once user done entering book(s)

        //calculate the total subtotal and addedTax with for loop
        for (int i = 0; i < numTitles; i++) {
            subtotal += cashierTitle[i].getSubtotal();
            addedTax += cashierTitle[i].getTax();
        }

        cout << "\nCashier slip for all titles: " << setprecision(2) << fixed << showpoint << endl;
        cout << setw(21) << "Subtotal (for the title(s) you just purchased - minus tax)" << setw(20) << "$" << setw(6) << (subtotal - addedTax) << endl; //displaying final totals for all orders
        cout << setw(16) << "Tax (for the title(s) you just purchased)" << setw(37) << "$" << setw(6) << addedTax << endl;
        cout << setw(18) << "Total (for all orders made during the execution of this program)" << setw(14) << "$" << setw(6) << cashierTitle[0].getTotal() << endl << endl; //can use any cashierTitle index b/c total is static

        delete[] cashierTitle; //delete memory 
        cashierTitle = nullptr;
        return;
    }

    else {
        cout << "\nNo proper ISBN number was given/no books were found in the inventory to match user provided ISBN number(s) or no books were purchased whatsoever." << endl;
        cout << "Returning to main menu." << endl;

        delete[] cashierTitle; //delete memory 
        cashierTitle = nullptr;
        return;
    }
}

//book info project 12
void bookInfo(const char* isbn, const char* title, const char* author, const char* publisher, const char* date, int qty, double wholesale, double retail) { //displaying the book information for each book and taking in new parameters 
    cout << "\n";
    cout << setw(35) << "Serendipity Booksellers" << endl;
    cout << setw(31) << "Book Information" << endl << endl;
    cout << "ISBN: " << isbn << endl;
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
    cout << "Publisher: " << publisher << endl;
    cout << "Date Added: " << date << endl;
    cout << "Quantity-On-Hand: " << qty << endl;
    cout << "Wholesale Cost: " << setprecision(2) << fixed << showpoint << "$ " << wholesale << endl; //output formatting
    cout << "Retail Price: " << setprecision(2) << fixed << showpoint << "$ " << retail << endl;
}

/* output example (exceptions)
case 1: inventory file fails to open, exception handler displays error and quits program
case 2: failed to dynamically allocate memory, exception handler displays error and quits Program
*/

/* output example (SoldBook functions)
case 1: setter and getter functions set and return private member variable values
*/

/*
output example (bookMatch member function)
case 1: returns true if the partial string/string is found in the title, else returns false
*/

/* output example (get functions)
case 1: the member variables' values are returned to the proper functions/processors
*/

/* output example (file)
case 1: now data is saved between runs and can access without manually adding new books every run
*/

/* output example (set functions)
case 1: the strings and numbers are copied to the class member variables
*/

/* output example (is empty)
case 1: no book is present and user can add book
*/

/* output example (remove book)
case 1: user wishes to remove book and remove book removes
*/

/* output example (str upper)
case 1: user enters book data and str upper makes it uppercase for the non-numeric and non date data
*/

/* output example (rep listing)
case 1: user asks for a book report
output: all data for the book is displayed (in uppercase)
*/

/* output example (rep wholesale)
case 1: user asks for wholesale report
output: data is displayed and user gets the wholesale value
*/

/* output example (rep retail)
case 1: user asks for retail report
output: data is displayed and user gets the retail value
*/

/* output example (rep qty)
case 1: user asks for qty report
output: data for the books are displayed in order from highest quantity to lowest (descending)
*/

/* output example (rep cost)
case 1: user asks for wholesale cost report
output: data for the books are displayed in order from highest cost to lowest (descending)
*/

/* output example (rep age)
case 1: user asks for date report
output: data for the books are displayed in order from oldest to newest (descending)
*/

/* output example (main menu)
Case 1: user enters 0
error message appears
display menu
read user choice
user enters 4
display message referring to selecting 4/exit
*/

/* output (inv menu)
Case 1: user enters 1
"You selected Look Up Book." is the output, displays menu again
user enters 5:
"You selected item 5." is the output, program branches back to main menu
*/

/* output example (reports)
Case 1: user enters 3
"You selected Inventory Retail Value." is the output, menu displayed again
user enters 7:
"You selected item 7." is the output, program branches back to main menu
*/

/* output example (cashier)
Case 1:
user enters isbn and the isbn is found, user enters another book and then the transaction for both books is calculated
*/

/*output example (book info)
Case 1:
book info gets these arguments passed: 0-333-90123-8; History of Scotland; Johnson, Jack; Johnson Inc.; 05-03-2019; 10; 20.00; 24.99
book info displays all the book information w/ output formatting
*/

/* output example (add book)
* case 1: null terminator is found and user enters book "History of Scotland" into the database w/ details above in bookInfo case output
* output: goes back to inventory database, then user looks up book
*/

/* output example (look up boock)
* case 1: user asks to look up book named "History of Scotland"c
* output: History (partial lookup) and its details are outputted
*/

/* output example (edit book)
* case 1: user asks to edit History (partial lookup) to change the title to History of Scot and the date added to 07-04-2020
* output: when the user goes to look up book, the new details are displayed
*/

/* output example (delete book)
* case 1: user asks to delete History of Finland
* output: book is not found message is displayed
*/