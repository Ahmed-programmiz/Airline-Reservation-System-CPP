//Ahmed Rajab
//24i-5604
//BS FT B

#include <iostream>
#include <string>
using namespace std;


class Traveler;
class Aircraft;
class Trip;
class Reservation;

// "UTILITY FUNCTIONS" 
int parseDigits(const string& s) {
    int out = 0;
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            out = out * 10 + (s[i] - '0');
        }
    }
    return out;
}

string toText(int value) {
    if (value == 0) return "0";
    bool neg = false;
    if (value < 0) {
        neg = true;
        value = -value;
    }
    string res = "";
    while (value > 0) {
        res = char('0' + (value % 10)) + res;
        value /= 10;
    }
    if (neg) res = "-" + res;
    return res;
}
// ==================== SEATSPOT CLASS ====================
class SeatSpot {
public:
    string id;
    int r;
    char c;
    bool occupied;

    SeatSpot() : r(0), c('A'), occupied(false) {}
    SeatSpot(int row, char col) : r(row), c(col), occupied(false) {
        id = toText(row) + col;
    }
};
// ==================== Aircraft CLASS ====================
class Aircraft {
public:
    string aircraftTag;
    string typeName;
    int capacityTotal;
    SeatSpot** layout;
    int numRows;
    int numCols;

    Aircraft() : aircraftTag(""), typeName(""), capacityTotal(60), layout(nullptr), numRows(10), numCols(6) {}

    Aircraft(string tag, string type) : aircraftTag(tag), typeName(type), capacityTotal(60), numRows(10), numCols(6) {
        layout = new SeatSpot*[numRows];
        for (int i = 0; i < numRows; ++i) {
            layout[i] = new SeatSpot[numCols];
            for (int j = 0; j < numCols; ++j) {
                layout[i][j] = SeatSpot(i + 1, char('A' + j));
            }
        }
    }

    ~Aircraft() {
        if (layout != nullptr) {
            for (int i = 0; i < numRows; ++i) {
                delete[] layout[i];
            }
            delete[] layout;
        }
    }

    void showLayout() {
        cout << "\n=== SEAT MAP ===" << endl;
        cout << "   ";
        for (int j = 0; j < numCols; ++j) cout << " " << char('A' + j) << " ";
        cout << endl;

        for (int i = 0; i < numRows; ++i) {
            cout << (i + 1 < 10 ? " " : "") << (i + 1) << " ";
            for (int j = 0; j < numCols; ++j) {
                if (layout[i][j].occupied) cout << "[X]";
                else cout << "[ ]";
            }
            cout << endl;
        }
        cout << "\n[X] = Booked  [ ] = Available" << endl;
    }

    bool reserveSpot(const string& spotId) {
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (layout[i][j].id == spotId && !layout[i][j].occupied) {
                    layout[i][j].occupied = true;
                    return true;
                }
            }
        }
        return false;
    }

    bool releaseSpot(const string& spotId) {
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (layout[i][j].id == spotId && layout[i][j].occupied) {
                    layout[i][j].occupied = false;
                    return true;
                }
            }
        }
        return false;
    }

    int countAvailable() {
        int cnt = 0;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (!layout[i][j].occupied) ++cnt;
            }
        }
        return cnt;
    }
};

// ==================== Reservation CLASS ====================

class Reservation {
public:
    string resCode;
    string travelerCode;
    string tripCode;
    string seatSpot;
    string resDate;
    Reservation* next;

    Reservation() : resCode(""), travelerCode(""), tripCode(""), seatSpot(""), resDate(""), next(nullptr) {}

    Reservation(string rc, string tc, string tnum, string ss, string d)
        : resCode(rc), travelerCode(tc), tripCode(tnum), seatSpot(ss), resDate(d), next(nullptr) {}

    void showDetails() {
        cout << "Booking ID: " << resCode << " | Flight: " << tripCode
             << " | Seat: " << seatSpot << " | Date: " << resDate << endl;
    }
};

// ==================== Traveller CLASS ====================

class Traveler {
public:
    string fullName;
    string idTag;
    string userTag;
    string pass;
    string phone;
    Reservation* history;

    Traveler() : fullName(""), idTag(""), userTag(""), pass(""), phone(""), history(nullptr) {}

    Traveler(string n, string id, string u, string p, string ph)
        : fullName(n), idTag(id), userTag(u), pass(p), phone(ph), history(nullptr) {}

    void appendReservation(Reservation* r) {
        if (history == nullptr) history = r;
        else {
            Reservation* t = history;
            while (t->next != nullptr) t = t->next;
            t->next = r;
        }
    }

    void listReservations() {
        if (history == nullptr) {
            cout << "No bookings found." << endl;
            return;
        }
        Reservation* t = history;
        while (t != nullptr) {
            t->showDetails();
            t = t->next;
        }
    }

    bool eraseReservation(const string& code) {
        if (history == nullptr) return false;
        if (history->resCode == code) {
            Reservation* toRem = history;
            history = history->next;
            delete toRem;
            return true;
        }
        Reservation* t = history;
        while (t->next != nullptr) {
            if (t->next->resCode == code) {
                Reservation* toRem = t->next;
                t->next = t->next->next;
                delete toRem;
                return true;
            }
            t = t->next;
        }
        return false;
    }
};

// ==================== Traveller Binary Search Tree ====================

class TravelerNode {
public:
    Traveler* traveler;
    TravelerNode* left;
    TravelerNode* right;
    TravelerNode(Traveler* t) : traveler(t), left(nullptr), right(nullptr) {}
};

class TravelerIndex {
private:
    TravelerNode* root;

    TravelerNode* addHelper(TravelerNode* node, Traveler* t) {
        if (node == nullptr) return new TravelerNode(t);
        if (t->userTag < node->traveler->userTag) node->left = addHelper(node->left, t);
        else if (t->userTag > node->traveler->userTag) node->right = addHelper(node->right, t);
        return node;
    }

    Traveler* findHelper(TravelerNode* node, const string& user) {
        if (node == nullptr) return nullptr;
        if (user == node->traveler->userTag) return node->traveler;
        if (user < node->traveler->userTag) return findHelper(node->left, user);
        return findHelper(node->right, user);
    }

    void inOrder(TravelerNode* node) {
        if (node) {
            inOrder(node->left);
            cout << "Username: " << node->traveler->userTag << " | Name: " << node->traveler->fullName
                 << " | ID: " << node->traveler->idTag << endl;
            inOrder(node->right);
        }
    }

    void preOrder(TravelerNode* node) {
        if (node) {
            cout << "Username: " << node->traveler->userTag << " | Name: " << node->traveler->fullName
                 << " | ID: " << node->traveler->idTag << endl;
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void postOrder(TravelerNode* node) {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << "Username: " << node->traveler->userTag << " | Name: " << node->traveler->fullName
                 << " | ID: " << node->traveler->idTag << endl;
        }
    }

public:
    TravelerIndex() : root(nullptr) {}

    void insertTraveler(Traveler* t) { root = addHelper(root, t); }
    Traveler* searchTraveler(const string& user) { return findHelper(root, user); }
    void printInOrder() {
        if (root == nullptr) { cout << "No passengers registered." << endl; return; }
        inOrder(root);
    }
    void printPreOrder() {
        if (root == nullptr) { cout << "No passengers registered." << endl; return; }
        preOrder(root);
    }
    void printPostOrder() {
        if (root == nullptr) { cout << "No passengers registered." << endl; return; }
        postOrder(root);
    }

    TravelerNode* removeNode(TravelerNode* node, const string& user) {
        if (node == nullptr) return nullptr;
        if (user < node->traveler->userTag) node->left = removeNode(node->left, user);
        else if (user > node->traveler->userTag) node->right = removeNode(node->right, user);
        else {
            if (node->left == nullptr) {
                TravelerNode* tmp = node->right;
                delete node;
                return tmp;
            } else if (node->right == nullptr) {
                TravelerNode* tmp = node->left;
                delete node;
                return tmp;
            }
        }
        return node;
    }
};

// ==================== WAIT QUEUE ====================
class WaitNode {
public:
    string travId;
    string travName;
    string timeStamp;
    WaitNode* next;
    WaitNode(string id, string name, string ts) : travId(id), travName(name), timeStamp(ts), next(nullptr) {}
};




class WaitQueue {
private:
    WaitNode* front;
    WaitNode* back;
    int length;
public:
    WaitQueue() : front(nullptr), back(nullptr), length(0) {}

    void push(const string& id, const string& name, const string& ts) {
        WaitNode* n = new WaitNode(id, name, ts);
        if (back == nullptr) front = back = n;
        else { back->next = n; back = n; }
        ++length;
    }

    bool pop(string& idOut, string& nameOut) {
        if (isEmpty()) return false;
        WaitNode* t = front;
        idOut = front->travId;
        nameOut = front->travName;
        front = front->next;
        if (front == nullptr) back = nullptr;
        delete t;
        --length;
        return true;
    }

    bool isEmpty() { return front == nullptr; }

    void list() {
        if (isEmpty()) { cout << "Waitlist is empty." << endl; return; }
        WaitNode* t = front;
        int pos = 1;
        while (t) {
            cout << pos << ". " << t->travName << " (ID: " << t->travId << ") - " << t->timeStamp << endl;
            t = t->next; ++pos;
        }
    }

    int size() { return length; }
};
// ==================== ROUTE CLASS ====================

class Route {
public:
    string fromCity;
    string toCity;
    Route() : fromCity(""), toCity("") {}
    Route(string f, string t) : fromCity(f), toCity(t) {}
};

// ==================== Trip CLASS ====================
class Trip {
public:
    string code;
    Route route;
    Aircraft* planeAssigned;
    string day;
    string departTime;
    double fare;
    WaitQueue* waitlist;
    Trip* next;

    Trip() : code(""), planeAssigned(nullptr), day(""), departTime(""), fare(0), waitlist(nullptr), next(nullptr) {}

    Trip(string c, Route r, Aircraft* a, string d, string t, double p)
        : code(c), route(r), planeAssigned(a), day(d), departTime(t), fare(p), next(nullptr) {
        waitlist = new WaitQueue();
    }

    void showSummary() {
        int avail = planeAssigned ? planeAssigned->countAvailable() : 0;
        int tot = planeAssigned ? planeAssigned->capacityTotal : 0;
        cout << code << " | " << departTime << " | " << (planeAssigned ? planeAssigned->typeName : "N/A")
             << " | Rs. " << fare << " | " << avail << "/" << tot << " seats | "
             << waitlist->size();
        if (avail == 0) cout << " (FULL)";
        cout << endl;
    }

    int availableSeats() { return planeAssigned ? planeAssigned->countAvailable() : 0; }
};

// ==================== Trip Collection ====================

class TripCollection {
private:
    Trip* first;
public:
    TripCollection() : first(nullptr) {}

    void addTrip(Trip* t) {
        if (first == nullptr) first = t;
        else {
            Trip* tmp = first;
            while (tmp->next != nullptr) tmp = tmp->next;
            tmp->next = t;
        }
    }

    bool removeTrip(const string& code) {
        if (first == nullptr) return false;
        if (first->code == code) {
            Trip* toDel = first;
            first = first->next;
            delete toDel;
            return true;
        }
        Trip* tmp = first;
        while (tmp->next != nullptr) {
            if (tmp->next->code == code) {
                Trip* toDel = tmp->next;
                tmp->next = tmp->next->next;
                delete toDel;
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }

    void findTrips(const string& from, const string& to, const string& date) {
        Trip* t = first;
        bool found = false;
        int idx = 1;
        cout << "\n=== AVAILABLE FLIGHTS ===" << endl;
        while (t) {
            if (t->route.fromCity == from && t->route.toCity == to && t->day == date) {
                cout << idx << ". ";
                t->showSummary();
                found = true;
                ++idx;
            }
            t = t->next;
        }
        if (!found) cout << "No flights found for the given route and date." << endl;
    }

    Trip* getByCode(const string& code) {
        Trip* t = first;
        while (t) {
            if (t->code == code) return t;
            t = t->next;
        }
        return nullptr;
    }

    void displayAll() {
        if (first == nullptr) { cout << "No flights available." << endl; return; }
        Trip* t = first;
        while (t) { t->showSummary(); t = t->next; }
    }

    int totalCount() {
        int cnt = 0;
        Trip* t = first;
        while (t) { ++cnt; t = t->next; }
        return cnt;
    }

    Trip* getHead() { return first; }
};

// ==================== ADMIN ACTION STACK ====================
class ActionNode {
public:
    string kind;
    string info;
    string time;
    void* payload;
    ActionNode* next;
    ActionNode(string k, string i, string tm, void* p = nullptr) : kind(k), info(i), time(tm), payload(p), next(nullptr) {}
};

class ActionStack {
private:
    ActionNode* top;
    int length;
    int limit;
public:
    ActionStack() : top(nullptr), length(0), limit(10) {}

    void push(const string& k, const string& i, const string& tm, void* p = nullptr) {
        ActionNode* n = new ActionNode(k, i, tm, p);
        n->next = top;
        top = n;
        ++length;
        if (length > limit) {
            ActionNode* t = top;
            for (int i = 0; i < limit - 1; ++i) t = t->next;
            ActionNode* rem = t->next;
            t->next = nullptr;
            delete rem;
            length = limit;
        }
    }

    bool pop(string& k, string& i, void*& p) {
        if (isEmpty()) return false;
        ActionNode* t = top;
        k = top->kind;
        i = top->info;
        p = top->payload;
        top = top->next;
        delete t;
        --length;
        return true;
    }

    bool peek(string& k, string& i) {
        if (isEmpty()) return false;
        k = top->kind; i = top->info;
        return true;
    }

    bool isEmpty() { return top == nullptr; }

    void showHistory() {
        if (isEmpty()) { cout << "No admin actions recorded." << endl; return; }
        cout << "\n=== RECENT ADMIN ACTIONS ===" << endl;
        ActionNode* t = top; int idx = 1;
        while (t) {
            cout << idx << ". [" << t->time << "] " << t->kind << " - " << t->info << endl;
            t = t->next; ++idx;
        }
    }
};

// ==================== Account Handler ====================
class AccountHandler {
private:
    TravelerIndex* index;
    int nextId;
public:
    AccountHandler(TravelerIndex* idx) : index(idx), nextId(1001) {}

    bool signUp() {
        string name, user, pwd, contact;
        cout << "\n=== PASSENGER REGISTRATION ===" << endl;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Username: ";
        cin >> user;

        if (index->searchTraveler(user) != nullptr) {
            cout << "Error: Username already exists. Please choose a different username." << endl;
            return false;
        }

        cout << "Enter Password: ";
        cin >> pwd;
        cout << "Enter Contact: ";
        cin >> contact;

        string id = "P" + toText(nextId++);
        Traveler* t = new Traveler(name, id, user, pwd, contact);
        index->insertTraveler(t);

        cout << "Registration successful! Your ID is: " << id << endl;
        return true;
    }

    Traveler* signIn() {
        string user, pwd;
        cout << "\n=== PASSENGER LOGIN ===" << endl;
        cout << "Enter Username: ";
        cin >> user;
        cout << "Enter Password: ";
        cin >> pwd;

        Traveler* t = index->searchTraveler(user);
        if (t != nullptr && t->pass == pwd) {
            cout << "Login successful! Welcome, " << t->fullName << endl;
            return t;
        }
        cout << "Invalid username or password." << endl;
        return nullptr;
    }

    bool checkCredentials(const string& user, const string& pwd) {
        Traveler* t = index->searchTraveler(user);
        return (t != nullptr && t->pass == pwd);
    }
};

// ==================== ADMIN Portal ====================

class AdminPortal {
private:
    string adminUser;
    string adminPass;
public:
    AdminPortal() : adminUser("Ahmed_Abbasi"), adminPass("Khuljasimsim") {}

    bool login() {
        int tries = 0;
        while (tries < 3) {
            string u, p;
            cout << "\n=== ADMIN LOGIN ===" << endl;
            cout << "Enter Username: ";
            cin >> u;
            cout << "Enter Password: ";
            cin >> p;
            if (u == adminUser && p == adminPass) {
                cout << "Admin login successful!" << endl;
                return true;
            }
            ++tries;
            cout << "Invalid credentials. Attempts remaining: " << (3 - tries) << endl;
        }
        cout << "Too many failed attempts. Returning to main menu." << endl;
        return false;
    }

    void menu() {
        cout << "\n========================================" << endl;
        cout << "ADMIN PORTAL" << endl;
        cout << "========================================" << endl;
        cout << "1. Plane Management" << endl;
        cout << "2. Flight Management" << endl;
        cout << "3. System Overview" << endl;
        cout << "4. Undo Last Action" << endl;
        cout << "5. Logout" << endl;
        cout << "========================================" << endl;
    }
};

// ==================== Aircraft Manager ====================

class AircraftManager {
private:
    Aircraft** fleet;
    int capacity;
    int used;
    int nextTag;

    void grow() {
        capacity *= 2;
        Aircraft** tmp = new Aircraft*[capacity];
        for (int i = 0; i < used; ++i) tmp[i] = fleet[i];
        delete[] fleet;
        fleet = tmp;
    }

public:
    AircraftManager() : capacity(10), used(0), nextTag(1) { fleet = new Aircraft*[capacity]; }

    void addAircraft() {
        string model;
        cout << "\nEnter Plane Model: ";
        cin.ignore();
        getline(cin, model);

        string tag = "PL";
        if (nextTag < 10) tag += "00" + toText(nextTag);
        else if (nextTag < 100) tag += "0" + toText(nextTag);
        else tag += toText(nextTag);
        ++nextTag;

        if (used == capacity) grow();

        fleet[used] = new Aircraft(tag, model);
        ++used;

        cout << "Plane added successfully! Plane ID: " << tag << endl;
    }

    void listAll() {
        if (used == 0) { cout << "No planes in the fleet." << endl; return; }
        cout << "\n=== FLEET DETAILS ===" << endl;
        for (int i = 0; i < used; ++i) {
            cout << "ID: " << fleet[i]->aircraftTag << " | Model: " << fleet[i]->typeName
                 << " | Capacity: " << fleet[i]->capacityTotal << " | Available: "
                 << fleet[i]->countAvailable() << endl;
        }
    }

    bool removeAircraft(const string& tag) {
        for (int i = 0; i < used; ++i) {
            if (fleet[i]->aircraftTag == tag) {
                delete fleet[i];
                for (int j = i; j < used - 1; ++j) fleet[j] = fleet[j + 1];
                --used;
                return true;
            }
        }
        return false;
    }

    Aircraft* lookup(const string& tag) {
        for (int i = 0; i < used; ++i) if (fleet[i]->aircraftTag == tag) return fleet[i];
        return nullptr;
    }

    int total() { return used; }

    bool isAssigned(const string& tag, TripCollection* coll) {
        Trip* t = coll->getHead();
        while (t) {
            if (t->planeAssigned && t->planeAssigned->aircraftTag == tag) return true;
            t = t->next;
        }
        return false;
    }
};

// ==================== MAIN SYSTEM ====================

class AirlineSystem {
private:
    TravelerIndex travelerIndex;
    AccountHandler accountHandler;
    TripCollection trips;
    AircraftManager aircrafts;
    AdminPortal admin;
    ActionStack actions;
    int nextResId;
    int nextTripId;

public:
    AirlineSystem() : accountHandler(&travelerIndex), nextResId(1), nextTripId(1) {}

    void mainMenu() {
        while (true) {
            cout << "\n========================================" << endl;
            cout << "AIRLINE RESERVATION SYSTEM" << endl;
            cout << "========================================" << endl;
            cout << "1. Passenger Login" << endl;
            cout << "2. Passenger Registration" << endl;
            cout << "3. Admin Login" << endl;
            cout << "4. Exit" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: {
                    Traveler* t = accountHandler.signIn();
                    if (t) passengerMenu(t);
                    break;
                }
                case 2:
                    accountHandler.signUp();
                    break;
                case 3:
                    if (admin.login()) adminMenu();
                    break;
                case 4:
                    cout << "Thank you for using Airline Reservation System!" << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void passengerMenu(Traveler* trav) {
        while (true) {
            cout << "\n========================================" << endl;
            cout << "WELCOME, " << trav->fullName << endl;
            cout << "========================================" << endl;
            cout << "1. Search Flights" << endl;
            cout << "2. View My Bookings" << endl;
            cout << "3. Cancel Booking" << endl;
            cout << "4. Logout" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";

            int ch;
            cin >> ch;
            switch (ch) {
                case 1: searchAndReserve(trav); break;
                case 2:
                    cout << "\n=== YOUR BOOKINGS ===" << endl;
                    trav->listReservations();
                    break;
                case 3: cancelBooking(trav); break;
                case 4:
                    cout << "Logging out..." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void searchAndReserve(Traveler* trav) {
        string from, to, date;
        cout << "\n=== SEARCH FLIGHTS ===" << endl;
        cout << "Enter Origin City: ";
        cin >> from;
        cout << "Enter Destination City: ";
        cin >> to;
        cout << "Enter Date (DD-MM-YYYY): ";
        cin >> date;

        trips.findTrips(from, to, date);

        cout << "\nEnter flight number to book (or 0 to go back): ";
        string tripCode;
        cin >> tripCode;
        if (tripCode == "0") return;

        Trip* trip = trips.getByCode(tripCode);
        if (!trip) { cout << "Flight not found." << endl; return; }

        if (trip->availableSeats() > 0) {
            trip->planeAssigned->showLayout();
            cout << "\nEnter seat (e.g., 2C): ";
            string seatSel;
            cin >> seatSel;

            if (trip->planeAssigned->reserveSpot(seatSel)) {
                string rid = "BK";
                if (nextResId < 10) rid += "00" + toText(nextResId);
                else if (nextResId < 100) rid += "0" + toText(nextResId);
                else rid += toText(nextResId);
                ++nextResId;

                Reservation* r = new Reservation(rid, trav->idTag, tripCode, seatSel, date);
                trav->appendReservation(r);

                cout << "\n Booking confirmed!" << endl;
                cout << "Booking Reference: " << rid << endl;
                cout << "Seat: " << seatSel << endl;
                cout << "Flight: " << tripCode << endl;
            } else {
                cout << "Seat booking failed. Seat may be invalid or already booked." << endl;
            }
        } else {
            cout << "\nFlight " << tripCode << " is currently full (" << trip->planeAssigned->capacityTotal
                 << "/" << trip->planeAssigned->capacityTotal << " seats booked)" << endl;
            cout << "Would you like to join the waitlist? (Y/N): ";
            char c; cin >> c;
            if (c == 'Y' || c == 'y') {
                trip->waitlist->push(trav->idTag, trav->fullName, date);
                cout << "\n✓ Added to waitlist!" << endl;
                cout << "Your position: " << trip->waitlist->size() << endl;
                cout << "You will be notified if a seat becomes available." << endl;
            }
        }
    }

    void cancelBooking(Traveler* trav) {
        cout << "\n=== CANCEL BOOKING ===" << endl;
        trav->listReservations();

        cout << "\nEnter Booking ID to cancel (or 0 to go back): ";
        string id;
        cin >> id;
        if (id == "0") return;

        Reservation* t = trav->history;
        while (t) {
            if (t->resCode == id) {
                Trip* trip = trips.getByCode(t->tripCode);
                if (trip) {
                    trip->planeAssigned->releaseSpot(t->seatSpot);

                    string wid, wname;
                    if (trip->waitlist->pop(wid, wname)) {
                        cout << "Seat automatically assigned to waitlisted passenger: "
                             << wname << " (ID: " << wid << ")" << endl;
                        trip->planeAssigned->reserveSpot(t->seatSpot);
                    }
                }

                trav->eraseReservation(id);
                cout << "Booking cancelled successfully!" << endl;
                return;
            }
            t = t->next;
        }
        cout << "Booking ID not found." << endl;
    }

    void adminMenu() {
        while (true) {
            admin.menu();
            cout << "Enter your choice: ";
            int ch; cin >> ch;
            switch (ch) {
                case 1: aircraftManagement(); break;
                case 2: flightManagement(); break;
                case 3: systemOverview(); break;
                case 4: undoAction(); break;
                case 5:
                    cout << "Logging out from admin portal..." << endl; return;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void aircraftManagement() {
        cout << "\n=== PLANE MANAGEMENT ===" << endl;
        cout << "1. Add New Plane" << endl;
        cout << "2. View All Planes" << endl;
        cout << "3. Remove Plane" << endl;
        cout << "4. Back" << endl;
        cout << "Enter your choice: ";
        int c; cin >> c;
        switch (c) {
            case 1:
                aircrafts.addAircraft();
                actions.push("ADD_PLANE", "Added new plane", "Current");
                break;
            case 2:
                aircrafts.listAll();
                break;
            case 3: {
                cout << "Enter Plane ID to remove: ";
                string pid; cin >> pid;
                if (aircrafts.isAssigned(pid, &trips)) {
                    cout << "Error: Cannot remove plane. It is assigned to a flight." << endl;
                } else if (aircrafts.removeAircraft(pid)) {
                    cout << "Plane removed successfully!" << endl;
                } else {
                    cout << "Plane not found." << endl;
                }
                break;
            }
            case 4: return;
            default: cout << "Invalid choice." << endl;
        }
    }

    void flightManagement() {
        cout << "\n=== FLIGHT MANAGEMENT ===" << endl;
        cout << "1. Create New Flight" << endl;
        cout << "2. Cancel Flight" << endl;
        cout << "3. View All Flights" << endl;
        cout << "4. Back" << endl;
        cout << "Enter your choice: ";
        int c; cin >> c;
        switch (c) {
            case 1: createTrip(); break;
            case 2: cancelTrip(); break;
            case 3: trips.displayAll(); break;
            case 4: return;
            default: cout << "Invalid choice." << endl;
        }
    }

    void createTrip() {
        string src, dst, planeId, date, time;
        double price;
        cout << "\n=== CREATE NEW FLIGHT ===" << endl;
        cout << "Enter Origin: ";
        cin >> src;
        cout << "Enter Destination: ";
        cin >> dst;
        cout << "Enter Plane ID: ";
        cin >> planeId;

        Aircraft* a = aircrafts.lookup(planeId);
        if (!a) { cout << "Error: Plane not found." << endl; return; }

        cout << "Enter Date (DD-MM-YYYY): ";
        cin >> date;
        cout << "Enter Time (HH:MM AM/PM): ";
        cin.ignore();
        getline(cin, time);
        cout << "Enter Price: ";
        cin >> price;

        string code = "FL";
        if (nextTripId < 10) code += "00" + toText(nextTripId);
        else if (nextTripId < 100) code += "0" + toText(nextTripId);
        else code += toText(nextTripId);
        ++nextTripId;

        Route route(src, dst);
        Trip* t = new Trip(code, route, a, date, time, price);
        trips.addTrip(t);

        cout << "Flight created successfully! Flight Number: " << code << endl;
        actions.push("CREATE_FLIGHT", code + " (" + src + " -> " + dst + ")", "Current");
    }

    void cancelTrip() {
        cout << "\nEnter Flight Number to cancel: ";
        string code; cin >> code;
        Trip* t = trips.getByCode(code);
        if (!t) { cout << "Flight not found." << endl; return; }

        cout << "\n=== AFFECTED PASSENGERS ===" << endl;
        cout << "The following passengers will have their bookings cancelled:" << endl;

        if (t->planeAssigned) {
            for (int i = 0; i < t->planeAssigned->numRows; ++i) {
                for (int j = 0; j < t->planeAssigned->numCols; ++j) {
                    t->planeAssigned->layout[i][j].occupied = false;
                }
            }
        }

        trips.removeTrip(code);
        cout << "Flight cancelled successfully!" << endl;
        actions.push("CANCEL_FLIGHT", code, "Current");
    }

    void systemOverview() {
        cout << "\n=== SYSTEM OVERVIEW ===" << endl;
        cout << "1. View All Passengers (Inorder)" << endl;
        cout << "2. View All Passengers (Preorder)" << endl;
        cout << "3. View All Passengers (Postorder)" << endl;
        cout << "4. View Statistics" << endl;
        cout << "5. View Admin Action History" << endl;
        cout << "6. Back" << endl;
        cout << "Enter your choice: ";
        int c; cin >> c;
        switch (c) {
            case 1: cout << "\n=== PASSENGERS (INORDER) ===" << endl; travelerIndex.printInOrder(); break;
            case 2: cout << "\n=== PASSENGERS (PREORDER) ===" << endl; travelerIndex.printPreOrder(); break;
            case 3: cout << "\n=== PASSENGERS (POSTORDER) ===" << endl; travelerIndex.printPostOrder(); break;
            case 4: displayStats(); break;
            case 5: actions.showHistory(); break;
            case 6: return;
            default: cout << "Invalid choice." << endl;
        }
    }

    void displayStats() {
        int totalTrips = trips.totalCount();
        int totalPlanes = aircrafts.total();
        cout << "\n=== SYSTEM STATISTICS ===" << endl;
        cout << "Total Flights: " << totalTrips << endl;
        cout << "Total Planes: " << totalPlanes << endl;
        cout << "Total Bookings: " << (nextResId - 1) << endl;

        double revenue = 0;
        Trip* t = trips.getHead();
        while (t) {
            int booked = t->planeAssigned->capacityTotal - t->availableSeats();
            revenue += booked * t->fare;
            t = t->next;
        }
        cout << "Total Revenue: Rs. " << revenue << endl;
    }

    void undoAction() {
        string kind, info; void* payload;
        if (!actions.peek(kind, info)) {
            cout << "No actions to undo." << endl; return;
        }
        cout << "\n=== UNDO LAST ACTION ===" << endl;
        cout << "Last action: " << kind << " - " << info << endl;
        cout << "Confirm undo? (Y/N): ";
        char conf; cin >> conf;
        if (conf == 'Y' || conf == 'y') {
            actions.pop(kind, info, payload);
            cout << "Action undone successfully!" << endl;
            cout << "Note: Undo implementation is simplified in this version." << endl;
        } else {
            cout << "Undo cancelled." << endl;
        }
    }
};

// ==================== MAIN FUNCTION ====================

int main() {
    AirlineSystem sys;
    sys.mainMenu();
    return 0;
}
