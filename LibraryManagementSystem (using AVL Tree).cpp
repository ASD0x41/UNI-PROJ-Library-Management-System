// Muhammad Asad Tariq (21L-5266) --- BCS-3F
// Data Structures --- Assignment 3

#include <iostream>
#include <conio.h>
#include <string>
using namespace std;

// Definitions of Constants for Input of Keys From Keyboard:

#define	C	99
#define	R	114
#define	U	117
#define	D	100
#define Q	113
#define t	116
#define A	97
#define I	105

// Horizontal line has to be displayed multiple times on the interface, so:

const char line[] = "-------------------------------------------------------------------------------------------------";

// helper function for finding maximum of two integers:

int max(int lhs, int rhs)
{
	int maximum = lhs;

	if (rhs > lhs)
		maximum = rhs;

	return maximum;
}

// Class Template of Singly-Linked List (for storing list of authors in each book record:
			// all operations related to authors list are being considered O(1)
			// as num of authors <<< num of records (n), so it is not O(n)

template <typename T>
class SLList
{
private:

	struct SLLNode
	{
		T data;
		SLLNode* next;

		SLLNode(const T& _data, SLLNode* _next = NULL) : data(_data), next(_next) {}
	};

	SLLNode* head;
	SLLNode* tail;

public:

	SLList() : head(NULL), tail(NULL) {}

	SLList(const SLList<T>& rhs) : head(NULL), tail(NULL)
	{
		SLLNode* temp = rhs.head;

		while (temp)
		{
			insertAtTail(temp->data);
			temp = temp->next;
		}
	}

	void insertAtHead(const T& key)
	{
		if (head)
			head = new SLLNode(key, head);
		else
			tail = head = new SLLNode(key);
	}

	void insertAtTail(const T& key)
	{
		if (tail)
			tail = tail->next = new SLLNode(key);
		else
			head = tail = new SLLNode(key);
	}

	// overloaded stream insertion operator which acts a print list function (to print list of authors):

	friend ostream& operator<<(ostream& out, const SLList<T>& sllist)
	{
		SLLNode* temp = sllist.head;
		while (temp)
		{
			out << "\t\t|\t\t\t" << temp->data << endl;
			temp = temp->next;
		}
		return out;
	}

	// bool returned to give info whether deletion occurred successfully or not (if false is returned, it means list is empty)

	bool deleteFromHead()
	{
		bool done = false;

		if (head)
		{
			SLLNode* temp = head;
			head = head->next;
			delete temp;

			if (!head)
				tail = NULL;

			done = true;
		}

		return done;
	}

	~SLList()
	{
		SLLNode* temp;
		while (head)
		{
			temp = head;
			head = head->next;
			delete temp;
		}
	}
};

// Class template of doubly-linked list (for storing list of book records):

template <typename T>
class DLList
{
private:

	struct DLLNode
	{
		T data;

		DLLNode* prev;
		DLLNode* next;

		DLLNode(const T& _data, DLLNode* _prev = NULL, DLLNode* _next = NULL) : data(_data), prev(_prev), next(_next) {}
	};

	DLLNode* head;
	DLLNode* tail;

	// Iterator used to provide access to the nodes of this list from outside (i.e. from the AVL tree representing the index):

	class DLLIterator;

public:

	typedef DLLIterator Iter;

	Iter begin()
	{
		return Iter(head);
	}

	Iter end()
	{
		return Iter(tail);
	}

	DLList() : head(NULL), tail(NULL) {}

	void insertAtHead(const T& key)
	{
		if (head)
			head = head->prev = new DLLNode(key, NULL, head);
		else
			tail = head = new DLLNode(key);
	}

	Iter insertNodeBefore(Iter iter, const T& key)	// inserts a node before the node provided via iterator
	{
		return Iter(insertNodeBefore(iter.iptr, key));
	}

	void insertAtTail(const T& key)
	{
		if (tail)
			tail = tail->next = new DLLNode(key, tail);
		else
			head = tail = new DLLNode(key);
	}

	Iter insertNodeAfter(Iter iter, const T& key)
	{
		return Iter(insertNodeAfter(iter.iptr, key));
	}

	bool deleteFromHead()
	{
		bool done = false;

		if (head)
		{
			DLLNode* temp = head;
			head = head->next;
			delete temp;

			if (!head)
				tail = NULL;

			done = true;
		}

		return done;
	}

	bool deleteNode(const Iter& iter)		// delete the node whose iterator is passed as parameter
	{
		bool done = false;

		DLLNode* node = iter.iptr;
		if (deleteNode(node))
			done = true;

		return done;
	}

	bool deleteFromTail()
	{
		bool done = false;

		if (head)
		{
			DLLNode* temp = tail;
			tail = tail->prev;
			delete temp;

			if (!tail)
				head = NULL;

			done = true;
		}

		return done;
	}

	~DLList()
	{
		DLLNode* temp;
		while (head)
		{
			temp = head;
			head = head->next;
			delete temp;
		}
	}

private:

	DLLNode* insertNodeBefore(DLLNode* node, const T& key)
	{
		DLLNode* newNode = NULL;

		if (node)
		{
			node->prev = newNode = new DLLNode(key, node->prev, node);

			if (newNode->prev)
				newNode->prev->next = newNode;
			else
				head = newNode;
		}
		else
		{
			insertAtTail(key);		// if NULL is provided as parameter, the function acts as insertAtTail()
			newNode = tail;
		}

		return newNode;
	}

	DLLNode* insertNodeAfter(DLLNode* node, const T& key)
	{
		DLLNode* newNode = NULL;

		if (node)
		{
			node->next = newNode = new DLLNode(key, node, node->next);

			if (newNode->next)
				newNode->next->prev = newNode;
			else
				tail = newNode;
		}
		else
		{
			insertAtHead(key);
			newNode = head;
		}

		return newNode;
	}

	bool deleteNode(DLLNode* node)
	{
		bool done = false;

		if (node)
		{
			if (node->prev)		// i.e. the node is not the head (the first node)
				node->prev->next = node->next;
			else
				head = head->next;

			if (node->next)		// i.e. the node is not the tail (the last node)
				node->next->prev = node->prev;
			else
				tail = tail->prev;

			delete node;
			done = true;
		}

		return done;
	}
};

template <typename T>
class DLList<T>::DLLIterator
{
	friend class DLList<T>;	// to provide access to iter.iptr inside class DLList for insertion and deletion-related functions

private:

	DLLNode* iptr;

public:

	DLLIterator(DLLNode* node = NULL) : iptr(node) {}

	T& operator*()
	{
		return iptr->data;		// *iter returns data of the node the iterator is on
	}

	// overloaded operators:

	bool operator==(const Iter& rhs)
	{
		return (iptr == rhs.iptr);
	}

	bool operator!=(const Iter& rhs)
	{
		return !(iptr == rhs.iptr);
	}

	Iter& operator++()
	{
		if (iptr)
			iptr = iptr->next;
		return (*this);
	}

	Iter operator++(int)
	{
		Iter old(*this);
		if (iptr)
			iptr = iptr->next;
		return old;
	}

	Iter& operator--()
	{
		if (iptr)
			iptr = iptr->prev;
		return (*this);
	}

	Iter operator--(int)
	{
		Iter old(*this);
		if (iptr)
			iptr = iptr->prev;
		return old;
	}
};

template <typename T>
class AVLTree	// without allowing duplicates
{
private:

	struct AVLNode
	{
		T data;

		AVLNode* left;
		AVLNode* right;

		int height;

		AVLNode(const T& _data, AVLNode* _left = NULL, AVLNode* _right = NULL) : data(_data), left(_left), right(_right)
		{
			computeHeight(this);	// not needed necessarily, but implemented this way to increase utility
									// e.g. in case of new AVLNode(node->data, clone(node->left), clone(node->right) etc...
		}
	};

	AVLNode* root;

public:

	AVLTree() : root(NULL) {}

	bool insertNode(const T& item)
	{
		return insertNode(root, item);
	}

	bool findNode(const T& item)
	{
		bool found = false;

		if (findNode(root, item))
			found = true;

		return found;
	}

	T* findNodeData(const int key)	// find using an int ID and return pointer to data
	{
		AVLNode* foundNode = findNode(root, key);

		T* foundData = NULL;
		if (foundNode)				// if not found, returns NULL pointer
			foundData = &foundNode->data;
		return foundData;
	}

	T* findClosestBelow(const int key)		// this function returns the greatest data smaller than or equal to the key
	{										// so it find the predecessor of a node in terms of the order of the data
		AVLNode* temp = NULL;
		AVLNode* closestBelow = findClosestBelow(root, key, temp);

		T* belowData = NULL;
		if (closestBelow)
			belowData = &closestBelow->data;
		return belowData;
	}

	bool deleteNode(const T& item)
	{
		return deleteNode(root, item);
	}

	~AVLTree()
	{
		destroySubTree(root);
	}

private:

	bool insertNode(AVLNode*& node, const T& item)
	{
		bool done = true;

		if (node)
		{
			if (node->data == item)
				done = false;
			else
			{
				if (node->data > item)
					done = insertNode(node->left, item);
				else
					done = insertNode(node->right, item);

				if (done)
					balanceSubTree(node);
			}
		}
		else
			node = new AVLNode(item);

		return done;
	}

	AVLNode* findNode(AVLNode* node, const T& item)
	{
		AVLNode* foundNode = node;

		if (node)
		{
			if (item < node->data)
				foundNode = findNode(node->left, item);
			else if (node->data < item)
				foundNode = findNode(node->right, item);
		}
		else
			foundNode = NULL;

		return foundNode;
	}

	AVLNode* findNode(AVLNode* node, const int key)
	{
		AVLNode* foundNode = node;

		if (node)
		{
			if (node->data > key)		// comparison operators of class T have to be overloaded to allow such comparisons in case T is not int
				foundNode = findNode(node->left, key);
			else if (node->data < key)
				foundNode = findNode(node->right, key);
		}
		else
			foundNode = NULL;

		return foundNode;
	}

	AVLNode* findClosestBelow(AVLNode* node, const int key, AVLNode* temp)
	{
		AVLNode* closestBelow = temp;

		if (node)
		{
			if (node->data == key)
				closestBelow = node;		// if found equal, no need to search for smaller value
			else if (node->data > key)
				closestBelow = findClosestBelow(node->left, key, temp);
			else if (node->data < key)
				closestBelow = findClosestBelow(node->right, key, node);
		}

		return closestBelow;
	}

	bool deleteNode(AVLNode*& node, const T& item)		// find and delete node having data item
	{
		bool done = true;

		if (node)
		{
			if (node->data == item)
				deleteNode(node);
			else
			{
				if (node->data > item)
					done = deleteNode(node->left, item);
				else
					done = deleteNode(node->right, item);

				balanceSubTree(node);
			}
		}
		else
			done = false;

		return done;
	}

	bool deleteNode(AVLNode*& node)		// delete the node whose pointer is provided
	{
		bool done = true;

		if (node)
		{
			AVLNode* temp = node;

			if (node->left && node->right)		// two children case
			{
				AVLNode* predecessor = getPredecessor(node);
				node->data = predecessor->data;
				deleteNode(node->left, predecessor->data);
			}
			else
			{
				if (node->left || node->right)		// one child case
				{
					if (node->left)
						node = node->left;
					else
						node = node->right;
				}
				else								// leaf node case
					node = NULL;

				delete temp;
			}
		}
		else
			done = false;

		return done;
	}

	AVLNode* getPredecessor(AVLNode* node)	// node with largest value from left subtree
	{
		AVLNode* predecessor = NULL;

		if (node)
		{
			predecessor = node->left;
			while (predecessor->right)
				predecessor = predecessor->right;
		}

		return predecessor;
	}

	void balanceSubTree(AVLNode*& node)
	{
		if (node)
		{
			computeHeight(node);

			switch (balanceFactor(node))
			{
			case 2:	// right side heavy

				if (height(node->right->right) >= height(node->right->left))
					rotateLeft(node);
				else
					rotateRightLeft(node);

				break;

			case -2:	// left side heavy

				if (height(node->left->left) >= height(node->left->right))
					rotateRight(node);
				else
					rotateLeftRight(node);

				break;
			}
		}
	}

	int balanceFactor(AVLNode* node)
	{
		int balFactor = 0;

		if (node)
			balFactor = height(node->right) - height(node->left);

		return balFactor;
	}

	static int height(AVLNode* node)	// function made so that we can assign and return -1 for height of NULL
	{
		int height = -1;

		if (node)
			height = node->height;

		return height;
	}

	static void computeHeight(AVLNode* node)	// used for updating heights of nodes (using heights of children) instead of the stored height
	{
		node->height = max(height(node->left), height(node->right)) + 1;
	}

	void rotateLeft(AVLNode*& X)
	{
		AVLNode* Y = X->right;
		AVLNode* orphan = Y->left;

		Y->left = X;
		X->right = orphan;

		computeHeight(X);
		computeHeight(Y);

		X = Y;
	}

	void rotateRight(AVLNode*& X)
	{
		AVLNode* Y = X->left;
		AVLNode* orphan = Y->right;

		Y->right = X;
		X->left = orphan;

		computeHeight(X);
		computeHeight(Y);

		X = Y;
	}

	void rotateLeftRight(AVLNode*& X)
	{
		rotateLeft(X->left);
		rotateRight(X);
	}

	void rotateRightLeft(AVLNode*& X)
	{
		rotateRight(X->right);
		rotateLeft(X);
	}

	void destroySubTree(AVLNode* node)
	{
		if (node)
		{
			destroySubTree(node->left);
			destroySubTree(node->right);
			delete node;
		}
	}
};

class BookRecord	// record of book
{
private:

	const int ISBN;		// const as ISBN cannot be changed

	string title;
	SLList<string>* authors;

	int reference_books;
	int issuable_books;

public:

	BookRecord(int _ISBN, string _title, SLList<string>* _authors, int _refBooks, int _issueBooks) : ISBN(_ISBN)
	{							// ISBN initialised via member initialiser list as it is a const data member
		title = _title;
		authors = new SLList<string>(*_authors);

		reference_books = _refBooks;
		issuable_books = _issueBooks;
	}

	BookRecord(const BookRecord& rhs) : ISBN(rhs.ISBN)		// copy constructor for deep copy of authors list
	{														// all operations related to authors list are being considered O(1)
															// as num of authors <<< num of records (n), so it is not O(n)
		title = rhs.title;
		authors = new SLList<string>(*rhs.authors);

		reference_books = rhs.reference_books;
		issuable_books = rhs.issuable_books;
	}

	void updateTitle(string newTitle)
	{
		title = newTitle;
	}

	void updateAuthors(SLList<string>* _authors)
	{
		delete authors;
		authors = new SLList<string>(*_authors);
	}

	void updateRefNum(int newRefNum)
	{
		reference_books = newRefNum;
	}

	void updateIssueNum(int newIssueNum)
	{
		issuable_books = newIssueNum;
	}

	// for displaying book record:

	friend ostream& operator<<(ostream& out, const BookRecord& bookrecord)
	{
		cout << "\n\t\tISBN:\t" << bookrecord.ISBN << endl;
		cout << "\t\t|" << endl;
		cout << "\t\t|\tTitle:\t\t" << bookrecord.title << endl;
		cout << "\t\t|\tAuthors:\n" << *bookrecord.authors;
		cout << "\t\t|\tNo. of Reference Editions:\t" << bookrecord.reference_books << endl;
		cout << "\t\t|\tNo. of Issuable Editions:\t" << bookrecord.issuable_books << endl;
		cout << "\t\t|" << endl;
		return out;
	}

	~BookRecord()
	{
		if (authors)
			delete authors;
	}
};

class IndexEntry
{
private:

	int ISBN;

	DLList<BookRecord>::Iter record;

public:

	IndexEntry(int _ISBN, const DLList<BookRecord>::Iter& _record) : ISBN(_ISBN), record(_record) {}

	DLList<BookRecord>::Iter operator*()	// *indexEntry returns iterator (whereas *iter returns data of node, i.e. BookRecord)
	{										// so **index_entry returns the BookRecord that it is storig the location of
		return record;
	}

	// overloaded comparison operators (as they are needed in the find functions of the AVLTree 

	bool operator==(const int rhs)
	{
		bool isEqualTo = false;
		if (ISBN == rhs)
			isEqualTo = true;
		return isEqualTo;
	}

	bool operator!=(const int rhs)
	{
		return !(*this == rhs);
	}

	bool operator<(const int rhs)
	{
		bool isSmaller = false;
		if (ISBN < rhs)
			isSmaller = true;
		return isSmaller;
	}

	bool operator>(const int rhs)
	{
		bool isGreater = false;
		if (ISBN > rhs)
			isGreater = true;
		return isGreater;
	}

	bool operator==(const IndexEntry& rhs)
	{
		return (*this == rhs.ISBN);
	}

	bool operator!=(const IndexEntry& rhs)
	{
		return !(*this == rhs);
	}

	bool operator<(const IndexEntry& rhs)
	{
		return (*this < rhs.ISBN);
	}

	bool operator>(const IndexEntry& rhs)
	{
		return (*this > rhs.ISBN);
	}
};

class LibraryManagementSystem
{
private:

	AVLTree<IndexEntry> index;

	DLList<BookRecord> books;

public:

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

			while (!valid)	// to taking input until valid input arrives
			{
				valid = true;

				switch (choice = _getch())
				{
				case C:

					cout << "\n\tPlease enter the ISBN number for the record you wish to create: ";
					cin >> ISBN;
					cin.ignore();

					createBookRecord(ISBN);

					break;

				case R:

					cout << "\n\tPlease enter the ISBN number for the record you wish to review: ";
					cin >> ISBN;
					cin.ignore();

					reviewBookRecord(ISBN);

					break;

				case U:

					cout << "\n\tPlease enter the ISBN number for the record you wish to update: ";
					cin >> ISBN;
					cin.ignore();

					updateBookRecord(ISBN);

					break;

				case D:

					cout << "\n\tPlease enter the ISBN number for the record you wish to delete: ";
					cin >> ISBN;
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

private:

	void createBookRecord(int ISBN)
	{
		if (!index.findNodeData(ISBN))	// if already exists, cannot allow duplicates, user should update that record instead
		{
			string title;
			string author;

			int totalAuthors = 0;
			SLList<string>* authors = new SLList<string>;

			int refbooks = 0;
			int issbooks = 0;

			cout << "\n\t\tPlease enter the title of the book: ";
			getline(cin, title);

			while (totalAuthors <= 0)
			{
				cout << "\t\tPlease enter the number of authors of this book: ";
				cin >> totalAuthors;
				cin.ignore();

				if (totalAuthors <= 0)
					cout << "\t\tError: Invalid Input! Please try again." << endl;
			}

			cout << "\t\tPlease enter the names of the authors:\n";
			for (int i = 1; i <= totalAuthors; i++)
			{
				cout << "\t\t\t\t\tAuthor " << i << ": ";
				getline(cin, author);
				authors->insertAtHead(author);
			}

			cout << "\t\tPlease enter the number of available reference editions of this books: ";
			cin >> refbooks;

			cout << "\t\tPlease enter the number of available issuable editions of this books: ";
			cin >> issbooks;

			BookRecord* newBook = new BookRecord(ISBN, title, authors, refbooks, issbooks);	// we create a boo record as per above data

			IndexEntry* previous = index.findClosestBelow(ISBN);	// we find previous ISBN entry (as insertion in DLList has to be in order)
			DLList<BookRecord>::Iter prevRecord(NULL);
			if (previous)	// if there is previous entry, use it, otherwise send NULL to insertNodeAfter(), which will then act as insertAtHead()
				prevRecord = **previous; // one * is dereferencing pointer, the other returns iterator stored in that IndexEntry
			index.insertNode(IndexEntry(ISBN, books.insertNodeAfter(prevRecord, *newBook)));
				// we insert the book record into the books DLList after the previous record (so list remains sorted)
				// then create an index entry using the address of that DLLNode and insert it into the index AVL tree

			delete newBook;
			delete authors;
			// this has to be done to prevent memory leakage
			// because when the references of the above dynamically alocated objects were sent to the insertion functions,
			// their deep copies were made, therefore, we have to deallocate these memory blocks here
		}
		else
			cout << "\n\t\tRecord of book with given ISBN already exists in database!" << endl;
	}

	void reviewBookRecord(int ISBN)
	{
		IndexEntry* desiredRecord = index.findNodeData(ISBN);
		if (desiredRecord) // if index entry with given ISBN is found
			cout << ***desiredRecord << endl;	// desired record is pointer to IndexEntry
												// *desiredRecord is that IndexEntry (i.e. pointer is dereferenced)
												// **desiredRecord is iterator to DLLNode (bcz of overloaded * operator in class indexEntry)
												// ***desiredRecord is BookRecord due to overloaded * operator in class DLL Iterator
		else
			cout << "\n\t\tRecord of book with given ISBN number does not exist in the database!" << endl;
	}

	void updateBookRecord(int ISBN)
	{
		IndexEntry* desiredRecord = index.findNodeData(ISBN);
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

			SLList<string>* authors = new SLList<string>;

			int tempnum = 0;

			while (!valid)
			{
				valid = true;

				switch (choice = _getch())
				{
				case t:

					cout << "\n\t\t\tPlease enter the updated title: ";
					getline(cin, title);
					(***desiredRecord).updateTitle(title);	// paranthesis required because . operator has higher precendence than *

					break;

				case A:

					while (tempnum <= 0)
					{
						cout << "\t\t\tPlease enter the number of authors of this book: ";
						cin >> tempnum;
						cin.ignore();

						if (tempnum <= 0)
							cout << "\t\t\tError: Invalid Input! Please try again." << endl;
					}

					cout << "\t\t\tPlease enter the names of the authors:\n";
					for (int i = 1; i <= tempnum; i++)
					{
						cout << "\t\t\t\t\t\tAuthor " << i << ": ";
						getline(cin, author);
						authors->insertAtHead(author);
					}

					(***desiredRecord).updateAuthors(authors);

					break;

				case R:

					cout << "\n\t\t\tPlease enter the updated number of reference books: ";
					cin >> tempnum;
					(***desiredRecord).updateRefNum(tempnum);

					break;

				case I:

					cout << "\n\t\t\tPlease enter the updated number of issuable books: ";
					cin >> tempnum;
					(***desiredRecord).updateIssueNum(tempnum);

					break;

				default:

					valid = false;
				}
			}

			cout << "\n\t\tRecord of book with given ISBN number has been updated successfully!" << endl;

			delete authors;	// need to delete to prevent memory leak (the list provided to BookRecord has been deepcopied by it)
		}
		else
			cout << "\n\t\tRecord of book with given ISBN number does not exist in the database!" << endl;
	}

	void deleteBookRecord(int ISBN)
	{
		IndexEntry* desiredRecord = index.findNodeData(ISBN);		// first we find address of that DLLNode via index
		if (desiredRecord)
		{
			books.deleteNode(**desiredRecord);			// we delete it from DLList
			index.deleteNode(*desiredRecord);			// finally, we delete the index entry corresponding to that book record

			cout << "\n\t\tRecord of book with ISBN " << ISBN << " has been deleted successfully!" << endl;
		}
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