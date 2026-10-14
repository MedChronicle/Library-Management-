#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <map>
#include <queue>

using namespace std;

class LibraryException : public exception {
protected:
    string message;
public:
    LibraryException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidPriceException : public LibraryException {
public:
    InvalidPriceException(const string& msg) : LibraryException(msg) {}
};

class InvalidResourceIDException : public LibraryException {
public:
    InvalidResourceIDException(const string& msg) : LibraryException(msg) {}
};

class InvalidTitleException : public LibraryException {
public:
    InvalidTitleException(const string& msg) : LibraryException(msg) {}
};

class ResourceNotFoundException : public LibraryException {
public:
    ResourceNotFoundException(const string& msg) : LibraryException(msg) {}
};

class DuplicateResourceException : public LibraryException {
public:
    DuplicateResourceException(const string& msg) : LibraryException(msg) {}
};

class InvalidQuantityException : public LibraryException {
public:
    InvalidQuantityException(const string& msg) : LibraryException(msg) {}
};

class MemberNotFoundException : public LibraryException {
public:
    MemberNotFoundException(const string& msg) : LibraryException(msg) {}
};

class ResourceUnavailableException : public LibraryException {
public:
    ResourceUnavailableException(const string& msg) : LibraryException(msg) {}
};

class InvalidYearException : public LibraryException {
public:
    InvalidYearException(const string& msg) : LibraryException(msg) {}
};

class DuplicateMemberException : public LibraryException {
public:
    DuplicateMemberException(const string& msg) : LibraryException(msg) {}
};

class TransactionNotFoundException : public LibraryException {
public:
    TransactionNotFoundException(const string& msg) : LibraryException(msg) {}
};

class InvalidImpactFactorException : public LibraryException {
public:
    InvalidImpactFactorException(const string& msg) : LibraryException(msg) {}
};

class InvalidFieldException : public LibraryException {
public:
    InvalidFieldException(const string& msg) : LibraryException(msg) {}
};

class BorrowLimitExceededException : public LibraryException {
public:
    BorrowLimitExceededException(const string& msg) : LibraryException(msg) {}
};

class AlreadyReservedException : public LibraryException {
public:
    AlreadyReservedException(const string& msg) : LibraryException(msg) {}
};

class ReservationNotFoundException : public LibraryException {
public:
    ReservationNotFoundException(const string& msg) : LibraryException(msg) {}
};

string getCurrentDateString() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return string(buffer);
}

string addDaysToDate(const string& dateStr, int days) {
    tm timeStruct = {};
    int year, month, day;
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_mday = day + days;
    timeStruct.tm_hour = 12;
    time_t t = mktime(&timeStruct);
    tm* newTime = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", newTime);
    return string(buffer);
}

int daysBetween(const string& startDate, const string& endDate) {
    tm t1 = {};
    tm t2 = {};
    int y, m, d;
    sscanf(startDate.c_str(), "%d-%d-%d", &y, &m, &d);
    t1.tm_year = y - 1900; t1.tm_mon = m - 1; t1.tm_mday = d; t1.tm_hour = 12;
    sscanf(endDate.c_str(), "%d-%d-%d", &y, &m, &d);
    t2.tm_year = y - 1900; t2.tm_mon = m - 1; t2.tm_mday = d; t2.tm_hour = 12;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    double diff = difftime(time2, time1);
    return static_cast<int>(diff / 86400.0);
}

void printSeparatorLine(char ch, int length) {
    cout << string(length, ch) << endl;
}

void printSeparatorLine(int length) {
    printSeparatorLine('-', length);
}

string toLowerCaseCopy(const string& input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

class Book;
class Journal;
class DVD;

class Resource {
protected:
    int resourceID;
    string title;
    double price;
    string dateAdded;
    static int totalResources;
    static double totalInventoryValue;
public:
    Resource(int id, const string& t, double p);
    virtual ~Resource();
    virtual void displayDetails() const = 0;
    virtual string getResourceType() const = 0;
    virtual double calculateLateFee(int daysLate) const = 0;
    virtual int getAvailableCopies() const = 0;
    virtual int getTotalCopies() const = 0;
    virtual void issueCopies(int quantity) = 0;
    virtual void returnCopies(int quantity) = 0;
    virtual int getTimesBorrowed() const = 0;
    int getResourceID() const;
    string getTitle() const;
    double getPrice() const;
    string getDateAdded() const;
    void setPrice(double newPrice);
    void setTitle(const string& newTitle);
    static int getTotalResources();
    static double getTotalInventoryValue();
    friend bool compareResourcePrices(const Resource& r1, const Resource& r2);
    friend double priceDifference(const Resource& r1, const Resource& r2);
    friend ostream& operator<<(ostream& os, const Resource& r);
};

int Resource::totalResources = 0;
double Resource::totalInventoryValue = 0.0;

Resource::Resource(int id, const string& t, double p) : resourceID(id), title(t), price(0.0) {
    if (id <= 0) {
        throw InvalidResourceIDException("Resource ID must be a positive integer, received: " + to_string(id));
    }
    if (t.empty()) {
        throw InvalidTitleException("Resource title cannot be an empty string");
    }
    if (p < 0) {
        throw InvalidPriceException("Resource price cannot be negative, received: " + to_string(p));
    }
    price = p;
    dateAdded = getCurrentDateString();
    totalResources++;
    totalInventoryValue += price;
    cout << "Resource object created with ID " << resourceID << " and title \"" << title << "\"" << endl;
}

Resource::~Resource() {
    totalResources--;
    totalInventoryValue -= price;
    cout << "Resource object with ID " << resourceID << " destroyed" << endl;
}

int Resource::getResourceID() const { return resourceID; }
string Resource::getTitle() const { return title; }
double Resource::getPrice() const { return price; }
string Resource::getDateAdded() const { return dateAdded; }

void Resource::setPrice(double newPrice) {
    if (newPrice < 0) {
        throw InvalidPriceException("Cannot set negative price for resource ID " + to_string(resourceID));
    }
    totalInventoryValue -= price;
    price = newPrice;
    totalInventoryValue += price;
}

void Resource::setTitle(const string& newTitle) {
    if (newTitle.empty()) {
        throw InvalidTitleException("Cannot set empty title for resource ID " + to_string(resourceID));
    }
    title = newTitle;
}

int Resource::getTotalResources() { return totalResources; }
double Resource::getTotalInventoryValue() { return totalInventoryValue; }

bool compareResourcePrices(const Resource& r1, const Resource& r2) {
    return r1.price > r2.price;
}

double priceDifference(const Resource& r1, const Resource& r2) {
    return fabs(r1.price - r2.price);
}

ostream& operator<<(ostream& os, const Resource& r) {
    os << left << setw(6) << r.resourceID << setw(32) << r.title << setw(12)
       << fixed << setprecision(2) << r.price << setw(14) << r.getResourceType()
       << setw(12) << r.dateAdded;
    return os;
}

class Book : public Resource {
private:
    string author;
    string isbn;
    string genre;
    string language;
    string format;
    int publicationYear;
    int totalCopies;
    int availableCopies;
    int timesBorrowed;
    static int totalBooksCreated;
public:
    Book(int id, const string& title, double price, const string& author, const string& isbn,
         const string& genre, int year, int copies, const string& language = "English",
         const string& format = "Paperback");
    ~Book();
    void displayDetails() const override;
    string getResourceType() const override;
    double calculateLateFee(int daysLate) const override;
    int getAvailableCopies() const override;
    int getTotalCopies() const override;
    void issueCopies(int quantity) override;
    void issueCopies();
    void returnCopies(int quantity) override;
    void returnCopies();
    string getAuthor() const;
    string getISBN() const;
    string getGenre() const;
    string getLanguage() const;
    string getFormat() const;
    int getPublicationYear() const;
    int getTimesBorrowed() const override;
    static int getTotalBooksCreated();
};

int Book::totalBooksCreated = 0;

Book::Book(int id, const string& t, double p, const string& a, const string& i, const string& g,
           int year, int copies, const string& lang, const string& fmt)
    : Resource(id, t, p), author(a), isbn(i), genre(g), language(lang), format(fmt),
      publicationYear(year), timesBorrowed(0) {
    if (year < 1450 || year > 2100) {
        throw InvalidYearException("Publication year " + to_string(year) + " is out of valid range for resource \"" + t + "\"");
    }
    if (a.empty()) {
        throw InvalidFieldException("Author name cannot be empty for book \"" + t + "\"");
    }
    if (copies <= 0) {
        throw InvalidQuantityException("Number of copies must be positive, received: " + to_string(copies));
    }
    totalCopies = copies;
    availableCopies = copies;
    totalBooksCreated++;
    cout << "Book object created: \"" << title << "\" by " << author << endl;
}

Book::~Book() {
    totalBooksCreated--;
    cout << "Book object \"" << title << "\" destroyed" << endl;
}

void Book::displayDetails() const {
    printSeparatorLine('-', 60);
    cout << "Resource Type    : Book" << endl;
    cout << "Resource ID      : " << resourceID << endl;
    cout << "Title            : " << title << endl;
    cout << "Author           : " << author << endl;
    cout << "ISBN             : " << isbn << endl;
    cout << "Genre            : " << genre << endl;
    cout << "Language         : " << language << endl;
    cout << "Format           : " << format << endl;
    cout << "Publication Year : " << publicationYear << endl;
    cout << "Price            : " << fixed << setprecision(2) << price << endl;
    cout << "Total Copies     : " << totalCopies << endl;
    cout << "Available Copies : " << availableCopies << endl;
    cout << "Times Borrowed   : " << timesBorrowed << endl;
    cout << "Date Added       : " << dateAdded << endl;
    printSeparatorLine('-', 60);
}

string Book::getResourceType() const { return "Book"; }

double Book::calculateLateFee(int daysLate) const {
    if (daysLate <= 0) return 0.0;
    return daysLate * 2.0;
}

int Book::getAvailableCopies() const { return availableCopies; }
int Book::getTotalCopies() const { return totalCopies; }

void Book::issueCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to issue must be positive for book \"" + title + "\"");
    }
    if (quantity > availableCopies) {
        throw ResourceUnavailableException("Not enough available copies of \"" + title + "\" to issue " + to_string(quantity));
    }
    availableCopies -= quantity;
    timesBorrowed += quantity;
}

void Book::issueCopies() {
    issueCopies(1);
}

void Book::returnCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to return must be positive for book \"" + title + "\"");
    }
    if (availableCopies + quantity > totalCopies) {
        throw InvalidQuantityException("Returning this many copies exceeds total copies for \"" + title + "\"");
    }
    availableCopies += quantity;
}

void Book::returnCopies() {
    returnCopies(1);
}

string Book::getAuthor() const { return author; }
string Book::getISBN() const { return isbn; }
string Book::getGenre() const { return genre; }
string Book::getLanguage() const { return language; }
string Book::getFormat() const { return format; }
int Book::getPublicationYear() const { return publicationYear; }
int Book::getTimesBorrowed() const { return timesBorrowed; }
int Book::getTotalBooksCreated() { return totalBooksCreated; }

class Journal : public Resource {
private:
    string issn;
    string publisher;
    int volume;
    int issueNumber;
    string frequency;
    double impactFactor;
    string indexingDatabase;
    int totalCopies;
    int availableCopies;
    int timesBorrowed;
    static int totalJournalsCreated;
public:
    Journal(int id, const string& title, double price, const string& issn, const string& publisher,
            int volume, int issueNumber, const string& frequency, int copies,
            double impactFactor = 0.0, const string& indexingDatabase = "Scopus");
    ~Journal();
    void displayDetails() const override;
    string getResourceType() const override;
    double calculateLateFee(int daysLate) const override;
    int getAvailableCopies() const override;
    int getTotalCopies() const override;
    void issueCopies(int quantity) override;
    void issueCopies();
    void returnCopies(int quantity) override;
    void returnCopies();
    string getISSN() const;
    string getPublisher() const;
    int getVolume() const;
    int getIssueNumber() const;
    string getFrequency() const;
    double getImpactFactor() const;
    string getIndexingDatabase() const;
    int getTimesBorrowed() const override;
    static int getTotalJournalsCreated();
};

int Journal::totalJournalsCreated = 0;

Journal::Journal(int id, const string& t, double p, const string& iss, const string& pub, int vol,
                  int issNo, const string& freq, int copies, double impact, const string& indexDb)
    : Resource(id, t, p), issn(iss), publisher(pub), volume(vol), issueNumber(issNo),
      frequency(freq), impactFactor(impact), indexingDatabase(indexDb), timesBorrowed(0) {
    if (vol <= 0) {
        throw InvalidQuantityException("Volume number must be positive for journal \"" + t + "\"");
    }
    if (issNo <= 0) {
        throw InvalidQuantityException("Issue number must be positive for journal \"" + t + "\"");
    }
    if (impact < 0) {
        throw InvalidImpactFactorException("Impact factor cannot be negative for journal \"" + t + "\"");
    }
    if (copies <= 0) {
        throw InvalidQuantityException("Number of copies must be positive, received: " + to_string(copies));
    }
    totalCopies = copies;
    availableCopies = copies;
    totalJournalsCreated++;
    cout << "Journal object created: \"" << title << "\" published by " << publisher << endl;
}

Journal::~Journal() {
    totalJournalsCreated--;
    cout << "Journal object \"" << title << "\" destroyed" << endl;
}

void Journal::displayDetails() const {
    printSeparatorLine('-', 60);
    cout << "Resource Type    : Journal" << endl;
    cout << "Resource ID      : " << resourceID << endl;
    cout << "Title            : " << title << endl;
    cout << "ISSN             : " << issn << endl;
    cout << "Publisher        : " << publisher << endl;
    cout << "Volume           : " << volume << endl;
    cout << "Issue Number     : " << issueNumber << endl;
    cout << "Frequency        : " << frequency << endl;
    cout << "Impact Factor    : " << fixed << setprecision(2) << impactFactor << endl;
    cout << "Indexed In       : " << indexingDatabase << endl;
    cout << "Price            : " << fixed << setprecision(2) << price << endl;
    cout << "Total Copies     : " << totalCopies << endl;
    cout << "Available Copies : " << availableCopies << endl;
    cout << "Times Borrowed   : " << timesBorrowed << endl;
    cout << "Date Added       : " << dateAdded << endl;
    printSeparatorLine('-', 60);
}

string Journal::getResourceType() const { return "Journal"; }

double Journal::calculateLateFee(int daysLate) const {
    if (daysLate <= 0) return 0.0;
    return daysLate * 1.5;
}

int Journal::getAvailableCopies() const { return availableCopies; }
int Journal::getTotalCopies() const { return totalCopies; }

void Journal::issueCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to issue must be positive for journal \"" + title + "\"");
    }
    if (quantity > availableCopies) {
        throw ResourceUnavailableException("Not enough available copies of \"" + title + "\" to issue " + to_string(quantity));
    }
    availableCopies -= quantity;
    timesBorrowed += quantity;
}

void Journal::issueCopies() {
    issueCopies(1);
}

void Journal::returnCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to return must be positive for journal \"" + title + "\"");
    }
    if (availableCopies + quantity > totalCopies) {
        throw InvalidQuantityException("Returning this many copies exceeds total copies for \"" + title + "\"");
    }
    availableCopies += quantity;
}

void Journal::returnCopies() {
    returnCopies(1);
}

string Journal::getISSN() const { return issn; }
string Journal::getPublisher() const { return publisher; }
int Journal::getVolume() const { return volume; }
int Journal::getIssueNumber() const { return issueNumber; }
string Journal::getFrequency() const { return frequency; }
double Journal::getImpactFactor() const { return impactFactor; }
string Journal::getIndexingDatabase() const { return indexingDatabase; }
int Journal::getTimesBorrowed() const { return timesBorrowed; }
int Journal::getTotalJournalsCreated() { return totalJournalsCreated; }

class DVD : public Resource {
private:
    string director;
    int runtimeMinutes;
    string ageRating;
    int totalCopies;
    int availableCopies;
    int timesBorrowed;
    static int totalDVDsCreated;
public:
    DVD(int id, const string& title, double price, const string& director, int runtimeMinutes,
        const string& ageRating, int copies);
    ~DVD();
    void displayDetails() const override;
    string getResourceType() const override;
    double calculateLateFee(int daysLate) const override;
    int getAvailableCopies() const override;
    int getTotalCopies() const override;
    void issueCopies(int quantity) override;
    void issueCopies();
    void returnCopies(int quantity) override;
    void returnCopies();
    string getDirector() const;
    int getRuntimeMinutes() const;
    string getAgeRating() const;
    int getTimesBorrowed() const override;
    static int getTotalDVDsCreated();
};

int DVD::totalDVDsCreated = 0;

DVD::DVD(int id, const string& t, double p, const string& dir, int runtime, const string& rating, int copies)
    : Resource(id, t, p), director(dir), runtimeMinutes(runtime), ageRating(rating), timesBorrowed(0) {
    if (dir.empty()) {
        throw InvalidFieldException("Director name cannot be empty for DVD \"" + t + "\"");
    }
    if (runtime <= 0) {
        throw InvalidQuantityException("Runtime must be a positive number of minutes for DVD \"" + t + "\"");
    }
    if (rating.empty()) {
        throw InvalidFieldException("Age rating cannot be empty for DVD \"" + t + "\"");
    }
    if (copies <= 0) {
        throw InvalidQuantityException("Number of copies must be positive, received: " + to_string(copies));
    }
    totalCopies = copies;
    availableCopies = copies;
    totalDVDsCreated++;
    cout << "DVD object created: \"" << title << "\" directed by " << director << endl;
}

DVD::~DVD() {
    totalDVDsCreated--;
    cout << "DVD object \"" << title << "\" destroyed" << endl;
}

void DVD::displayDetails() const {
    printSeparatorLine('-', 60);
    cout << "Resource Type    : DVD" << endl;
    cout << "Resource ID      : " << resourceID << endl;
    cout << "Title            : " << title << endl;
    cout << "Director         : " << director << endl;
    cout << "Runtime (mins)   : " << runtimeMinutes << endl;
    cout << "Age Rating       : " << ageRating << endl;
    cout << "Price            : " << fixed << setprecision(2) << price << endl;
    cout << "Total Copies     : " << totalCopies << endl;
    cout << "Available Copies : " << availableCopies << endl;
    cout << "Times Borrowed   : " << timesBorrowed << endl;
    cout << "Date Added       : " << dateAdded << endl;
    printSeparatorLine('-', 60);
}

string DVD::getResourceType() const { return "DVD"; }

double DVD::calculateLateFee(int daysLate) const {
    if (daysLate <= 0) return 0.0;
    return daysLate * 3.0;
}

int DVD::getAvailableCopies() const { return availableCopies; }
int DVD::getTotalCopies() const { return totalCopies; }

void DVD::issueCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to issue must be positive for DVD \"" + title + "\"");
    }
    if (quantity > availableCopies) {
        throw ResourceUnavailableException("Not enough available copies of \"" + title + "\" to issue " + to_string(quantity));
    }
    availableCopies -= quantity;
    timesBorrowed += quantity;
}

void DVD::issueCopies() {
    issueCopies(1);
}

void DVD::returnCopies(int quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException("Quantity to return must be positive for DVD \"" + title + "\"");
    }
    if (availableCopies + quantity > totalCopies) {
        throw InvalidQuantityException("Returning this many copies exceeds total copies for \"" + title + "\"");
    }
    availableCopies += quantity;
}

void DVD::returnCopies() {
    returnCopies(1);
}

string DVD::getDirector() const { return director; }
int DVD::getRuntimeMinutes() const { return runtimeMinutes; }
string DVD::getAgeRating() const { return ageRating; }
int DVD::getTimesBorrowed() const { return timesBorrowed; }
int DVD::getTotalDVDsCreated() { return totalDVDsCreated; }

class Member {
private:
    int memberID;
    string name;
    string email;
    string membershipType;
    int maxBorrowLimit;
    double finesOwed;
    vector<int> borrowedResourceIDs;
    static int totalMembers;
public:
    Member(int id, const string& name, const string& email, const string& membershipType = "Student");
    int getMemberID() const;
    string getName() const;
    string getEmail() const;
    string getMembershipType() const;
    int getMaxBorrowLimit() const;
    bool canBorrowMore() const;
    double getFinesOwed() const;
    vector<int> getBorrowedResourceIDs() const;
    void addBorrowedResource(int resourceID);
    void removeBorrowedResource(int resourceID);
    void addFine(double amount);
    void payFine(double amount);
    static int getTotalMembers();
};

int Member::totalMembers = 0;

Member::Member(int id, const string& n, const string& e, const string& type)
    : memberID(id), name(n), email(e), finesOwed(0.0) {
    if (id <= 0) {
        throw InvalidResourceIDException("Member ID must be a positive integer, received: " + to_string(id));
    }
    if (n.empty()) {
        throw InvalidFieldException("Member name cannot be empty");
    }
    if (e.empty() || e.find('@') == string::npos) {
        throw InvalidFieldException("Member email must be a valid email address");
    }
    string lowerType = toLowerCaseCopy(type);
    if (lowerType == "student") {
        membershipType = "Student";
        maxBorrowLimit = 5;
    } else if (lowerType == "faculty") {
        membershipType = "Faculty";
        maxBorrowLimit = 10;
    } else if (lowerType == "guest") {
        membershipType = "Guest";
        maxBorrowLimit = 2;
    } else {
        throw InvalidFieldException("Membership type must be one of: Student, Faculty, Guest (received \"" + type + "\")");
    }
    totalMembers++;
}

int Member::getMemberID() const { return memberID; }
string Member::getName() const { return name; }
string Member::getEmail() const { return email; }
string Member::getMembershipType() const { return membershipType; }
int Member::getMaxBorrowLimit() const { return maxBorrowLimit; }
bool Member::canBorrowMore() const { return static_cast<int>(borrowedResourceIDs.size()) < maxBorrowLimit; }
double Member::getFinesOwed() const { return finesOwed; }
vector<int> Member::getBorrowedResourceIDs() const { return borrowedResourceIDs; }

void Member::addBorrowedResource(int resourceID) {
    borrowedResourceIDs.push_back(resourceID);
}

void Member::removeBorrowedResource(int resourceID) {
    auto it = find(borrowedResourceIDs.begin(), borrowedResourceIDs.end(), resourceID);
    if (it != borrowedResourceIDs.end()) {
        borrowedResourceIDs.erase(it);
    }
}

void Member::addFine(double amount) {
    if (amount < 0) {
        throw InvalidPriceException("Fine amount cannot be negative for member " + to_string(memberID));
    }
    finesOwed += amount;
}

void Member::payFine(double amount) {
    if (amount < 0) {
        throw InvalidPriceException("Payment amount cannot be negative");
    }
    if (amount > finesOwed) {
        throw InvalidPriceException("Payment of " + to_string(amount) + " exceeds fines owed of " + to_string(finesOwed));
    }
    finesOwed -= amount;
}

int Member::getTotalMembers() { return totalMembers; }

class Transaction {
private:
    static int nextTransactionID;
    int transactionID;
    int memberID;
    int resourceID;
    string issueDate;
    string dueDate;
    string returnDate;
    bool isReturned;
    double fineAmount;
public:
    Transaction(int memberID, int resourceID, int loanDays);
    int getTransactionID() const;
    int getMemberID() const;
    int getResourceID() const;
    string getIssueDate() const;
    string getDueDate() const;
    string getReturnDate() const;
    bool getIsReturned() const;
    double getFineAmount() const;
    void markReturned(double fine);
    void extendDueDate(int additionalDays);
};

int Transaction::nextTransactionID = 1001;

Transaction::Transaction(int mID, int rID, int loanDays) : memberID(mID), resourceID(rID), isReturned(false), fineAmount(0.0) {
    if (loanDays <= 0) {
        throw InvalidQuantityException("Loan period must be a positive number of days");
    }
    transactionID = nextTransactionID++;
    issueDate = getCurrentDateString();
    dueDate = addDaysToDate(issueDate, loanDays);
    returnDate = "N/A";
}

int Transaction::getTransactionID() const { return transactionID; }
int Transaction::getMemberID() const { return memberID; }
int Transaction::getResourceID() const { return resourceID; }
string Transaction::getIssueDate() const { return issueDate; }
string Transaction::getDueDate() const { return dueDate; }
string Transaction::getReturnDate() const { return returnDate; }
bool Transaction::getIsReturned() const { return isReturned; }
double Transaction::getFineAmount() const { return fineAmount; }

void Transaction::markReturned(double fine) {
    isReturned = true;
    returnDate = getCurrentDateString();
    fineAmount = fine;
}

void Transaction::extendDueDate(int additionalDays) {
    if (additionalDays <= 0) {
        throw InvalidQuantityException("Additional days for loan renewal must be positive");
    }
    dueDate = addDaysToDate(dueDate, additionalDays);
}

class Library {
private:
    vector<Resource*> resources;
    vector<Member> members;
    vector<Transaction> transactions;
    map<int, queue<int>> resourceWaitlists;
    string libraryName;
public:
    Library(const string& name);
    ~Library();
    void addResource(Book* book);
    void addResource(Journal* journal);
    void addResource(DVD* dvd);
    void removeResource(int resourceID);
    Resource* findResourceByID(int resourceID) const;
    vector<Resource*> findResourcesByTitle(const string& keyword) const;
    vector<Resource*> findResourcesByAuthor(const string& author) const;
    vector<Resource*> findResourcesByGenre(const string& genre) const;
    void addMember(int id, const string& name, const string& email, const string& membershipType = "Student");
    Member* findMemberByID(int memberID);
    void displayAllResources() const;
    void displayAllBooks() const;
    void displayAllJournals() const;
    void displayAllDVDs() const;
    void displayResourcesAbovePrice(double threshold) const;
    void sortResourcesByPrice(bool ascending);
    void sortResourcesByTitle();
    double calculateAveragePrice() const;
    double calculateAveragePriceByType(const string& type) const;
    Resource* findMostExpensiveResource() const;
    Resource* findCheapestResource() const;
    void issueResourceToMember(int resourceID, int memberID, int loanDays);
    void returnResourceFromMember(int transactionID, int daysLate);
    void reserveResource(int resourceID, int memberID);
    void cancelReservation(int resourceID, int memberID);
    void displayWaitlist(int resourceID) const;
    void renewLoan(int transactionID, int additionalDays);
    void displayOverdueTransactions() const;
    void displayActiveTransactions() const;
    void displayMemberHistory(int memberID) const;
    void displayMostBorrowedResources(int topN) const;
    void processFinePayment(int memberID, double amount);
    void generateInventoryReport() const;
    void saveInventoryToFile(const string& filename) const;
    void loadInventoryFromFile(const string& filename);
    void compareTwoResources(int id1, int id2) const;
    void displayStatistics() const;
    void applyDiscountToAll(double percent);
    void applyDiscountByType(const string& type, double percent);
    int getResourceCount() const;
    void exportMembersCSV(const string& filename) const;
    void importMembersCSV(const string& filename);
    void exportTransactionsCSV(const string& filename) const;
    void displayLowStock(int threshold) const;
    void exportAnalyticsCSV(const string& filename) const;
};

Library::Library(const string& name) : libraryName(name) {}

Library::~Library() {
    for (Resource* r : resources) {
        delete r;
    }
    resources.clear();
}

void Library::addResource(Book* book) {
    for (Resource* r : resources) {
        if (r->getResourceID() == book->getResourceID()) {
            delete book;
            throw DuplicateResourceException("A resource with ID " + to_string(book->getResourceID()) + " already exists");
        }
    }
    resources.push_back(book);
}

void Library::addResource(Journal* journal) {
    for (Resource* r : resources) {
        if (r->getResourceID() == journal->getResourceID()) {
            delete journal;
            throw DuplicateResourceException("A resource with ID " + to_string(journal->getResourceID()) + " already exists");
        }
    }
    resources.push_back(journal);
}

void Library::addResource(DVD* dvd) {
    for (Resource* r : resources) {
        if (r->getResourceID() == dvd->getResourceID()) {
            delete dvd;
            throw DuplicateResourceException("A resource with ID " + to_string(dvd->getResourceID()) + " already exists");
        }
    }
    resources.push_back(dvd);
}

void Library::removeResource(int resourceID) {
    auto it = find_if(resources.begin(), resources.end(), [resourceID](Resource* r) {
        return r->getResourceID() == resourceID;
    });
    if (it == resources.end()) {
        throw ResourceNotFoundException("No resource found with ID " + to_string(resourceID));
    }
    delete *it;
    resources.erase(it);
}

Resource* Library::findResourceByID(int resourceID) const {
    auto it = find_if(resources.begin(), resources.end(), [resourceID](Resource* r) {
        return r->getResourceID() == resourceID;
    });
    if (it == resources.end()) {
        throw ResourceNotFoundException("No resource found with ID " + to_string(resourceID));
    }
    return *it;
}

vector<Resource*> Library::findResourcesByTitle(const string& keyword) const {
    vector<Resource*> matches;
    string lowerKeyword = toLowerCaseCopy(keyword);
    copy_if(resources.begin(), resources.end(), back_inserter(matches), [&lowerKeyword](Resource* r) {
        return toLowerCaseCopy(r->getTitle()).find(lowerKeyword) != string::npos;
    });
    return matches;
}

vector<Resource*> Library::findResourcesByAuthor(const string& author) const {
    vector<Resource*> matches;
    string lowerAuthor = toLowerCaseCopy(author);
    for (Resource* r : resources) {
        if (r->getResourceType() == "Book") {
            Book* b = dynamic_cast<Book*>(r);
            if (toLowerCaseCopy(b->getAuthor()).find(lowerAuthor) != string::npos) {
                matches.push_back(r);
            }
        }
    }
    return matches;
}

vector<Resource*> Library::findResourcesByGenre(const string& genre) const {
    vector<Resource*> matches;
    string lowerGenre = toLowerCaseCopy(genre);
    for (Resource* r : resources) {
        if (r->getResourceType() == "Book") {
            Book* b = dynamic_cast<Book*>(r);
            if (toLowerCaseCopy(b->getGenre()).find(lowerGenre) != string::npos) {
                matches.push_back(r);
            }
        }
    }
    return matches;
}

void Library::addMember(int id, const string& name, const string& email, const string& membershipType) {
    for (const Member& m : members) {
        if (m.getMemberID() == id) {
            throw DuplicateMemberException("A member with ID " + to_string(id) + " already exists");
        }
    }
    members.push_back(Member(id, name, email, membershipType));
}

Member* Library::findMemberByID(int memberID) {
    for (Member& m : members) {
        if (m.getMemberID() == memberID) {
            return &m;
        }
    }
    throw MemberNotFoundException("No member found with ID " + to_string(memberID));
}

void Library::displayAllResources() const {
    if (resources.empty()) {
        cout << "No resources currently in the library." << endl;
        return;
    }
    cout << left << setw(6) << "ID" << setw(32) << "Title" << setw(12) << "Price"
         << setw(14) << "Type" << setw(12) << "Date Added" << endl;
    printSeparatorLine(76);
    for (Resource* r : resources) {
        cout << *r << endl;
    }
}

void Library::displayAllBooks() const {
    bool any = false;
    cout << left << setw(6) << "ID" << setw(32) << "Title" << setw(20) << "Author" << setw(12) << "Price" << endl;
    printSeparatorLine(70);
    for (Resource* r : resources) {
        if (r->getResourceType() == "Book") {
            Book* b = dynamic_cast<Book*>(r);
            cout << left << setw(6) << b->getResourceID() << setw(32) << b->getTitle()
                 << setw(20) << b->getAuthor() << setw(12) << fixed << setprecision(2) << b->getPrice() << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No books currently in the library." << endl;
    }
}

void Library::displayAllJournals() const {
    bool any = false;
    cout << left << setw(6) << "ID" << setw(32) << "Title" << setw(20) << "Publisher" << setw(12) << "Price" << endl;
    printSeparatorLine(70);
    for (Resource* r : resources) {
        if (r->getResourceType() == "Journal") {
            Journal* j = dynamic_cast<Journal*>(r);
            cout << left << setw(6) << j->getResourceID() << setw(32) << j->getTitle()
                 << setw(20) << j->getPublisher() << setw(12) << fixed << setprecision(2) << j->getPrice() << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No journals currently in the library." << endl;
    }
}

void Library::displayAllDVDs() const {
    bool any = false;
    cout << left << setw(6) << "ID" << setw(32) << "Title" << setw(20) << "Director" << setw(12) << "Price" << endl;
    printSeparatorLine(70);
    for (Resource* r : resources) {
        if (r->getResourceType() == "DVD") {
            DVD* d = dynamic_cast<DVD*>(r);
            cout << left << setw(6) << d->getResourceID() << setw(32) << d->getTitle()
                 << setw(20) << d->getDirector() << setw(12) << fixed << setprecision(2) << d->getPrice() << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No DVDs currently in the library." << endl;
    }
}

void Library::displayResourcesAbovePrice(double threshold) const {
    cout << "Resources priced above " << fixed << setprecision(2) << threshold << ":" << endl;
    bool found = false;
    for_each(resources.begin(), resources.end(), [threshold, &found](Resource* r) {
        if (r->getPrice() > threshold) {
            cout << *r << endl;
            found = true;
        }
    });
    if (!found) {
        cout << "No resources found above this price." << endl;
    }
}

void Library::sortResourcesByPrice(bool ascending) {
    sort(resources.begin(), resources.end(), [ascending](Resource* a, Resource* b) {
        if (ascending) return a->getPrice() < b->getPrice();
        return a->getPrice() > b->getPrice();
    });
}

void Library::sortResourcesByTitle() {
    sort(resources.begin(), resources.end(), [](Resource* a, Resource* b) {
        return toLowerCaseCopy(a->getTitle()) < toLowerCaseCopy(b->getTitle());
    });
}

double Library::calculateAveragePrice() const {
    if (resources.empty()) return 0.0;
    double total = accumulate(resources.begin(), resources.end(), 0.0, [](double sum, Resource* r) {
        return sum + r->getPrice();
    });
    return total / resources.size();
}

double Library::calculateAveragePriceByType(const string& type) const {
    vector<Resource*> filtered;
    copy_if(resources.begin(), resources.end(), back_inserter(filtered), [&type](Resource* r) {
        return r->getResourceType() == type;
    });
    if (filtered.empty()) return 0.0;
    double total = accumulate(filtered.begin(), filtered.end(), 0.0, [](double sum, Resource* r) {
        return sum + r->getPrice();
    });
    return total / filtered.size();
}

Resource* Library::findMostExpensiveResource() const {
    if (resources.empty()) {
        throw ResourceNotFoundException("No resources available to compare");
    }
    return *max_element(resources.begin(), resources.end(), [](Resource* a, Resource* b) {
        return a->getPrice() < b->getPrice();
    });
}

Resource* Library::findCheapestResource() const {
    if (resources.empty()) {
        throw ResourceNotFoundException("No resources available to compare");
    }
    return *min_element(resources.begin(), resources.end(), [](Resource* a, Resource* b) {
        return a->getPrice() < b->getPrice();
    });
}

void Library::issueResourceToMember(int resourceID, int memberID, int loanDays) {
    Resource* r = findResourceByID(resourceID);
    Member* m = findMemberByID(memberID);
    if (!m->canBorrowMore()) {
        throw BorrowLimitExceededException("Member " + m->getName() + " (" + m->getMembershipType() +
            ") has reached the maximum borrowing limit of " + to_string(m->getMaxBorrowLimit()) + " resources");
    }
    r->issueCopies(1);
    Transaction t(memberID, resourceID, loanDays);
    transactions.push_back(t);
    m->addBorrowedResource(resourceID);
    cout << "Resource \"" << r->getTitle() << "\" issued to member " << m->getName()
         << ". Transaction ID: " << t.getTransactionID() << ". Due Date: " << t.getDueDate() << endl;
}

void Library::returnResourceFromMember(int transactionID, int daysLate) {
    auto it = find_if(transactions.begin(), transactions.end(), [transactionID](const Transaction& t) {
        return t.getTransactionID() == transactionID;
    });
    if (it == transactions.end()) {
        throw TransactionNotFoundException("No transaction found with ID " + to_string(transactionID));
    }
    if (it->getIsReturned()) {
        throw InvalidQuantityException("Transaction " + to_string(transactionID) + " has already been marked as returned");
    }
    Resource* r = findResourceByID(it->getResourceID());
    Member* m = findMemberByID(it->getMemberID());
    if (daysLate < 0) throw InvalidQuantityException("Days late cannot be negative");
    double fine = r->calculateLateFee(daysLate);
    it->markReturned(fine);
    r->returnCopies(1);
    m->removeBorrowedResource(it->getResourceID());
    if (fine > 0) {
        m->addFine(fine);
    }
    cout << "Resource \"" << r->getTitle() << "\" returned by member " << m->getName()
         << ". Fine charged: " << fixed << setprecision(2) << fine << endl;
    auto wit = resourceWaitlists.find(it->getResourceID());
    if (wit != resourceWaitlists.end() && !wit->second.empty()) {
        int nextMemberID = wit->second.front();
        wit->second.pop();
        cout << "Notice: Member " << nextMemberID << " is next in line and has been notified that \""
             << r->getTitle() << "\" is now available." << endl;
    }
}

void Library::reserveResource(int resourceID, int memberID) {
    Resource* r = findResourceByID(resourceID);
    Member* m = findMemberByID(memberID);
    if (r->getAvailableCopies() > 0) {
        throw InvalidQuantityException("Resource \"" + r->getTitle() + "\" currently has available copies; no need to reserve");
    }
    queue<int>& q = resourceWaitlists[resourceID];
    queue<int> temp = q;
    while (!temp.empty()) {
        if (temp.front() == memberID) {
            throw AlreadyReservedException("Member " + to_string(memberID) + " has already reserved resource " + to_string(resourceID));
        }
        temp.pop();
    }
    q.push(memberID);
    cout << m->getName() << " has been added to the waitlist for \"" << r->getTitle() << "\". Position: " << q.size() << endl;
}

void Library::cancelReservation(int resourceID, int memberID) {
    auto it = resourceWaitlists.find(resourceID);
    if (it == resourceWaitlists.end() || it->second.empty()) {
        throw ReservationNotFoundException("No waitlist exists for resource " + to_string(resourceID));
    }
    queue<int> newQueue;
    bool found = false;
    while (!it->second.empty()) {
        int mid = it->second.front();
        it->second.pop();
        if (mid == memberID) {
            found = true;
            continue;
        }
        newQueue.push(mid);
    }
    it->second = newQueue;
    if (!found) {
        throw ReservationNotFoundException("Member " + to_string(memberID) + " was not found in the waitlist for resource " + to_string(resourceID));
    }
    cout << "Reservation cancelled for member " << memberID << " on resource " << resourceID << endl;
}

void Library::displayWaitlist(int resourceID) const {
    auto it = resourceWaitlists.find(resourceID);
    if (it == resourceWaitlists.end() || it->second.empty()) {
        cout << "No members are currently on the waitlist for resource " << resourceID << endl;
        return;
    }
    cout << "Waitlist for resource " << resourceID << ":" << endl;
    queue<int> temp = it->second;
    int position = 1;
    while (!temp.empty()) {
        cout << position++ << ". Member ID " << temp.front() << endl;
        temp.pop();
    }
}

void Library::renewLoan(int transactionID, int additionalDays) {
    auto it = find_if(transactions.begin(), transactions.end(), [transactionID](const Transaction& t) {
        return t.getTransactionID() == transactionID;
    });
    if (it == transactions.end()) {
        throw TransactionNotFoundException("No transaction found with ID " + to_string(transactionID));
    }
    if (it->getIsReturned()) {
        throw InvalidQuantityException("Cannot renew transaction " + to_string(transactionID) + " because it has already been returned");
    }
    auto wit = resourceWaitlists.find(it->getResourceID());
    if (wit != resourceWaitlists.end() && !wit->second.empty()) {
        throw ResourceUnavailableException("Cannot renew: another member is waiting for resource " + to_string(it->getResourceID()));
    }
    it->extendDueDate(additionalDays);
    cout << "Loan renewed for transaction " << transactionID << ". New due date: " << it->getDueDate() << endl;
}

void Library::displayOverdueTransactions() const {
    string today = getCurrentDateString();
    bool any = false;
    cout << left << setw(10) << "TransID" << setw(10) << "MemberID" << setw(12) << "ResourceID"
         << setw(14) << "DueDate" << setw(12) << "DaysLate" << setw(12) << "EstFine" << endl;
    printSeparatorLine(70);
    for (const Transaction& t : transactions) {
        if (!t.getIsReturned() && today > t.getDueDate()) {
            int daysLate = daysBetween(t.getDueDate(), today);
            double fine = 0.0;
            try {
                Resource* r = findResourceByID(t.getResourceID());
                fine = r->calculateLateFee(daysLate);
            } catch (LibraryException&) {
                fine = 0.0;
            }
            cout << left << setw(10) << t.getTransactionID() << setw(10) << t.getMemberID()
                 << setw(12) << t.getResourceID() << setw(14) << t.getDueDate()
                 << setw(12) << daysLate << setw(12) << fixed << setprecision(2) << fine << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No overdue transactions at this time." << endl;
    }
}

void Library::displayActiveTransactions() const {
    bool any = false;
    cout << left << setw(10) << "TransID" << setw(10) << "MemberID" << setw(12) << "ResourceID"
         << setw(14) << "IssueDate" << setw(14) << "DueDate" << endl;
    printSeparatorLine(60);
    for (const Transaction& t : transactions) {
        if (!t.getIsReturned()) {
            cout << left << setw(10) << t.getTransactionID() << setw(10) << t.getMemberID()
                 << setw(12) << t.getResourceID() << setw(14) << t.getIssueDate()
                 << setw(14) << t.getDueDate() << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No active transactions at this time." << endl;
    }
}

void Library::displayMemberHistory(int memberID) const {
    bool any = false;
    for (const Transaction& t : transactions) {
        if (t.getMemberID() == memberID) {
            cout << "Transaction " << t.getTransactionID() << ": Resource " << t.getResourceID()
                 << ", Issued " << t.getIssueDate() << ", Due " << t.getDueDate()
                 << ", Returned " << t.getReturnDate() << ", Fine "
                 << fixed << setprecision(2) << t.getFineAmount() << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No transaction history found for member " << memberID << endl;
    }
}

void Library::displayMostBorrowedResources(int topN) const {
    if (resources.empty()) {
        cout << "No resources available." << endl;
        return;
    }
    if (topN <= 0) {
        throw InvalidQuantityException("Number of top resources to display must be positive");
    }
    vector<Resource*> sorted = resources;
    sort(sorted.begin(), sorted.end(), [](Resource* a, Resource* b) {
        return a->getTimesBorrowed() > b->getTimesBorrowed();
    });
    int count = min(topN, static_cast<int>(sorted.size()));
    cout << "Top " << count << " Most Borrowed Resources:" << endl;
    printSeparatorLine(60);
    for (int i = 0; i < count; i++) {
        cout << (i + 1) << ". " << sorted[i]->getTitle() << " (" << sorted[i]->getResourceType()
             << ") - Borrowed " << sorted[i]->getTimesBorrowed() << " time(s)" << endl;
    }
}

void Library::processFinePayment(int memberID, double amount) {
    Member* m = findMemberByID(memberID);
    m->payFine(amount);
    cout << "Payment of " << fixed << setprecision(2) << amount << " received from " << m->getName()
         << ". Remaining fines owed: " << fixed << setprecision(2) << m->getFinesOwed() << endl;
}

void Library::generateInventoryReport() const {
    printSeparatorLine('=', 76);
    cout << libraryName << " - Inventory Report" << endl;
    printSeparatorLine('=', 76);
    displayAllResources();
    printSeparatorLine(76);
    cout << "Total Resources         : " << Resource::getTotalResources() << endl;
    cout << "Total Books             : " << Book::getTotalBooksCreated() << endl;
    cout << "Total Journals          : " << Journal::getTotalJournalsCreated() << endl;
    cout << "Total DVDs              : " << DVD::getTotalDVDsCreated() << endl;
    cout << "Total Members           : " << Member::getTotalMembers() << endl;
    cout << "Total Inventory Value   : " << fixed << setprecision(2) << Resource::getTotalInventoryValue() << endl;
    cout << "Average Price (Overall) : " << fixed << setprecision(2) << calculateAveragePrice() << endl;
    cout << "Average Price (Books)   : " << fixed << setprecision(2) << calculateAveragePriceByType("Book") << endl;
    cout << "Average Price (Journals): " << fixed << setprecision(2) << calculateAveragePriceByType("Journal") << endl;
    cout << "Average Price (DVDs)    : " << fixed << setprecision(2) << calculateAveragePriceByType("DVD") << endl;
    printSeparatorLine('=', 76);
}

void Library::saveInventoryToFile(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw runtime_error("Could not open file " + filename + " for writing");
    }
    outFile << "ResourceID,Type,Title,Price,DateAdded,Author,ISBN,Genre,Language,Format,PubYear,"
            << "Copies,AvailCopies,TimesBorrowed,ISSN,Publisher,Volume,IssueNumber,Frequency,"
            << "ImpactFactor,IndexDB,Director,Runtime,AgeRating\n";
    for (Resource* r : resources) {
        outFile << r->getResourceID() << "," << r->getResourceType() << "," << r->getTitle() << ","
                << fixed << setprecision(2) << r->getPrice() << "," << r->getDateAdded() << ",";
        if (r->getResourceType() == "Book") {
            Book* b = dynamic_cast<Book*>(r);
            outFile << b->getAuthor() << "," << b->getISBN() << "," << b->getGenre() << ","
                    << b->getLanguage() << "," << b->getFormat() << "," << b->getPublicationYear() << ","
                    << b->getTotalCopies() << "," << b->getAvailableCopies() << "," << b->getTimesBorrowed()
                    << ",,,,,,,,,\n";
        } else if (r->getResourceType() == "Journal") {
            Journal* j = dynamic_cast<Journal*>(r);
            outFile << ",,,,," << j->getTotalCopies() << "," << j->getAvailableCopies() << ","
                    << j->getTimesBorrowed() << "," << j->getISSN() << "," << j->getPublisher() << ","
                    << j->getVolume() << "," << j->getIssueNumber() << "," << j->getFrequency() << ","
                    << fixed << setprecision(2) << j->getImpactFactor() << "," << j->getIndexingDatabase()
                    << ",,,\n";
        } else if (r->getResourceType() == "DVD") {
            DVD* d = dynamic_cast<DVD*>(r);
            outFile << ",,,,," << d->getTotalCopies() << "," << d->getAvailableCopies() << ","
                    << d->getTimesBorrowed() << ",,,,,,,," << d->getDirector() << ","
                    << d->getRuntimeMinutes() << "," << d->getAgeRating() << "\n";
        }
    }
    outFile.close();
    cout << "Inventory successfully saved to " << filename << " (" << resources.size() << " resource(s))" << endl;
}

void Library::loadInventoryFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw runtime_error("Could not open file " + filename + " for reading");
    }
    string line;
    getline(inFile, line);
    int loadedCount = 0;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> fields;
        string field;
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        while (fields.size() < 24) fields.push_back("");
        try {
            int id = stoi(fields[0]);
            string type = fields[1];
            string title = fields[2];
            double price = stod(fields[3]);
            if (type == "Book") {
                string author = fields[5];
                string isbn = fields[6];
                string genre = fields[7];
                string language = fields[8].empty() ? "English" : fields[8];
                string format = fields[9].empty() ? "Paperback" : fields[9];
                int year = stoi(fields[10]);
                int copies = stoi(fields[11]);
                addResource(new Book(id, title, price, author, isbn, genre, year, copies, language, format));
                loadedCount++;
            } else if (type == "Journal") {
                string issn = fields[14];
                string publisher = fields[15];
                int volume = stoi(fields[16]);
                int issueNumber = stoi(fields[17]);
                string frequency = fields[18];
                int copies = stoi(fields[11]);
                double impact = fields[19].empty() ? 0.0 : stod(fields[19]);
                string indexDb = fields[20].empty() ? "Scopus" : fields[20];
                addResource(new Journal(id, title, price, issn, publisher, volume, issueNumber, frequency, copies, impact, indexDb));
                loadedCount++;
            } else if (type == "DVD") {
                string director = fields[21];
                int runtime = stoi(fields[22]);
                string ageRating = fields[23];
                int copies = stoi(fields[11]);
                addResource(new DVD(id, title, price, director, runtime, ageRating, copies));
                loadedCount++;
            } else {
                cout << "Skipped row with unrecognized resource type: " << type << endl;
            }
        } catch (LibraryException& ex) {
            cout << "Skipped a row while loading: " << ex.what() << endl;
        } catch (...) {
            cout << "Skipped a malformed row in the inventory file." << endl;
        }
    }
    inFile.close();
    cout << loadedCount << " resource(s) loaded successfully from " << filename << endl;
}

void Library::compareTwoResources(int id1, int id2) const {
    Resource* r1 = nullptr;
    Resource* r2 = nullptr;
    for (Resource* r : resources) {
        if (r->getResourceID() == id1) r1 = r;
        if (r->getResourceID() == id2) r2 = r;
    }
    if (r1 == nullptr || r2 == nullptr) {
        throw ResourceNotFoundException("One or both resource IDs were not found for comparison");
    }
    cout << "Comparing \"" << r1->getTitle() << "\" (" << fixed << setprecision(2) << r1->getPrice()
         << ") and \"" << r2->getTitle() << "\" (" << r2->getPrice() << ")" << endl;
    if (compareResourcePrices(*r1, *r2)) {
        cout << "\"" << r1->getTitle() << "\" is more expensive." << endl;
    } else if (compareResourcePrices(*r2, *r1)) {
        cout << "\"" << r2->getTitle() << "\" is more expensive." << endl;
    } else {
        cout << "Both resources are priced equally." << endl;
    }
    cout << "Price difference: " << fixed << setprecision(2) << priceDifference(*r1, *r2) << endl;
}

void Library::displayStatistics() const {
    cout << "Library Name             : " << libraryName << endl;
    cout << "Total Resources          : " << Resource::getTotalResources() << endl;
    cout << "Total Books              : " << Book::getTotalBooksCreated() << endl;
    cout << "Total Journals           : " << Journal::getTotalJournalsCreated() << endl;
    cout << "Total DVDs               : " << DVD::getTotalDVDsCreated() << endl;
    cout << "Total Members            : " << Member::getTotalMembers() << endl;
    cout << "Total Inventory Value    : " << fixed << setprecision(2) << Resource::getTotalInventoryValue() << endl;
    if (!resources.empty()) {
        cout << "Average Resource Price   : " << fixed << setprecision(2) << calculateAveragePrice() << endl;
        cout << "Most Expensive Resource  : " << findMostExpensiveResource()->getTitle() << endl;
        cout << "Cheapest Resource        : " << findCheapestResource()->getTitle() << endl;
    }
}

void Library::applyDiscountToAll(double percent) {
    if (percent < 0 || percent > 100) {
        throw InvalidPriceException("Discount percentage must be between 0 and 100");
    }
    for_each(resources.begin(), resources.end(), [percent](Resource* r) {
        double newPrice = r->getPrice() * (1.0 - percent / 100.0);
        r->setPrice(newPrice);
    });
}

void Library::applyDiscountByType(const string& type, double percent) {
    if (percent < 0 || percent > 100) {
        throw InvalidPriceException("Discount percentage must be between 0 and 100");
    }
    for_each(resources.begin(), resources.end(), [percent, &type](Resource* r) {
        if (r->getResourceType() == type) {
            double newPrice = r->getPrice() * (1.0 - percent / 100.0);
            r->setPrice(newPrice);
        }
    });
}

int Library::getResourceCount() const { return static_cast<int>(resources.size()); }



void Library::importMembersCSV(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("Could not open " + filename);
    string line;
    getline(in, line);
    int loaded = 0, skipped = 0;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> f;
        string value;
        while (getline(ss, value, ',')) {
            if (!value.empty() && value.back() == '\r') value.pop_back();
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                value = value.substr(1, value.size() - 2);
            f.push_back(value);
        }
        try {
            if (f.size() < 4) throw InvalidFieldException("Expected MemberID,Name,Email,MembershipType");
            addMember(stoi(f[0]), f[1], f[2], f[3]);
            loaded++;
        } catch (exception&) {
            skipped++;
        }
    }
    cout << loaded << " member(s) imported; " << skipped << " row(s) skipped from " << filename << "\n";
}

void Library::exportMembersCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) throw runtime_error("Could not open " + filename);
    out << "MemberID,Name,Email,MembershipType,MaxBorrowLimit,FinesOwed,ActiveBorrowCount\n";
    for (const Member& m : members)
        out << m.getMemberID() << ",\"" << m.getName() << "\",\"" << m.getEmail() << "\","
            << m.getMembershipType() << "," << m.getMaxBorrowLimit() << ","
            << fixed << setprecision(2) << m.getFinesOwed() << ","
            << m.getBorrowedResourceIDs().size() << "\n";
    cout << "Members exported to " << filename << "\n";
}

void Library::exportTransactionsCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) throw runtime_error("Could not open " + filename);
    out << "TransactionID,MemberID,ResourceID,IssueDate,DueDate,ReturnDate,Returned,FineAmount\n";
    for (const Transaction& t : transactions)
        out << t.getTransactionID() << "," << t.getMemberID() << "," << t.getResourceID() << ","
            << t.getIssueDate() << "," << t.getDueDate() << "," << t.getReturnDate() << ","
            << (t.getIsReturned() ? "true" : "false") << "," << fixed << setprecision(2)
            << t.getFineAmount() << "\n";
    cout << "Transactions exported to " << filename << "\n";
}

void Library::displayLowStock(int threshold) const {
    if (threshold < 0) throw InvalidQuantityException("Stock threshold cannot be negative");
    bool found = false;
    cout << left << setw(8) << "ID" << setw(34) << "Title" << setw(14) << "Type"
         << setw(12) << "Available" << setw(10) << "Total" << "\n";
    printSeparatorLine(78);
    for (Resource* r : resources) {
        if (r->getAvailableCopies() <= threshold) {
            cout << left << setw(8) << r->getResourceID() << setw(34) << r->getTitle()
                 << setw(14) << r->getResourceType() << setw(12) << r->getAvailableCopies()
                 << setw(10) << r->getTotalCopies() << "\n";
            found = true;
        }
    }
    if (!found) cout << "No resources at or below the selected stock threshold.\n";
}

void Library::exportAnalyticsCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) throw runtime_error("Could not open " + filename);
    out << "ResourceID,Title,Type,Price,TotalCopies,AvailableCopies,BorrowedCopies,TimesBorrowed,StockStatus\n";
    for (Resource* r : resources) {
        int borrowed = r->getTotalCopies() - r->getAvailableCopies();
        string status = r->getAvailableCopies() == 0 ? "OUT_OF_STOCK" :
                        r->getAvailableCopies() <= 2 ? "LOW_STOCK" : "AVAILABLE";
        string title = r->getTitle();
        replace(title.begin(), title.end(), '"', '\'');
        out << r->getResourceID() << ",\"" << title << "\"," << r->getResourceType() << ","
            << fixed << setprecision(2) << r->getPrice() << "," << r->getTotalCopies() << ","
            << r->getAvailableCopies() << "," << borrowed << "," << r->getTimesBorrowed()
            << "," << status << "\n";
    }
    cout << "Inventory analytics exported to " << filename << "\n";
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter an integer." << endl;
        } else {
            cin.ignore(10000, '\n');
            return value;
        }
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
        } else {
            cin.ignore(10000, '\n');
            return value;
        }
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void displayMenu() {
    printSeparatorLine('=', 55);
    cout << "   DIGITAL LIBRARY RESOURCE MANAGEMENT SYSTEM" << endl;
    printSeparatorLine('=', 55);
    cout << " 1. Add New Book" << endl;
    cout << " 2. Add New Journal" << endl;
    cout << " 3. Display All Resources" << endl;
    cout << " 4. Display All Books" << endl;
    cout << " 5. Display All Journals" << endl;
    cout << " 6. Search Resource by Title" << endl;
    cout << " 7. Display Resources Above Price" << endl;
    cout << " 8. Compare Two Resources by Price" << endl;
    cout << " 9. Sort Resources by Price" << endl;
    cout << "10. Sort Resources by Title" << endl;
    cout << "11. Add New Member" << endl;
    cout << "12. Issue Resource to Member" << endl;
    cout << "13. Return Resource" << endl;
    cout << "14. Display Member Borrowing History" << endl;
    cout << "15. Display Active Transactions" << endl;
    cout << "16. Display Library Statistics" << endl;
    cout << "17. Generate Inventory Report" << endl;
    cout << "18. Save Inventory to File" << endl;
    cout << "19. Remove Resource" << endl;
    cout << "20. Apply Discount to All Resources" << endl;
    cout << "21. Apply Discount by Resource Type" << endl;
    cout << "22. Add New DVD" << endl;
    cout << "23. Display All DVDs" << endl;
    cout << "24. Search Resources by Author" << endl;
    cout << "25. Search Resources by Genre" << endl;
    cout << "26. Reserve a Resource (Join Waitlist)" << endl;
    cout << "27. Cancel a Reservation" << endl;
    cout << "28. Display Waitlist for a Resource" << endl;
    cout << "29. Renew a Loan" << endl;
    cout << "30. Display Overdue Transactions" << endl;
    cout << "31. Display Most Borrowed Resources" << endl;
    cout << "32. Pay Member Fine" << endl;
    cout << "33. Load Inventory from File" << endl;
    cout << "34. Export Members to CSV" << endl;
    cout << "35. Export Transactions to CSV" << endl;
    cout << "36. Low-Stock Alert Report" << endl;
    cout << "37. Export Inventory Analytics CSV" << endl;
    cout << "38. Import Members from CSV" << endl;
    cout << " 0. Exit" << endl;
    printSeparatorLine('=', 55);
}

void loadSampleData(Library& library) {
    try {
        library.addResource(new Book(101, "The Pragmatic Programmer", 850.00, "Andrew Hunt",
                                      "978-0135957059", "Technology", 2019, 5, "English", "Paperback"));
        library.addResource(new Book(102, "Introduction to Algorithms", 1200.50, "Thomas Cormen",
                                      "978-0262046305", "Computer Science", 2022, 3, "English", "Hardcover"));
        library.addResource(new Book(103, "Clean Code", 650.00, "Robert Martin",
                                      "978-0132350884", "Technology", 2008, 4, "English", "Paperback"));
        library.addResource(new Book(104, "Design Patterns", 720.75, "Erich Gamma",
                                      "978-0201633610", "Computer Science", 1994, 2, "English", "Hardcover"));
        library.addResource(new Journal(201, "Nature", 300.00, "0028-0836", "Springer Nature",
                                         45, 12, "Weekly", 6, 49.9, "Scopus"));
        library.addResource(new Journal(202, "IEEE Transactions on AI", 450.75, "2691-4581", "IEEE",
                                         5, 3, "Quarterly", 4, 8.2, "IEEE Xplore"));
        library.addResource(new Journal(203, "Journal of Data Science", 275.25, "1234-5678", "Elsevier",
                                         12, 8, "Monthly", 5, 3.6, "Scopus"));
        library.addResource(new DVD(301, "The Social Network", 199.99, "David Fincher", 120, "PG-13", 3));
        library.addResource(new DVD(302, "Inception", 249.50, "Christopher Nolan", 148, "PG-13", 2));
        library.addMember(1, "Arpit Saharan", "arpit@vitstudent.ac.in", "Student");
        library.addMember(2, "Sarthak Kamra", "sarthak@vitstudent.ac.in", "Student");
        library.addMember(3, "Priya Sharma", "priya@vitstudent.ac.in", "Faculty");
        library.addMember(4, "Rohan Verma", "rohan@vitstudent.ac.in", "Guest");
    } catch (LibraryException& ex) {
        cout << "Error while loading sample data: " << ex.what() << endl;
    }
}

void demonstrateExceptionHandling() {
    printSeparatorLine('*', 55);
    cout << "SCENARIO 2: EXCEPTION HANDLING DEMONSTRATION" << endl;
    printSeparatorLine('*', 55);
    try {
        Book invalidBook(999, "Invalid Price Book", -500.00, "Unknown Author",
                          "000-0000000000", "Fiction", 2020, 2);
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        Journal invalidJournal(998, "Invalid Copies Journal", 100.00, "1111-2222",
                                "Test Publisher", 1, 1, "Monthly", -3);
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        Book invalidYearBook(997, "Time Traveler Book", 100.00, "Someone",
                              "222-333", "Sci-fi", 3050, 2);
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        Journal invalidImpactJournal(996, "Bad Impact Journal", 50.00, "3333-4444",
                                      "Test Press", 1, 1, "Monthly", 2, -5.0);
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        Member invalidEmailMember(995, "Test User", "bad-email-address");
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        Member invalidTypeMember(994, "Test User Two", "test2@example.com", "SuperUser");
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    try {
        DVD invalidDVD(993, "Untitled Footage", 50.00, "", 90, "R", 2);
    } catch (LibraryException& ex) {
        cout << "Caught exception: " << ex.what() << endl;
    }
    printSeparatorLine('*', 55);
}

int main() {
    cout << "Welcome to the Digital Library Resource Management System" << endl;
    Library library("VIT Central Digital Library");

    printSeparatorLine('*', 55);
    cout << "SCENARIO 1: NORMAL EXECUTION - LOADING SAMPLE DATA" << endl;
    printSeparatorLine('*', 55);
    ifstream resourceCheck("resources.csv");
    ifstream memberCheck("members.csv");
    if (resourceCheck.good() && memberCheck.good()) {
        try {
            library.loadInventoryFromFile("resources.csv");
            library.importMembersCSV("members.csv");
        } catch (exception& ex) {
            cout << "CSV startup loading failed: " << ex.what() << endl;
        }
    } else {
        loadSampleData(library);
        cout << "Tip: place resources.csv and members.csv beside the executable to load CSV data." << endl;
    }

    demonstrateExceptionHandling();

    cout << endl;
    library.displayAllResources();
    cout << endl;

    int choice;
    do {
        displayMenu();
        choice = readInt("Enter your choice: ");
        try {
            switch (choice) {
                case 1: {
                    int id = readInt("Enter Resource ID: ");
                    string title = readLine("Enter Title: ");
                    double price = readDouble("Enter Price: ");
                    string author = readLine("Enter Author: ");
                    string isbn = readLine("Enter ISBN: ");
                    string genre = readLine("Enter Genre: ");
                    int year = readInt("Enter Publication Year: ");
                    int copies = readInt("Enter Number of Copies: ");
                    library.addResource(new Book(id, title, price, author, isbn, genre, year, copies));
                    cout << "Book added successfully." << endl;
                    break;
                }
                case 2: {
                    int id = readInt("Enter Resource ID: ");
                    string title = readLine("Enter Title: ");
                    double price = readDouble("Enter Price: ");
                    string issn = readLine("Enter ISSN: ");
                    string publisher = readLine("Enter Publisher: ");
                    int volume = readInt("Enter Volume: ");
                    int issueNumber = readInt("Enter Issue Number: ");
                    string frequency = readLine("Enter Frequency: ");
                    int copies = readInt("Enter Number of Copies: ");
                    library.addResource(new Journal(id, title, price, issn, publisher, volume, issueNumber, frequency, copies));
                    cout << "Journal added successfully." << endl;
                    break;
                }
                case 3:
                    library.displayAllResources();
                    break;
                case 4:
                    library.displayAllBooks();
                    break;
                case 5:
                    library.displayAllJournals();
                    break;
                case 6: {
                    string keyword = readLine("Enter title keyword to search: ");
                    vector<Resource*> results = library.findResourcesByTitle(keyword);
                    if (results.empty()) {
                        cout << "No matching resources found." << endl;
                    } else {
                        for (Resource* r : results) {
                            cout << *r << endl;
                        }
                    }
                    break;
                }
                case 7: {
                    double threshold = readDouble("Enter price threshold: ");
                    library.displayResourcesAbovePrice(threshold);
                    break;
                }
                case 8: {
                    int id1 = readInt("Enter first Resource ID: ");
                    int id2 = readInt("Enter second Resource ID: ");
                    library.compareTwoResources(id1, id2);
                    break;
                }
                case 9: {
                    int order = readInt("Sort ascending (1) or descending (2): ");
                    library.sortResourcesByPrice(order == 1);
                    cout << "Resources sorted by price." << endl;
                    library.displayAllResources();
                    break;
                }
                case 10:
                    library.sortResourcesByTitle();
                    cout << "Resources sorted by title." << endl;
                    library.displayAllResources();
                    break;
                case 11: {
                    int id = readInt("Enter Member ID: ");
                    string name = readLine("Enter Member Name: ");
                    string email = readLine("Enter Member Email: ");
                    string type = readLine("Enter Membership Type (Student/Faculty/Guest) [default: Student]: ");
                    if (type.empty()) type = "Student";
                    library.addMember(id, name, email, type);
                    cout << "Member added successfully." << endl;
                    break;
                }
                case 12: {
                    int resourceID = readInt("Enter Resource ID: ");
                    int memberID = readInt("Enter Member ID: ");
                    int loanDays = readInt("Enter Loan Period (days): ");
                    library.issueResourceToMember(resourceID, memberID, loanDays);
                    break;
                }
                case 13: {
                    int transactionID = readInt("Enter Transaction ID: ");
                    int daysLate = readInt("Enter Days Late (0 if on time): ");
                    library.returnResourceFromMember(transactionID, daysLate);
                    break;
                }
                case 14: {
                    int memberID = readInt("Enter Member ID: ");
                    library.displayMemberHistory(memberID);
                    break;
                }
                case 15:
                    library.displayActiveTransactions();
                    break;
                case 16:
                    library.displayStatistics();
                    break;
                case 17:
                    library.generateInventoryReport();
                    break;
                case 18: {
                    string filename = readLine("Enter filename to save (e.g. inventory.txt): ");
                    library.saveInventoryToFile(filename);
                    break;
                }
                case 19: {
                    int id = readInt("Enter Resource ID to remove: ");
                    library.removeResource(id);
                    cout << "Resource removed successfully." << endl;
                    break;
                }
                case 20: {
                    double percent = readDouble("Enter discount percentage: ");
                    library.applyDiscountToAll(percent);
                    cout << "Discount applied to all resources." << endl;
                    break;
                }
                case 21: {
                    string type = readLine("Enter Resource Type (Book/Journal/DVD): ");
                    double percent = readDouble("Enter discount percentage: ");
                    library.applyDiscountByType(type, percent);
                    cout << "Discount applied to all resources of type " << type << "." << endl;
                    break;
                }
                case 22: {
                    int id = readInt("Enter Resource ID: ");
                    string title = readLine("Enter Title: ");
                    double price = readDouble("Enter Price: ");
                    string director = readLine("Enter Director: ");
                    int runtime = readInt("Enter Runtime (minutes): ");
                    string ageRating = readLine("Enter Age Rating (e.g. PG-13): ");
                    int copies = readInt("Enter Number of Copies: ");
                    library.addResource(new DVD(id, title, price, director, runtime, ageRating, copies));
                    cout << "DVD added successfully." << endl;
                    break;
                }
                case 23:
                    library.displayAllDVDs();
                    break;
                case 24: {
                    string author = readLine("Enter author keyword to search: ");
                    vector<Resource*> results = library.findResourcesByAuthor(author);
                    if (results.empty()) {
                        cout << "No matching books found for that author." << endl;
                    } else {
                        for (Resource* r : results) {
                            cout << *r << endl;
                        }
                    }
                    break;
                }
                case 25: {
                    string genre = readLine("Enter genre keyword to search: ");
                    vector<Resource*> results = library.findResourcesByGenre(genre);
                    if (results.empty()) {
                        cout << "No matching books found for that genre." << endl;
                    } else {
                        for (Resource* r : results) {
                            cout << *r << endl;
                        }
                    }
                    break;
                }
                case 26: {
                    int resourceID = readInt("Enter Resource ID: ");
                    int memberID = readInt("Enter Member ID: ");
                    library.reserveResource(resourceID, memberID);
                    break;
                }
                case 27: {
                    int resourceID = readInt("Enter Resource ID: ");
                    int memberID = readInt("Enter Member ID: ");
                    library.cancelReservation(resourceID, memberID);
                    break;
                }
                case 28: {
                    int resourceID = readInt("Enter Resource ID: ");
                    library.displayWaitlist(resourceID);
                    break;
                }
                case 29: {
                    int transactionID = readInt("Enter Transaction ID: ");
                    int additionalDays = readInt("Enter number of additional days: ");
                    library.renewLoan(transactionID, additionalDays);
                    break;
                }
                case 30:
                    library.displayOverdueTransactions();
                    break;
                case 31: {
                    int topN = readInt("Enter number of top resources to display: ");
                    library.displayMostBorrowedResources(topN);
                    break;
                }
                case 32: {
                    int memberID = readInt("Enter Member ID: ");
                    double amount = readDouble("Enter payment amount: ");
                    library.processFinePayment(memberID, amount);
                    break;
                }
                case 33: {
                    string filename = readLine("Enter filename to load from (e.g. inventory.txt): ");
                    library.loadInventoryFromFile(filename);
                    break;
                }
                case 34: {
                    string filename = readLine("CSV filename [members.csv]: ");
                    library.exportMembersCSV(filename.empty() ? "members.csv" : filename);
                    break;
                }
                case 35: {
                    string filename = readLine("CSV filename [transactions.csv]: ");
                    library.exportTransactionsCSV(filename.empty() ? "transactions.csv" : filename);
                    break;
                }
                case 36:
                    library.displayLowStock(readInt("Show resources with available copies <= "));
                    break;
                case 37: {
                    string filename = readLine("CSV filename [inventory_analytics.csv]: ");
                    library.exportAnalyticsCSV(filename.empty() ? "inventory_analytics.csv" : filename);
                    break;
                }
                case 38: {
                    string filename = readLine("Members CSV filename [members.csv]: ");
                    library.importMembersCSV(filename.empty() ? "members.csv" : filename);
                    break;
                }
                case 0:
                    cout << "Thank you for using the Digital Library Resource Management System." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } catch (LibraryException& ex) {
            cout << "Error: " << ex.what() << endl;
        } catch (exception& ex) {
            cout << "Unexpected error: " << ex.what() << endl;
        }
        cout << endl;
    } while (choice != 0);

    return 0;
}
