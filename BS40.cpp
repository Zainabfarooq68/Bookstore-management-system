// Updated Full Code with Cancel Order Feature
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// --- Color Codes ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

struct Book {
    int id;
    string title;
    string author;
    string category;
    double price;
    double discount;
};

vector<Book> books;
vector<Book> cart;
vector<Book> wishlist;

string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

void sortBooksById() {
    for (int i = 0; i < books.size() - 1; i++) {
        for (int j = i + 1; j < books.size(); j++) {
            if (books[i].id > books[j].id) {
                swap(books[i], books[j]);
            }
        }
    }
}

void saveBooksToFile() {
    sortBooksById();
    ofstream file("books.txt");
    for (Book b : books) {
        file << b.id << "," << b.title << "," << b.author << "," << b.category << "," << b.price << "," << b.discount << "\n";
    }
    file.close();
}

void loadBooksFromFile() {
    ifstream file("books.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Book b;
        string idStr, priceStr, discountStr;
        getline(ss, idStr, ',');
        getline(ss, b.title, ',');
        getline(ss, b.author, ',');
        getline(ss, b.category, ',');
        getline(ss, priceStr, ',');
        getline(ss, discountStr, ',');
        b.id = stoi(idStr);
        b.price = stod(priceStr);
        b.discount = stod(discountStr);
        books.push_back(b);
    }
    file.close();
    sortBooksById();
}

void displayAllBooks() {
    if (books.empty()) {
        cout << RED << "No books available.\n" << RESET;
        return;
    }
    for (Book b : books) {
        cout << CYAN << "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author
             << ", Category: " << b.category << ", Price: " << b.price
             << ", Discount: " << b.discount << "%" << RESET << "\n";
    }
}

void saveOrderToFile(const vector<Book>& order) {
    ofstream file("orders.txt", ios::app);
    for (Book b : order) {
        file << b.id << "," << b.title << "," << b.price << "," << b.discount << "\n";
    }
    file << "====\n"; // delimiter for each order
    file.close();
}

void cancelOrder() {
    ifstream file("orders.txt");
    vector<string> allLines;
    string line;
    while (getline(file, line)) {
        allLines.push_back(line);
    }
    file.close();

    if (allLines.empty()) {
        cout << RED << "No orders to cancel.\n" << RESET;
        return;
    }

    int orderIndex = 0, count = 0;
    cout << CYAN << "\nExisting Orders:\n" << RESET;
    for (int i = 0; i < allLines.size(); i++) {
        if (allLines[i] == "====") {
            cout << CYAN << "Order #" << ++orderIndex << "\n" << RESET;
        } else {
            cout << allLines[i] << endl;
        }
    }

    cout << YELLOW << "Enter order number to cancel: " << RESET;
    cin >> count;

    orderIndex = 0;
    vector<string> updatedLines;
    for (int i = 0; i < allLines.size(); i++) {
        if (allLines[i] == "====") orderIndex++;
        if (orderIndex != count) updatedLines.push_back(allLines[i]);
    }

    ofstream outFile("orders.txt");
    for (string l : updatedLines) outFile << l << "\n";
    outFile.close();
    cout << GREEN << "Order #" << count << " canceled successfully.\n" << RESET;
}

// Existing Functionalities...
void addBook();
void changePrice(int bookId);
void updateBook(int bookId);
void deleteBook(int bookId);
void applyDiscount(int bookId, double discount);
void searchByCategory(string category);
void viewBookDetails(int bookId);
void addToCart(int bookId);
void addToWishlist(int bookId);
void makePayment();
void showCart();
void showWishlist();

void addBook() {
    Book b;
    cout << YELLOW << "Enter Book ID: " << RESET;
    cin >> b.id;
    cin.ignore();
    cout << YELLOW << "Enter Title: " << RESET;
    getline(cin, b.title);
    cout << YELLOW << "Enter Author: " << RESET;
    getline(cin, b.author);
    cout << YELLOW << "Enter Category: " << RESET;
    getline(cin, b.category);
    cout << YELLOW << "Enter Price: " << RESET;
    cin >> b.price;
    b.discount = 0.0;
    books.push_back(b);
    saveBooksToFile();
    cout << GREEN << "Book added successfully!\n" << RESET;
}

void changePrice(int bookId) {
    for (Book& b : books) {
        if (b.id == bookId) {
            cout << YELLOW << "New Price: " << RESET;
            cin >> b.price;
            saveBooksToFile();
            cout << GREEN << "Price updated.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void updateBook(int bookId) {
    for (Book& b : books) {
        if (b.id == bookId) {
            cin.ignore();
            cout << YELLOW << "Update Title: " << RESET;
            getline(cin, b.title);
            cout << YELLOW << "Update Author: " << RESET;
            getline(cin, b.author);
            cout << YELLOW << "Update Category: " << RESET;
            getline(cin, b.category);
            cout << YELLOW << "Update Price: " << RESET;
            cin >> b.price;
            saveBooksToFile();
            cout << GREEN << "Book updated.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void deleteBook(int bookId) {
    for (int i = 0; i < books.size(); i++) {
        if (books[i].id == bookId) {
            books.erase(books.begin() + i);
            saveBooksToFile();
            cout << GREEN << "Book deleted.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void applyDiscount(int bookId, double discountPercent) {
    for (Book& b : books) {
        if (b.id == bookId) {
            b.discount = discountPercent;
            saveBooksToFile();
            cout << GREEN << "Discount applied.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void searchByCategory(string category) {
    bool found = false;
    for (Book b : books) {
        if (toLower(b.category) == toLower(category)) {
            cout << CYAN << "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Price: " << b.price << ", Discount: " << b.discount << "%\n" << RESET;
            found = true;
        }
    }
    if (!found) {
        cout << RED << "No books found in this category.\n" << RESET;
    }
}

void viewBookDetails(int bookId) {
    for (Book b : books) {
        if (b.id == bookId) {
            cout << MAGENTA << "ID: " << b.id << "\nTitle: " << b.title << "\nAuthor: " << b.author
                 << "\nCategory: " << b.category << "\nPrice: " << b.price << "\nDiscount: " << b.discount << "%\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void addToCart(int bookId) {
    for (Book b : books) {
        if (b.id == bookId) {
            cart.push_back(b);
            cout << GREEN << "Book added to cart.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void addToWishlist(int bookId) {
    for (Book b : books) {
        if (b.id == bookId) {
            wishlist.push_back(b);
            cout << GREEN << "Book added to wishlist.\n" << RESET;
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void makePayment() {
    double total = 0;
    for (Book b : cart) {
        total += b.price * (1 - b.discount / 100);
    }
    if (cart.empty()) {
        cout << RED << "Your cart is empty.\n" << RESET;
        return;
    }

    cout << YELLOW << "Total amount: " << total << "\nEnter payment method (Card/Bank/Wallet): " << RESET;
    string method;
    cin >> method;
    method = toLower(method);
    if (method != "card" && method != "bank" && method != "wallet") {
        cout << RED << "Invalid payment method. Transaction cancelled.\n" << RESET;
        return;
    }

    cout << GREEN << "Payment successful using " << method << ".\n" << RESET;
    saveOrderToFile(cart);
    cart.clear();
}

void showCart() {
    if (cart.empty()) {
        cout << RED << "Your cart is empty.\n" << RESET;
        return;
    }
    for (Book b : cart) {
        cout << BLUE << b.title << " - Price: " << b.price << " - Discount: " << b.discount << "%\n" << RESET;
    }
}

void showWishlist() {
    if (wishlist.empty()) {
        cout << RED << "Your wishlist is empty.\n" << RESET;
        return;
    }
    for (Book b : wishlist) {
        cout << BLUE << b.title << " - Price: " << b.price << "\n" << RESET;
    }
}

void adminMenu() {
    int choice, id;
    double discount;
    do {
        cout << BOLD << "\nAdmin Menu:\n" << RESET
             << YELLOW << "1. Add Book\n2. Change Price\n3. Update Book\n4. Delete Book\n5. Apply Discount\n6. Display All Books\n7. Cancel Order\n0. Back\n" << RESET
             << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addBook(); break;
            case 2: cout << "Book ID: "; cin >> id; changePrice(id); break;
            case 3: cout << "Book ID: "; cin >> id; updateBook(id); break;
            case 4: cout << "Book ID: "; cin >> id; deleteBook(id); break;
            case 5: cout << "Book ID: "; cin >> id; cout << "Discount: "; cin >> discount; applyDiscount(id, discount); break;
            case 6: displayAllBooks(); break;
            case 7: cancelOrder(); break;
            case 0: break;
            default: cout << RED << "Invalid option. Please try again.\n" << RESET; break;
        }
    } while (choice != 0);
}

void userMenu() {
    int choice, id;
    string category;
    do {
        cout << BOLD << "\nUser Menu:\n" << RESET
             << CYAN << "1. Search by Category\n2. View Book Details\n3. Add to Cart\n4. Add to Wishlist\n5. Show Cart\n6. Show Wishlist\n7. Make Payment\n8. Display All Books\n0. Back\n" << RESET
             << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: cin.ignore(); cout << "Category: "; getline(cin, category); searchByCategory(category); break;
            case 2: cout << "Book ID: "; cin >> id; viewBookDetails(id); break;
            case 3: cout << "Book ID: "; cin >> id; addToCart(id); break;
            case 4: cout << "Book ID: "; cin >> id; addToWishlist(id); break;
            case 5: showCart(); break;
            case 6: showWishlist(); break;
            case 7: makePayment(); break;
            case 8: displayAllBooks(); break;
            case 0: break;
            default: cout << RED << "Invalid option. Please try again.\n" << RESET; break;
        }
    } while (choice != 0);
}

int main() {
    loadBooksFromFile();
    int mainChoice;
    do {
        cout << BOLD << "\n=== WELCOME TO ZAINAB BOOKSTORE MANAGEMENT SYSTEM ===\n" << RESET
             << GREEN << "1. Admin Panel\n" << BLUE << "2. User Panel\n" << RED << "0. Exit\n" << RESET
             << "Enter your choice: ";
        cin >> mainChoice;
        switch (mainChoice) {
            case 1: adminMenu(); break;
            case 2: userMenu(); break;
            case 0: cout << GREEN << "\nThank you for using the Book Store Management System. Goodbye!\n" << RESET; break;
            default: cout << RED << "Invalid option. Please try again.\n" << RESET; break;
        }
    } while (mainChoice != 0);
    return 0;
}

