#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

struct BusTrip {
    string destination;
    string departureTime;
    double ticketPrice;
    int availableSeats;
};

struct BusTicket {
    string passengerName;
    int TicketNumber;
    double ticketPrice;
    BusTrip trip;
};

vector<BusTrip> readBusTripsFromFile(const string& filename) {
    vector<BusTrip> trips;
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        BusTrip trip;
        while (inputFile >> trip.destination >> trip.departureTime >> trip.ticketPrice >> trip.availableSeats) {
            trips.push_back(trip);
        }
        inputFile.close();
    } else {
        cerr << "Error: Unable to open the file for reading.\n";
    }

    return trips;
}

void displayAvailableTrips(const vector<BusTrip>& trips) {
    cout << "\nAvailable Bus Trips From Putrajaya Sentral:\n";
    cout << "-----------------------------------------------------------------------\n";
    cout << "ID\tDestination\tDepartureTime\tPrice\t\tAvailableSeats\n";
    for (size_t i = 0; i < trips.size(); ++i) {
        cout << i + 1 << "\t" << trips[i].destination << "\t" << trips[i].departureTime
             << "\t\tRM" << fixed << setprecision(2) << trips[i].ticketPrice << "\t\t" << trips[i].availableSeats << "\n";
    }
    cout << "-----------------------------------------------------------------------\n";
}

void bookTicket(BusTicket& ticket, vector<BusTrip>& trips, vector<BusTicket>& bookedTickets, double& totalAmount) {
    int tripId;

    displayAvailableTrips(trips);

    cout << "\nEnter the ID of the trip you want to book: ";
    cin >> tripId;

    if (tripId >= 1 && tripId <= static_cast<int>(trips.size())) {
        ticket.trip = trips[tripId - 1];
        ticket.ticketPrice = ticket.trip.ticketPrice; // Add this line

        cout << "Enter passenger name: ";
        cin.ignore();
        getline(cin, ticket.passengerName);

        cout << "Enter Number of Ticket: ";
        cin >> ticket.TicketNumber;

        if (ticket.TicketNumber < 1 || ticket.TicketNumber > ticket.trip.availableSeats) {
            cerr << "Error: Invalid Ticket number.\n";
            return;
        }
        for (const auto& bookedTicket : bookedTickets) {
            if (bookedTicket.trip.destination == ticket.trip.destination &&
                bookedTicket.trip.departureTime == ticket.trip.departureTime &&
                bookedTicket.TicketNumber == ticket.TicketNumber) {
                cerr << "\nError: Seat already full for this trip.";
                cerr << "\nPlease book another ticket.\n";
                return;
            }
        }

        totalAmount += ticket.ticketPrice * ticket.TicketNumber;

        trips[tripId - 1].availableSeats -= ticket.TicketNumber;

        bookedTickets.push_back(ticket);
        cout << "\nTicket Booking Successful!\n";
    } else {
        cerr << "Error: Invalid trip ID.\n";
    }
}

void printReceipt(const BusTicket& ticket) {
    ofstream outputFile("receipt.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "------------------- Receipt -------------------\n";
        outputFile << "Passenger Name: " << ticket.passengerName << endl;
        outputFile << "Destination: " << ticket.trip.destination << endl;
        outputFile << "Departure Time: " << ticket.trip.departureTime << endl;
        outputFile << "Total Ticket: " << ticket.TicketNumber << endl;
        outputFile << "Total Price: RM" << fixed << setprecision(2) << ticket.ticketPrice * ticket.TicketNumber << endl;
        outputFile.close();
        cout << "\n\n";
        cout << "***********************************************************************" << endl;
        cout << "------------------- Receipt -------------------\n";
        cout << "Passenger Name: " << ticket.passengerName << endl;
        cout << "Destination: " << ticket.trip.destination << endl;
        cout << "Departure Time: " << ticket.trip.departureTime << endl;
        cout << "Total Ticket: " << ticket.TicketNumber << endl;
        cout << "Total Price: RM" << fixed << setprecision(2) << ticket.ticketPrice * ticket.TicketNumber << endl;
        cout << "Please pay and redeem your ticket at Putrajaya Sentral !" << endl;
        cout << "Receipt saved to 'receipt.txt'\n";
        cout << "***********************************************************************" << endl;
    } else {
        cerr << "Error: Unable to open the file for writing.\n";
    }
}

int main() {
    int choice;
    double totalAmount = 0.0; 
    vector<BusTrip> trips = readBusTripsFromFile("bus_trips.txt");
    vector<BusTicket> bookedTickets;

    while (true) {
        cout << "\n";
        cout << "\t\t\tWelcome to Ticket Bus Online\t\t\t";
        cout << "\n\t\t\t1.Display Available Buses\n\t\t\t" << "2.Book a Ticket\n\t\t\t" << "3.Show Receipt\n\t\t\t" << "4.Exit";
        cout << "\n\t\t\tEnter your choice:-> ";
        cin >> choice;

        switch (choice) {
            case 1: {
                displayAvailableTrips(trips);
                break;
            }
            case 2: {
                BusTicket ticket;
                bookTicket(ticket, trips, bookedTickets, totalAmount);
                break;
            }
            case 3: {
                for (const auto& ticket : bookedTickets) {
                    printReceipt(ticket);
                }
                break;
            }
            case 4:
                exit(0);
            default:
                cerr << "Error: Invalid choice. Please enter a valid option.\n";
                break;
        }
    }

    return 0;
}
