#include <bits/stdc++.h>
using namespace std;

//===========================
// Forward Declarations
//===========================

class ParkingTicket;
class ParkingRow;
class ParkingFloor;

//===========================
// Enums
//===========================

enum class VehicleType {
    BIKE,
    CAR
};

enum class ParkingSpotStatus {
    AVAILABLE,
    OCCUPIED
};

//===========================
// Vehicle Class
//===========================

class Vehicle {

private:
    string vehicleNumber;
    VehicleType vehicleType;

public:

    Vehicle(string vehicleNumber,
            VehicleType vehicleType) {

        this->vehicleNumber = vehicleNumber;
        this->vehicleType = vehicleType;
    }

    string getVehicleNumber() {
        return vehicleNumber;
    }

    VehicleType getVehicleType() {
        return vehicleType;
    }
};

//===========================
// Payment Class
//===========================

class Payment {

private:

    int transactionId;
    double amount;

public:

    Payment(int transactionId,
            double amount) {

        this->transactionId = transactionId;
        this->amount = amount;
    }

    int getTransactionId() {
        return transactionId;
    }

    double getAmount() {
        return amount;
    }

    void calculateAmount(VehicleType type) {

        if(type == VehicleType::BIKE)
            amount = 20;

        else
            amount = 50;
    }

    void pay() {

        cout << "Payment Successful\n";
        cout << "Transaction Id : "
             << transactionId
             << endl;

        cout << "Amount Paid : "
             << amount
             << endl;
    }
};

//===========================
// ParkingSpot
//===========================

class ParkingSpot {

private:

    int spotNumber;
    ParkingSpotStatus status;
    ParkingTicket* currentTicket;

public:

    ParkingSpot(int spotNumber) {

        this->spotNumber = spotNumber;
        status = ParkingSpotStatus::AVAILABLE;
        currentTicket = nullptr;
    }

    int getSpotNumber() {

        return spotNumber;
    }

    bool isAvailable() {

        return status ==
            ParkingSpotStatus::AVAILABLE;
    }

    ParkingTicket* getCurrentTicket() {

        return currentTicket;
    }

    void occupy(ParkingTicket* ticket) {

        currentTicket = ticket;
        status = ParkingSpotStatus::OCCUPIED;
    }

    void vacate() {

        currentTicket = nullptr;
        status = ParkingSpotStatus::AVAILABLE;
    }
};

//====================================
// Parking Row
//====================================

class ParkingRow {

private:

    int rowNumber;
    vector<ParkingSpot*> parkingSpots;

public:

    ParkingRow(int rowNumber) {

        this->rowNumber = rowNumber;
    }

    int getRowNumber() {

        return rowNumber;
    }

    void addParkingSpot(ParkingSpot* spot) {

        parkingSpots.push_back(spot);
    }

    vector<ParkingSpot*> getParkingSpots() {

        return parkingSpots;
    }

    ParkingSpot* findAvailableSpot() {

        for (ParkingSpot* spot : parkingSpots) {

            if (spot->isAvailable()) {
                return spot;
            }

        }

        return nullptr;
    }

};

//====================================
// Parking Floor
//====================================

class ParkingFloor {

private:

    int floorNumber;
    vector<ParkingRow*> parkingRows;

public:

    ParkingFloor(int floorNumber) {

        this->floorNumber = floorNumber;
    }

    int getFloorNumber() {

        return floorNumber;
    }

    void addParkingRow(ParkingRow* row) {

        parkingRows.push_back(row);
    }

    vector<ParkingRow*> getParkingRows() {

        return parkingRows;
    }

    ParkingSpot* findAvailableSpot() {

        for (ParkingRow* row : parkingRows) {

            ParkingSpot* spot =
                row->findAvailableSpot();

            if (spot != nullptr)
                return spot;
        }

        return nullptr;
    }

};

//====================================
// Parking Ticket
//====================================

class ParkingTicket {

private:

    int ticketId;

    Vehicle* vehicle;

    ParkingSpot* parkingSpot;

    Payment *payment;

    string entryTime;
    string exitTime;

public:

    ParkingTicket(
        int ticketId,
        Vehicle* vehicle,
        ParkingSpot* parkingSpot,
        Payment* payment
    ){

        this->ticketId = ticketId;
        this->vehicle = vehicle;
        this->parkingSpot = parkingSpot;
        this->payment = payment;
        

        entryTime = "03-08-2026 10:00 AM";
        exitTime = "";
    }

    int getTicketId() {

        return ticketId;
    }

    Vehicle* getVehicle() {

        return vehicle;
    }

    ParkingSpot* getParkingSpot() {

        return parkingSpot;
    }

    Payment* getPayment() {

        return payment;
    }

    string getEntryTime() {

        return entryTime;
    }

    string getExitTime() {

        return exitTime;
    }

    void closeTicket() {

        exitTime = "03-08-2026 02:30 PM";
    }

};

//====================================
// Parking Lot
//====================================

class ParkingLot {

private:

    string name;

    vector<ParkingFloor*> parkingFloors;

    unordered_map<int, ParkingTicket*> activeTickets;

public:

    ParkingLot(string name) {

        this->name = name;
    }

    void addParkingFloor(ParkingFloor* floor) {

        parkingFloors.push_back(floor);
    }

    ParkingSpot* findAvailableSpot() {

        for (ParkingFloor* floor : parkingFloors) {

            ParkingSpot* spot =
                floor->findAvailableSpot();

            if (spot != nullptr) {
                return spot;
            }
        }

        return nullptr;
    }

    ParkingTicket* generateParkingTicket(
        Vehicle* vehicle,
        ParkingSpot* spot,
        Payment* pay
    ) {

        ParkingTicket* ticket =
            new ParkingTicket(
                rand(),
                vehicle,
                spot,
                pay
            );

        return ticket;
    }

    void parkVehicle(Vehicle* vehicle) {

        ParkingSpot* spot =
            findAvailableSpot();

        if (spot == nullptr) {

            cout << "Parking Full\n";
            return;
        }
        
        Payment* payment = new Payment(rand(),0);

        payment->calculateAmount(vehicle->getVehicleType());

        ParkingTicket* ticket =
        generateParkingTicket(
                vehicle,
                spot,
                payment
            );
                
                
        spot->occupy(ticket);

        activeTickets[
            ticket->getTicketId()
        ] = ticket;

        cout << "Vehicle Parked Successfully\n";

        cout << "Ticket Id : "
             << ticket->getTicketId()
             << endl;

        cout << "Spot Number : "
             << spot->getSpotNumber()
             << endl;
    }

    Vehicle* searchParkedVehicle(
        int ticketId
    ) {

        if(activeTickets.find(ticketId)
            == activeTickets.end()) {

            return nullptr;
        }

        return activeTickets[ticketId]
                    ->getVehicle();
    }

    void exitVehicle(
        int ticketId
    ) {

        if(activeTickets.find(ticketId)
            == activeTickets.end()) {

            cout << "Invalid Ticket\n";
            return;
        }

        ParkingTicket* ticket =
            activeTickets[ticketId];

        ticket->closeTicket();

        ticket->getPayment()->pay();

        ticket->getParkingSpot()->vacate();

        activeTickets.erase(ticketId);

        cout << "Vehicle Exited Successfully\n";

        delete ticket;
    }

    void displayActiveTickets() {

        cout << "\nActive Tickets\n";

        for(auto entry : activeTickets) {

            ParkingTicket* ticket =
                entry.second;

            cout << "Ticket : "
                 << ticket->getTicketId()
                 << " Vehicle : "
                 << ticket->getVehicle()
                        ->getVehicleNumber()
                 << endl;
        }
    }

};

//====================================
// Main
//====================================

int main() {

    //-----------------------------
    // Create Parking Lot
    //-----------------------------

    ParkingLot* parkingLot =
        new ParkingLot("City Parking");



    //-----------------------------
    // Floor 1
    //-----------------------------

    ParkingFloor* floor1 =
        new ParkingFloor(1);



    ParkingRow* row1 =
        new ParkingRow(1);

    ParkingRow* row2 =
        new ParkingRow(2);



    // Row 1 -> 10 Spots

    for(int i = 1; i <= 10; i++) {

        row1->addParkingSpot(
            new ParkingSpot(i)
        );
    }



    // Row 2 -> 10 Spots

    for(int i = 11; i <= 20; i++) {

        row2->addParkingSpot(
            new ParkingSpot(i)
        );
    }



    floor1->addParkingRow(row1);
    floor1->addParkingRow(row2);



    parkingLot->addParkingFloor(floor1);



    //-----------------------------
    // Create Vehicles
    //-----------------------------

    Vehicle* bike =
        new Vehicle(
            "KA01AB1111",
            VehicleType::BIKE
        );



    Vehicle* car =
        new Vehicle(
            "KA05XY2222",
            VehicleType::CAR
        );



    //-----------------------------
    // Park Vehicles
    //-----------------------------

    cout << "\nParking Bike\n";
    parkingLot->parkVehicle(bike);

    cout << "\nParking Car\n";
    parkingLot->parkVehicle(car);



    //-----------------------------
    // Show Active Tickets
    //-----------------------------

    parkingLot->displayActiveTickets();



    //-----------------------------
    // Search Vehicle
    //-----------------------------

    cout << "\nEnter Ticket Id : ";

    int ticketId;
    cin >> ticketId;



    Vehicle* vehicle =
        parkingLot->searchParkedVehicle(ticketId);



    if(vehicle != nullptr) {

        cout << "\nVehicle Found\n";

        cout << vehicle->getVehicleNumber()
             << endl;
    }

    else {

        cout << "\nVehicle Not Found\n";
    }



    //-----------------------------
    // Exit Vehicle
    //-----------------------------

    cout << "\nEnter Ticket Id to Exit : ";

    cin >> ticketId;

    parkingLot->exitVehicle(ticketId);



    cout << endl;

    parkingLot->displayActiveTickets();



    //-----------------------------
    // Cleanup
    //-----------------------------

    delete bike;
    delete car;

    delete row1;
    delete row2;

    delete floor1;

    delete parkingLot;

    return 0;
}