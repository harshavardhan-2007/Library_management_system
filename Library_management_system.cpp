#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Book {
private:
    int bookID;
    char title[50];
    char author[50];
    bool issued;

public:
    void addBook() {
        cout << "Enter Book ID: ";
        cin >> bookID;
        cin.ignore();
        cout << "Enter Title: ";
        cin.getline(title, 50);
        cout << "Enter Author: ";
        cin.getline(author, 50);
        issued = false;
    }

    void display() const {
        cout << "\nBook ID: " << bookID;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author;
        cout << "\nStatus: " << (issued ? "Issued" : "Available") << endl;
    }

    int getID() const {
        return bookID;
    }

    char* getTitle() {
        return title;
    }

    char* getAuthor() {
        return author;
    }

    bool isIssued() const {
        return issued;
    }

    void issueBook() {
        issued = true;
    }

    void returnBook() {
        issued = false;
    }
};

void addBook() {
    Book b;
    ofstream out("library.dat", ios::binary | ios::app);
    b.addBook();
    out.write(reinterpret_cast<char*>(&b), sizeof(b));
    out.close();
    cout << "Book added successfully!\n";
}

void displayBooks() {
    Book b;
    ifstream in("library.dat", ios::binary);

    while (in.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        b.display();
    }
    in.close();
}

void modifyBook(int id, int type) {
    Book b;
    fstream file("library.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&b), sizeof(b));

        if (b.getID() == id) {
            if (type == 1) {
                if (!b.isIssued()) {
                    b.issueBook();
                    cout << "Book issued successfully!\n";
                } else {
                    cout << "Book already issued!\n";
                }
            } else {
                if (b.isIssued()) {
                    b.returnBook();
                    cout << "Book returned successfully!\n";
                } else {
                    cout << "Book was not issued!\n";
                }
            }

            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&b), sizeof(b));
            found = true;
        }
    }

    file.close();
    if (!found)
        cout << "Book not found!\n";
}

void searchBook() {
    Book b;
    ifstream in("library.dat", ios::binary);
    char keyword[50];
    bool found = false;

    cin.ignore();
    cout << "Enter title or author to search: ";
    cin.getline(keyword, 50);

    while (in.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (strstr(b.getTitle(), keyword) || strstr(b.getAuthor(), keyword)) {
            b.display();
            found = true;
        }
    }

    in.close();

    if (!found)
        cout << "No matching books found!\n";
}

int main() {
    int choice, id;

    do {
        cout << "\n--- LIBRARY MANAGEMENT SYSTEM ---\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            cout << "Enter Book ID: ";
            cin >> id;
            modifyBook(id, 1);
            break;
        case 4:
            cout << "Enter Book ID: ";
            cin >> id;
            modifyBook(id, 2);
            break;
        case 5:
            searchBook();
            break;
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}
