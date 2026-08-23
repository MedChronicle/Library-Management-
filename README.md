# Digital Library Resource Management System

A console-based **Digital Library Resource Management System** developed in **C++** using Object-Oriented Programming concepts.

The system manages different types of library resources such as **Books, Journals, and DVDs**, along with library members, borrowing transactions, reservations, fines, inventory management, and file-based inventory storage.

---

## 📌 Project Overview

The Digital Library Resource Management System provides a complete command-line interface for managing library resources and members.

The project demonstrates practical implementation of:

* Object-Oriented Programming
* Inheritance
* Polymorphism
* Abstraction
* Encapsulation
* Function Overloading
* Operator Overloading
* Friend Functions
* Static Data Members
* Exception Handling
* STL Containers
* File Handling
* Searching and Sorting
* Queue-based Waitlists
* Transaction Management

The main `Library` class manages resources, members, transactions, and resource waitlists.

---

## ✨ Features

### 📚 Resource Management

The system supports three resource types:

* Books
* Journals
* DVDs

Users can:

* Add new resources
* Remove resources
* Display all resources
* Display resources by type
* Search resources by title
* Search books by author
* Search books by genre
* Find resources above a specific price
* Compare two resources by price
* Sort resources by price
* Sort resources alphabetically by title

The project uses a common abstract `Resource` base class with specialized `Book`, `Journal`, and `DVD` classes.

---

## 👤 Member Management

The system allows library members to be registered with different membership types:

* Student
* Faculty
* Guest

Each membership type has a different borrowing limit:

| Membership | Maximum Resources |
| ---------- | ----------------: |
| Student    |                 5 |
| Faculty    |                10 |
| Guest      |                 2 |

The system also maintains member borrowing information and outstanding fines.

---

## 🔄 Borrowing & Transactions

Members can borrow and return library resources.

The transaction system records:

* Transaction ID
* Member ID
* Resource ID
* Issue date
* Due date
* Return date
* Return status
* Fine amount

Transaction IDs start from `1001` and are automatically incremented.

The system supports:

* Issuing resources
* Returning resources
* Renewing loans
* Viewing active transactions
* Viewing overdue transactions
* Viewing member borrowing history
* Finding the most borrowed resources

---

## ⏳ Reservation & Waitlist System

When a resource has no available copies, members can join its waitlist.

The system provides:

* Reserve a resource
* Cancel a reservation
* Display a resource waitlist
* Automatic notification when a returned resource becomes available
* Prevention of duplicate reservations

A `queue<int>` is used to maintain the waitlist for each resource.

---

## 💰 Fine Management

Different resource types have different late-fee rates.

| Resource |  Late Fee |
| -------- | --------: |
| Book     |    ₹2/day |
| Journal  | ₹1.50/day |
| DVD      |    ₹3/day |

The system automatically calculates fines based on the number of days late.

Members can also make payments toward their outstanding fines.

---

## 💾 File Handling

The system supports saving and loading inventory information using files.

Inventory can be exported to a file containing information such as:

* Resource ID
* Resource type
* Title
* Price
* Date added
* Author
* ISBN
* Genre
* Publication information
* Number of copies
* Availability
* Borrowing statistics

The inventory is stored in a structured CSV-style format.

---

## 📊 Library Statistics

The system can generate statistics including:

* Total resources
* Total books
* Total journals
* Total DVDs
* Total members
* Total inventory value
* Average resource price
* Average book price
* Average journal price
* Average DVD price
* Most borrowed resources

An inventory report can also be generated for the complete library.

---

## 🛡️ Exception Handling

The project contains a custom exception hierarchy derived from `LibraryException`.

Examples include:

* `InvalidPriceException`
* `InvalidResourceIDException`
* `InvalidTitleException`
* `ResourceNotFoundException`
* `DuplicateResourceException`
* `InvalidQuantityException`
* `MemberNotFoundException`
* `ResourceUnavailableException`
* `InvalidYearException`
* `DuplicateMemberException`
* `TransactionNotFoundException`
* `InvalidImpactFactorException`
* `InvalidFieldException`
* `BorrowLimitExceededException`
* `AlreadyReservedException`
* `ReservationNotFoundException`

These exceptions prevent invalid data and invalid operations from crashing the program.

The program also contains a dedicated exception-handling demonstration with invalid books, journals, DVDs, and members.

---

## 🧠 OOP Concepts Used

### 1. Abstraction

`Resource` is an abstract base class containing pure virtual functions such as:

```cpp
virtual void displayDetails() const = 0;
virtual string getResourceType() const = 0;
virtual double calculateLateFee(int daysLate) const = 0;
```

This provides a common interface for different resource types.

### 2. Inheritance

The following classes inherit from `Resource`:

```text
             Resource
             /   |   \
            /    |    \
         Book  Journal  DVD
```

### 3. Polymorphism

Resources are stored using:

```cpp
vector<Resource*> resources;
```

This allows `Book`, `Journal`, and `DVD` objects to be handled through the common `Resource` interface.

### 4. Encapsulation

Class data members are protected/private and accessed through member functions such as getters and setters.

### 5. Function Overloading

Examples include:

```cpp
void issueCopies(int quantity);
void issueCopies();
```

and:

```cpp
void returnCopies(int quantity);
void returnCopies();
```

### 6. Operator Overloading

The project overloads the `<<` operator to display resource information in a formatted manner.

### 7. Friend Functions

Friend functions are used for resource price comparison and price difference calculations.

### 8. Static Members

Static members maintain global statistics such as:

* Total resources
* Total books
* Total journals
* Total DVDs
* Total members
* Total inventory value

---

## 🗂️ STL Concepts Used

The project uses several C++ Standard Template Library components:

```cpp
vector
map
queue
algorithm
numeric
string
fstream
sstream
```

Examples include:

* `vector` → resources, members, transactions
* `queue` → reservation waitlists
* `map` → resource-to-waitlist mapping
* `sort()` → sorting resources
* `find_if()` → searching
* `copy_if()` → filtering
* `accumulate()` → calculating totals
* `max_element()` → finding the most expensive resource
* `min_element()` → finding the cheapest resource

---

## 🖥️ Menu Options

The program provides an interactive console menu with **33 operations**.

```text
1.  Add New Book
2.  Add New Journal
3.  Display All Resources
4.  Display All Books
5.  Display All Journals
6.  Search Resource by Title
7.  Display Resources Above Price
8.  Compare Two Resources by Price
9.  Sort Resources by Price
10. Sort Resources by Title
11. Add New Member
12. Issue Resource to Member
13. Return Resource
14. Display Member Borrowing History
15. Display Active Transactions
16. Display Library Statistics
17. Generate Inventory Report
18. Save Inventory to File
19. Remove Resource
20. Apply Discount to All Resources
21. Apply Discount by Resource Type
22. Add New DVD
23. Display All DVDs
24. Search Resources by Author
25. Search Resources by Genre
26. Reserve a Resource
27. Cancel a Reservation
28. Display Waitlist
29. Renew a Loan
30. Display Overdue Transactions
31. Display Most Borrowed Resources
32. Pay Member Fine
33. Load Inventory from File
0.  Exit
```

These options correspond to the interactive menu implemented in the program.

---

## 🧪 Sample Data

The program automatically loads sample library data when it starts.

Example resources include:

* *The Pragmatic Programmer*
* *Introduction to Algorithms*
* *Clean Code*
* *Design Patterns*
* *Nature*
* *IEEE Transactions on AI*
* *Journal of Data Science*
* *The Social Network*
* *Inception*

Sample members are also created for demonstration and testing.

---

## ⚙️ Requirements

To compile and run the project, you need:

* C++ compiler
* C++11 or later
* Terminal / Command Prompt
* Any C++ IDE such as:

  * Visual Studio Code
  * Code::Blocks
  * CLion
  * Visual Studio

---

## 🚀 How to Run

### Step 1: Save the source code

Save the program as:

```text
main.cpp
```

### Step 2: Compile

Using `g++`:

```bash
g++ main.cpp -o library
```

### Step 3: Run

On macOS/Linux:

```bash
./library
```

On Windows:

```bash
library.exe
```

---

## 📁 File Structure

A simple project structure can be:

```text
Digital-Library-Management/
│
├── main.cpp
├── inventory.csv
└── README.md
```

Where:

* `main.cpp` → Main C++ implementation
* `inventory.csv` → Saved inventory data
* `README.md` → Project documentation

---

## 🔐 Data Validation

The system validates important inputs before accepting them.

Examples include:

* Resource IDs must be positive.
* Prices cannot be negative.
* Titles cannot be empty.
* Number of copies must be positive.
* Publication years must be within the accepted range.
* Journal impact factors cannot be negative.
* Member emails must contain `@`.
* Membership types must be Student, Faculty, or Guest.
* Borrowing limits are enforced.
* Duplicate resource IDs are rejected.
* Duplicate member IDs are rejected.

This validation is implemented through the custom exception classes.

---

## 🎯 Project Objectives

The main objectives of this project are:

1. Build a complete console-based library management system.
2. Demonstrate core C++ OOP concepts.
3. Implement inheritance and runtime polymorphism.
4. Practice STL containers and algorithms.
5. Implement custom exception handling.
6. Manage borrowing and returning transactions.
7. Implement resource reservation using queues.
8. Implement file-based inventory persistence.
9. Generate useful library statistics.
10. Create a realistic application combining multiple C++ concepts.

---

## 👨‍💻 Author

**Arpit Saharan**

C++ Object-Oriented Programming Project

---

## 📄 License

This project is intended for **educational and academic purposes**.
