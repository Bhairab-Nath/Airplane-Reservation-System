#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User structure
struct User {
    int userID;
    char username[30];
    char password[30];
};

// Flight structure
struct Flight {
    int flightID;
    char destination[50];
    int seatsAvailable;
    float price;
};

// Booking structure
struct Booking {
    int bookingID;
    int userID;
    int flightID;
    int status; // 1 for confirmed, 0 for canceled
};

int currentUserID = -1; // Stores the ID of the logged-in user
int bookingID = 1001; // For unique booking IDs

void registerUser() {
    FILE *file = fopen("users.dat", "ab");
    struct User user;

    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);

    user.userID = rand() % 1000 + 1; // Random user ID
    fwrite(&user, sizeof(struct User), 1, file);

    printf("User registered successfully! Your User ID: %d\n", user.userID);
    fclose(file);
}

int loginUser() {
    FILE *file = fopen("users.dat", "rb");
    struct User user;
    char username[30], password[30];

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    while (fread(&user, sizeof(struct User), 1, file)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            printf("Login Successful!\n");
            currentUserID = user.userID;
            fclose(file);
            return 1;
        }
    }

    printf("Invalid Credentials!\n");
    fclose(file);
    return 0;
}

void addFlight() {
    FILE *file = fopen("flights.dat", "ab");
    struct Flight flight;

    printf("Enter Flight ID: ");
    scanf("%d", &flight.flightID);
    printf("Enter Destination: ");
    scanf("%s", flight.destination);
    printf("Available Seats: ");
    scanf("%d", &flight.seatsAvailable);
    printf("Flight Price: ");
    scanf("%f", &flight.price);

    fwrite(&flight, sizeof(struct Flight), 1, file);
    printf("Flight added successfully!\n");
    fclose(file);
}

void searchFlights() {
    FILE *file = fopen("flights.dat", "rb");
    struct Flight flight;

    printf("\nAvailable Flights:\n");
    printf("Flight ID\tDestination\tSeats\tPrice\n");
    printf("------------------------------------------------\n");

    while (fread(&flight, sizeof(struct Flight), 1, file)) {
        printf("%d\t\t%s\t\t%d\t%.2f\n", 
            flight.flightID, flight.destination, flight.seatsAvailable, flight.price);
    }

    fclose(file);
}

void bookFlight() {
    FILE *file = fopen("flights.dat", "rb+");
    FILE *bookingFile = fopen("bookings.dat", "ab");
    struct Flight flight;
    struct Booking booking;
    int flightID, found = 0;

    printf("Enter Flight ID to Book: ");
    scanf("%d", &flightID);

    while (fread(&flight, sizeof(struct Flight), 1, file)) {
        if (flight.flightID == flightID && flight.seatsAvailable > 0) {
            found = 1;
            flight.seatsAvailable--;

            fseek(file, -sizeof(struct Flight), SEEK_CUR);
            fwrite(&flight, sizeof(struct Flight), 1, file);

            booking.bookingID = bookingID++;
            booking.userID = currentUserID;
            booking.flightID = flightID;
            booking.status = 1;

            fwrite(&booking, sizeof(struct Booking), 1, bookingFile);

            printf("Flight booked successfully! Booking ID: %d\n", booking.bookingID);
            break;
        }
    }

    if (!found) {
        printf("Flight not found or no seats available!\n");
    }

    fclose(file);
    fclose(bookingFile);
}

void viewBookings() {
    FILE *file = fopen("bookings.dat", "rb");
    struct Booking booking;

    printf("\nYour Booking History:\n");
    printf("Booking ID\tFlight ID\tStatus\n");
    printf("-----------------------------------\n");

    while (fread(&booking, sizeof(struct Booking), 1, file)) {
        if (booking.userID == currentUserID) {
            printf("%d\t\t%d\t\t%s\n", 
                booking.bookingID, booking.flightID, booking.status ? "Confirmed" : "Cancelled");
        }
    }

    fclose(file);
}

void cancelBooking() {
    FILE *file = fopen("bookings.dat", "rb+");
    struct Booking booking;
    int bookingID, found = 0;

    printf("Enter Booking ID to Cancel: ");
    scanf("%d", &bookingID);

    while (fread(&booking, sizeof(struct Booking), 1, file)) {
        if (booking.bookingID == bookingID && booking.userID == currentUserID && booking.status == 1) {
            found = 1;
            booking.status = 0;

            fseek(file, -sizeof(struct Booking), SEEK_CUR);
            fwrite(&booking, sizeof(struct Booking), 1, file);

            printf("Booking cancelled successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Booking not found or already cancelled!\n");
    }

    fclose(file);
}

int adminLogin() {
    char username[20], password[20];

    printf("\n🔐 Admin Login\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    // Check for admin credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("✅ Admin Login Successful!\n");
        return 1; // Return 1 if login is successful
    } else {
        printf("❌ Invalid Admin Credentials!\n");
        return 0; // Return 0 if login fails
    }
}




int main() {
    int choice;

    while (1) {
        printf("\nAirplane Booking System\n");
        printf("1. Register\n2. Login\n3. Search Flights\n4. Book Flight\n5. View Bookings\n6. Cancel Booking\n7. Add Flight (Admin)\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: searchFlights(); break;
            case 4: bookFlight(); break;
            case 5: viewBookings(); break;
            case 6: cancelBooking(); break;
           
           case 7: 
              if (adminLogin()) {
              addFlight(); 
              } else {
              printf("Access Denied!\n");
              }
           break; 
            case 8: exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}


