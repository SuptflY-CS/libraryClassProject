#pragma once
//prototypes for each function 
void cashier();
void invMenu();
void bookInfo(const char*, const char*, const char*, const char*, const char*, int, double, double);
void reports();
//prototypes for inventory stub functions
void lookUpBook();
void addBook();
void editBook();
void deleteBook();
//prototypes for reports stub functions 
void repListing();
void repWholesale();
void repRetail();
void repQty();
void repCost();
void repAge();
//uppercase conversion function
void strUpper(char*);