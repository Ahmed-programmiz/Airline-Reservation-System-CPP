# Airline Reservation System (C++)

A console-based Airline Reservation System written in C++ to demonstrate object-oriented programming and fundamental data structures. Passengers can register, search for flights, book seats, and cancel reservations. Administrators can manage aircraft and flights, and view basic system statistics.

## Features

**Passenger portal**
- Register a passenger account and log in.
- Search flights by origin, destination, and date.
- View seat availability and reserve a specific seat.
- View and cancel bookings.
- Join a waitlist when a flight is full.

**Administrator portal**
- Log in to the administrator menu.
- Add, view, and remove aircraft.
- Create, view, and cancel flights.
- View registered passengers and system statistics.
- Review an action history and access a simplified undo option.

## Programming concepts

- **Object-oriented programming:** classes and object relationships for passengers, aircraft, seats, flights, bookings, and administration.
- **Binary search tree:** index passengers by username and traverse them in preorder, inorder, and postorder.
- **Linked lists:** store flights and passenger reservations.
- **Queue:** manage flight waitlists.
- **Stack:** store recent administrator actions.
- **Dynamic memory:** allocate seats, aircraft, and linked data structures.

## Requirements

- A C++ compiler supporting C++11 or later (for example, GCC / MinGW-w64).
- A command-line terminal.

## Build and run

From the project root:

**Windows (MinGW-w64 / g++ in PowerShell):**

```powershell
g++ -std=c++17 -o APMS.exe src/APMS.cpp
.\APMS.exe
```

**Linux or macOS (g++):**

```bash
g++ -std=c++17 -o apms src/APMS.cpp
./apms
```

The initial menu offers Passenger Login, Passenger Registration, Admin Login, and Exit. To see bookable flights, an administrator must first add an aircraft and create a flight.

## Project structure

```text
Airline-Reservation-System-CPP/
├── src/
│   └── APMS.cpp       # Main C++ source
├── .gitignore         # Excludes compiled binaries and temp files
└── README.md          # Project documentation
```

## Important limitations and security notes

This is an **educational console application**, not a production booking system.

- Data is held in memory and is not saved between program runs.
- Administrator login credentials are **hardcoded in the source**, and passenger passwords are stored in plain text in memory. Do not use real passwords. Replace this approach before making the project public or deploying it.
- The undo feature is marked as simplified in the application.
- Joining the waitlist prints a notification message, but the program does not send external messages.
- The included source is the original submitted project, with the file extension normalized from `.c++` to `.cpp`; no application logic was modified.

## Author

Student C++ project.
