// Muhammad Asad Tariq (21L-5266) --- BCS-3F
// Data Structures --- Assignment 4

#include <iostream>		// for input/output
#include <cstdlib>		// for rand() and srand()
#include <conio.h>		// for _getch()
#include <limits>		// for validating inputs
#include <string>		// for book title and author names
#include <vector>		// for hash table
#include <list>			// for collision resolution via chaining, and for list of author names

// Definitions of Constants for Input of Keys From Keyboard:

#define	C	99			// for creating a new book record
#define	R	114			// for reviewing an existing book record
#define	U	117			// for updating an existing book record
#define	D	100			// for deleting an existing bbok record

#define Q	113			// for quitting from the Library Management System

#define t	116			// for updating title
#define a	97			// for updating authors' list
#define rb	114			// for updating number of reference books
#define ib	105			// for updating number of issuable books

using namespace std;

// For easier alteration of hash table size while evaluating the code: (change to smaller nums like 3 and 5 to check collision resolution)

const int HASHTABLE_SIZE = 101;
const int P_ABOVE_T_SIZE = 103;		// the prime P used in the Universal Hashing Function

// Horizontal line has to be displayed multiple times on the interface, so:

const char line[] = "-------------------------------------------------------------------------------------------------";

// Class Template for Hash Table; in this program, HashTable<BookRecord> will be used:

template <typename T>
class HashTable
{
private:

	int tableSize;

	vector<list<T>> hashTable;			// of list<T> because lists of type T are needed to resolve collisions (via chaining)

	int P;

public:

	HashTable(int _tableSize, int primeP) : tableSize(_tableSize), P(primeP)
	{
		hashTable.resize(tableSize);	// the vector now acts as an array of size tableSize, i.e. hashTable[0] to hashTable[tableSize - 1]
	}

	void insert(int key, const T& value)	// O(1) unless collision + less collisions due to universal hash function and large table size
	{										// so O(1) time complexity on average (same for find() and remove() functions)
		srand(key);
		int A = rand() % P, B = rand() % P;
		hashTable[((A * key + B) % P) % tableSize].push_front(value);	// i.e. add to head, whether the list is previously empty or not
	}

	T* find(int key)
	{
		srand(key);				// since rand() gives pseudo-random numbers, seeding with key gives same values oa and b as before
		int A = rand() % P, B = rand() % P;

		T* value = NULL;

		int hash = ((A * key + B) % P) % tableSize;
		for (list<T>::iterator iter = hashTable[hash].begin(); iter != hashTable[hash].end() && !value; iter++)
			if (*iter == key)	// checked via overloaded == operator in BookRecord class, which compares with a given ISBN
				value = &*iter;	// if found, return pointer

		return value;			// otherwise, NULL is returned to show not found
	}

	bool remove(int key)
	{
		srand(key);
		int A = rand() % P, B = rand() % P;

		bool done = false;

		int hash = ((A * key + B) % P) % tableSize;
		for (list<T>::iterator iter = hashTable[hash].begin(); iter != hashTable[hash].end(); iter++)
			if (*iter == key)
			{
				hashTable[hash].erase(iter);		// if found, deleted and done = true returned
				done = true;
				break;
			}

		return done;								// otherwise, done = false is returned
	}

	// no destructor required as no dynamically allocated data
};

class BookRecord
{
private:

	const int ISBN;				// const as ISBN of a book cannot be changed

	string title;

	list<string> authors;

	int reference_books;
	int issuable_books;

public:

	BookRecord(int _ISBN, const string& _title, const list<string>& _authors, int ref_books, int issue_books) : ISBN(_ISBN)
	{											// ISBN initialised via member initialiser list as it is a const data member
		title = _title;
		
		authors = _authors;

		reference_books = ref_books;
		issuable_books = issue_books;
	}

	void updateTitle(const string& _title)
	{
		title = _title;
	}

	void updateAuthors(const list<string>& _authors)
	{
		authors = _authors;
	}

	void updateReferenceBooks(int ref_books)
	{
		reference_books = ref_books;
	}

	void updateIssuableBooks(int issue_books)
	{
		issuable_books = issue_books;
	}

	bool operator==(int _ISBN)		// so that ISBN can be used as an integer ID for finding a BookRecord from a list (e.g. via find())
	{
		return (ISBN == _ISBN);
	}

	friend ostream& operator<<(ostream& out, BookRecord& bookRecord)	// operator << overloaded to provide print() functionality
	{
		out << "\n\t\tISBN:\t" << bookRecord.ISBN << endl;
		out << "\t\t|" << endl;

		out << "\t\t|\tTitle:\t\t" << bookRecord.title << endl;
		out << "\t\t|\tAuthors:\n";	// printing via iterator as authors is list<string>:

		for (list<string>::iterator iter = bookRecord.authors.begin(); iter != bookRecord.authors.end(); iter++)
			cout << "\t\t|\t\t\t" << *iter << endl;
		
		out << "\t\t|\tNo. of Reference Editions:\t" << bookRecord.reference_books << endl;
		out << "\t\t|\tNo. of Issuable Editions:\t" << bookRecord.issuable_books << endl;

		out << "\t\t|" << endl;
		return out;
	}

	// no destructor required as no dynamically allocated data
};

class LibraryManagementSystem
{
private:

	HashTable<BookRecord> bookRecords;

public:

	LibraryManagementSystem() : bookRecords(HashTable<BookRecord>(HASHTABLE_SIZE, P_ABOVE_T_SIZE)) {}	// these are passed as arguments to
																										// constructor of hash table
	void userInterface()
	{
		cout << line << endl << "\t\t\tWelcome to the Library Management System (LMS)!" << endl;
		
		int choice = 0;
		bool valid = false;
		
		int ISBN = 0;
		
		while (choice != Q)
		{
			cout << line << endl << endl;
			cout << "\t\t\t-> Press C to create a new book record." << endl;
			cout << "\t\t\t-> Press R to review an existing book record." << endl;
			cout << "\t\t\t-> Press U to update an existing book record." << endl;
			cout << "\t\t\t-> Press D to delete an old book record." << endl;
			cout << "\t\t\t-> Press Q to quit the LMS." << endl;
			cout << endl << line << endl;
		
			valid = false;
		
			while (!valid)	// for taking input until valid input arrives
			{
				valid = true;
		
				switch (choice = _getch())
				{
				case C:
		
					cout << "\n\tPlease enter the ISBN number for the record you wish to create: ";
					while (!(cin >> ISBN)){  // to ensure that only an integer is input into ISBN, not a char or string
						cin.clear();		// if input is of wrong type, cin enters fail state, we clear its state
						cin.ignore(numeric_limits<streamsize>::max(), '\n');	// and ignore whatever is in the input buffer
						cout << "\tError: Invalid Input! Please try again." << endl;	// and as user to reenter correct value
						cout << "\n\tPlease enter the ISBN number for the record you wish to create: ";
					}
					cin.ignore();	// to prevent data inside buffer from spoiling next input
		
					createBookRecord(ISBN);
		
					break;
		
				case R:
		
					cout << "\n\tPlease enter the ISBN number for the record you wish to review: ";
					while (!(cin >> ISBN)){  // to ensure that only an integer is input into ISBN, not a char or string
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\tError: Invalid Input! Please try again." << endl;
						cout << "\n\tPlease enter the ISBN number for the record you wish to review: ";
					}
					cin.ignore();

					reviewBookRecord(ISBN);
		
					break;
		
				case U:
		
					cout << "\n\tPlease enter the ISBN number for the record you wish to update: ";
					while (!(cin >> ISBN)){  // to ensure that only an integer is input into ISBN, not a char or string
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\tError: Invalid Input! Please try again." << endl;
						cout << "\n\tPlease enter the ISBN number for the record you wish to update: ";
					}
					cin.ignore();

					updateBookRecord(ISBN);
		
					break;
		
				case D:
		
					cout << "\n\tPlease enter the ISBN number for the record you wish to delete: ";
					while (!(cin >> ISBN)){  // to ensure that only an integer is input into ISBN, not a char or string
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\tError: Invalid Input! Please try again." << endl;
						cout << "\n\tPlease enter the ISBN number for the record you wish to delete: ";
					}
					cin.ignore();

					deleteBookRecord(ISBN);
		
					break;
		
				case Q:
		
					cout << "\tYou have chosen to exit the library management system. Thank you for using LMS!" << endl;
		
					break;
		
				default:
		
					valid = false;
				}
			}
		
			cout << endl;
		}
	}

	// no destructor required as no dynamically allocated data

private:

	// these functions kept private as only called from public userInterface() function, i.e. are utility functions

	void createBookRecord(int ISBN)
	{
		if (!bookRecords.find(ISBN))	// if already exists, cannot allow duplicates, user should update that record instead
		{
			string title;
			string author;
			
			int totalAuthors = 0;
			list<string> authors;
			
			int refbooks = 0;
			int issuebooks = 0;
			
			cout << "\n\t\tPlease enter the title of the book: ";
			getline(cin, title);
			
			while (totalAuthors <= 0)
			{
				cout << "\t\tPlease enter the number of authors of this book: ";
				while (!(cin >> totalAuthors)){  // to ensure that only an integer is input into ISBN, not a char or string
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\t\tError: Invalid Input! Please try again." << endl;
					cout << "\n\t\tPlease enter the number of authors of this book: ";
				}
				cin.ignore();
			
				if (totalAuthors <= 0)	// number of authors can also not be negative, otherwise the subsequent loop will run infinitely
					cout << "\t\tError: Invalid Input! Please try again." << endl;
			}
			
			cout << "\t\tPlease enter the names of the authors:\n";
			for (int i = 1; i <= totalAuthors; i++)
			{
				cout << "\t\t\t\t\tAuthor " << i << ": ";
				getline(cin, author);
				authors.push_back(author);	// author name added to list of authors
			}
			
			cout << "\t\tPlease enter the number of available reference editions of this books: ";
			cin >> refbooks;
			
			cout << "\t\tPlease enter the number of available issuable editions of this books: ";
			cin >> issuebooks;
			
			BookRecord newRecord(ISBN, title, authors, refbooks, issuebooks);	// new book record created
			bookRecords.insert(ISBN, newRecord);	// newly created book record inserted into hash table
		}
		else
			cout << "\n\t\tRecord of book with given ISBN already exists in database!" << endl;
	}
	
	void reviewBookRecord(int ISBN)
	{
		BookRecord* desiredRecord = bookRecords.find(ISBN); // returns NULL if not found
		
		if (desiredRecord)	// i.e. if found
			cout << *desiredRecord; // diplayed via overloaded << operator for class BookRecord, accessed by dereferencing returned pointer
		else
			cout << "\n\t\tRecord of book with given ISBN number does not exist in the database!" << endl;
	}

	void updateBookRecord(int ISBN)
	{
		BookRecord* desiredRecord = bookRecords.find(ISBN);

		if (desiredRecord)
		{
			int choice = 0;
			bool valid = false;
		
			cout << endl;
			cout << "\t\tPress T to update the title." << endl;
			cout << "\t\tPress A to update the authors." << endl;
			cout << "\t\tPress R to update the number of reference books." << endl;
			cout << "\t\tPress I to update the number of issuable books." << endl;
		
			string title;
			string author;
		
			list<string> authors;
		
			int tempnum = 0;
		
			while (!valid)
			{
				valid = true;
		
				switch (choice = _getch())
				{
				case t:
		
					cout << "\n\t\t\tPlease enter the updated title: ";
					getline(cin, title);
					desiredRecord->updateTitle(title);
		
					break;
		
				case a:
		
					while (tempnum <= 0)
					{
						cout << "\n\t\t\tPlease enter the number of authors of this book: ";
						while (!(cin >> tempnum)){  // to ensure that only an integer is input into ISBN, not a char or string
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "\t\t\tError: Invalid Input! Please try again." << endl;
							cout << "\n\t\tPlease enter the number of authors of this book: ";
						}
						cin.ignore();
		
						if (tempnum <= 0)
							cout << "\t\t\tError: Invalid Input! Please try again." << endl;
					}
		
					cout << "\t\t\tPlease enter the names of the authors:\n";
					for (int i = 1; i <= tempnum; i++)
					{
						cout << "\t\t\t\t\t\tAuthor " << i << ": ";
						getline(cin, author);
						authors.push_back(author);
					}
		
					desiredRecord->updateAuthors(authors);
		
					break;
		
				case rb:
		
					cout << "\n\t\t\tPlease enter the updated number of reference books: ";
					cin >> tempnum;
					desiredRecord->updateReferenceBooks(tempnum);
		
					break;
		
				case ib:
		
					cout << "\n\t\t\tPlease enter the updated number of issuable books: ";
					cin >> tempnum;
					desiredRecord->updateIssuableBooks(tempnum);
		
					break;
		
				default:
		
					valid = false;
				}
			}
		
			cout << "\n\t\tRecord of book with given ISBN number has been updated successfully!" << endl;
		}
		else
			cout << "\n\t\tRecord of book with given ISBN number does not exist in the database!" << endl;
	}

	void deleteBookRecord(int ISBN)
	{
		if (bookRecords.remove(ISBN))	// returns true if deleted, false if not found (and hence not deleted)
			cout << "\n\t\tRecord of book with ISBN " << ISBN << " has been deleted successfully!" << endl;
		else
			cout << "\n\t\tRecord of book with given ISBN number does not exist in the database!" << endl;
	}
};

void main()
{
	LibraryManagementSystem LMS;
	LMS.userInterface();
	system("PAUSE");
}