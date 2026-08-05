#include <bits/stdc++.h>
using namespace std;

//==================== ENUMS ====================//

enum class SeatStatus{
    AVAILABLE,
    BOOKED
};

enum class BookingStatus{
    PENDING,
    CONFIRMED,
    CANCELLED
};

enum class PaymentStatus{
    PENDING,
    SUCCESS,
    FAILED
};

enum class SeatType{
    ECONOMY,
    BUSINESS
};

//==================== USER ====================//

class User{

private:
    int userId;
    string name;

public:

    User(int userId,string name){
        this->userId = userId;
        this->name = name;
    }

    int getUserId(){
        return userId;
    }

    string getName(){
        return name;
    }
};

//==================== SEAT ====================//

class Seat{

private:

    string seatNumber;
    SeatType seatType;
    SeatStatus status;

public:

    Seat(string seatNumber,SeatType seatType){

        this->seatNumber = seatNumber;
        this->seatType = seatType;
        status = SeatStatus::AVAILABLE;
    }

    string getSeatNumber(){
        return seatNumber;
    }

    SeatType getSeatType(){
        return seatType;
    }

    SeatStatus getSeatStatus(){
        return status;
    }

    bool isAvailable(){
        return status == SeatStatus::AVAILABLE;
    }

    bool book(){

        if(status == SeatStatus::BOOKED)
            return false;

        status = SeatStatus::BOOKED;
        return true;
    }

    void release(){
        status = SeatStatus::AVAILABLE;
    }
};

//==================== FLIGHT ====================//

class Flight{

private:

    int flightId;
    string flightNumber;

    string source;
    string destination;

    string departureDate;
    string departureTime;
    string arrivalTime;

    vector<Seat*> seats;

public:

    Flight(int flightId,
           string flightNumber,
           string source,
           string destination,
           string departureDate,
           string departureTime,
           string arrivalTime){

        this->flightId = flightId;
        this->flightNumber = flightNumber;
        this->source = source;
        this->destination = destination;
        this->departureDate = departureDate;
        this->departureTime = departureTime;
        this->arrivalTime = arrivalTime;
    }

    void addSeat(Seat* seat){
        seats.push_back(seat);
    }

    vector<Seat*> getSeats(){
        return seats;
    }

    int getFlightId(){
        return flightId;
    }

    string getFlightNumber(){
        return flightNumber;
    }

    string getSource(){
        return source;
    }

    string getDestination(){
        return destination;
    }

    string getDepartureDate(){
        return departureDate;
    }

    string getDepartureTime(){
        return departureTime;
    }

    string getArrivalTime(){
        return arrivalTime;
    }

    bool hasAvailableSeat(){

        for(auto seat : seats){

            if(seat->isAvailable())
                return true;
        }

        return false;
    }

    Seat* findAvailableSeat(){

        for(auto seat : seats){

            if(seat->isAvailable())
                return seat;
        }

        return nullptr;
    }
};

//==================== PAYMENT ====================//

class Payment{

private:

    int transactionId;
    double amount;
    PaymentStatus paymentStatus;

public:

    Payment(int transactionId,double amount){

        this->transactionId = transactionId;
        this->amount = amount;
        paymentStatus = PaymentStatus::PENDING;
    }

    bool pay(){

        cout<<"Processing Payment..."<<endl;

        paymentStatus = PaymentStatus::SUCCESS;

        cout<<"Payment Successful"<<endl;

        return true;
    }

    int getTransactionId(){
        return transactionId;
    }

    double getAmount(){
        return amount;
    }

    PaymentStatus getPaymentStatus(){
        return paymentStatus;
    }

};

//==================== BOOKING ====================//

class Booking{

private:

    int bookingId;

    User* user;

    Flight* flight;

    Seat* seat;

    Payment* payment;

    BookingStatus bookingStatus;

    string bookingTime;

public:

    Booking(int bookingId,
            User* user,
            Flight* flight,
            Seat* seat,
            Payment* payment,
            string bookingTime){

        this->bookingId = bookingId;
        this->user = user;
        this->flight = flight;
        this->seat = seat;
        this->payment = payment;
        this->bookingTime = bookingTime;

        bookingStatus = BookingStatus::PENDING;
    }

    int getBookingId(){
        return bookingId;
    }

    User* getUser(){
        return user;
    }

    Flight* getFlight(){
        return flight;
    }

    Seat* getSeat(){
        return seat;
    }

    Payment* getPayment(){
        return payment;
    }

    BookingStatus getBookingStatus(){
        return bookingStatus;
    }

    string getBookingTime(){
        return bookingTime;
    }

    void confirmBooking(){

        bookingStatus = BookingStatus::CONFIRMED;
    }

    void cancelBooking(){

        bookingStatus = BookingStatus::CANCELLED;
    }

};

//==================== AIRLINE ====================//

class Airline{

private:

    string airlineName;

    vector<Flight*> flights;

public:

    Airline(string airlineName){

        this->airlineName = airlineName;
    }

    void addFlight(Flight* flight){

        flights.push_back(flight);
    }

    vector<Flight*> getFlights(){

        return flights;
    }

    Flight* searchFlight(string source,
                         string destination,
                         string departureDate){

        for(auto flight : flights){

            if(flight->getSource() == source &&
               flight->getDestination() == destination &&
               flight->getDepartureDate() == departureDate){

                return flight;
            }
        }

        return nullptr;
    }

    string getAirlineName(){

        return airlineName;
    }

};

//==================== NOTIFICATION SERVICE ====================//

class NotificationService{

public:

    void sendEmail(User* user){

        cout << "Email sent to " << user->getName() << endl;
    }

    void sendSMS(User* user){

        cout << "SMS sent to " << user->getName() << endl;
    }

    void sendPushNotification(User* user){

        cout << "Push Notification sent to " << user->getName() << endl;
    }
};

//==================== BOOKING SERVICE ====================//

class BookingService{

private:

    NotificationService notificationService;

public:

    Booking* bookFlight(User* user,
                        Flight* flight,
                        int bookingId,
                        int transactionId,
                        double amount){

        // Check Flight
        if(flight == nullptr){

            cout<<"Flight not found."<<endl;
            return nullptr;
        }

        // Check Seat Availability
        Seat* seat = flight->findAvailableSeat();

        if(seat == nullptr){

            cout<<"No seats available."<<endl;
            return nullptr;
        }

        // Create Payment
        Payment* payment = new Payment(transactionId, amount);

        // Create Booking
        Booking* booking = new Booking(
                                bookingId,
                                user,
                                flight,
                                seat,
                                payment,
                                "05-08-2026");

        // Process Payment
        bool paymentSuccess = payment->pay();

        if(!paymentSuccess){

            cout<<"Payment Failed"<<endl;
            return nullptr;
        }

        // Confirm Booking
        booking->confirmBooking();

        // Occupy Seat
        seat->book();

        // Send Notifications
        notificationService.sendEmail(user);
        notificationService.sendSMS(user);
        notificationService.sendPushNotification(user);

        cout<<endl;
        cout<<"Booking Successful"<<endl;
        cout<<"Booking Id : "<<booking->getBookingId()<<endl;
        cout<<"Flight     : "<<flight->getFlightNumber()<<endl;
        cout<<"Seat       : "<<seat->getSeatNumber()<<endl;

        return booking;
    }

    void cancelBooking(Booking* booking){

        if(booking == nullptr)
            return;

        booking->cancelBooking();

        booking->getSeat()->release();

        cout<<"Booking Cancelled Successfully"<<endl;
    }

    Flight* searchFlight(Airline* airline,
                         string source,
                         string destination,
                         string date){

        return airline->searchFlight(source,
                                     destination,
                                     date);
    }

    void displayBooking(Booking* booking){

        if(booking == nullptr){

            cout<<"Booking Not Found"<<endl;
            return;
        }

        cout<<"\n========= BOOKING DETAILS ========="<<endl;

        cout<<"Booking Id : "
            <<booking->getBookingId()<<endl;

        cout<<"Passenger  : "
            <<booking->getUser()->getName()<<endl;

        cout<<"Flight     : "
            <<booking->getFlight()->getFlightNumber()<<endl;

        cout<<"Seat       : "
            <<booking->getSeat()->getSeatNumber()<<endl;

        cout<<"Amount     : "
            <<booking->getPayment()->getAmount()<<endl;

        cout<<"Transaction: "
            <<booking->getPayment()->getTransactionId()<<endl;

        cout<<"==================================="<<endl;
    }

};

int main() {

    //================== Create Airline ==================//

    Airline airline("IndiGo");

    //================== Create Flight ==================//

    Flight* flight1 = new Flight(
        101,
        "6E-205",
        "Delhi",
        "Bangalore",
        "10-08-2026",
        "09:00",
        "12:00"
    );

    //================== Add Seats ==================//

    flight1->addSeat(new Seat("1A", SeatType::BUSINESS));
    flight1->addSeat(new Seat("1B", SeatType::BUSINESS));
    flight1->addSeat(new Seat("2A", SeatType::ECONOMY));
    flight1->addSeat(new Seat("2B", SeatType::ECONOMY));
    flight1->addSeat(new Seat("3A", SeatType::ECONOMY));

    airline.addFlight(flight1);

    //================== Create User ==================//

    User* user = new User(1, "Urvashi");

    //================== Booking Service ==================//

    BookingService bookingService;

    //================== Search Flight ==================//

    Flight* flight = bookingService.searchFlight(
        &airline,
        "Delhi",
        "Bangalore",
        "10-08-2026"
    );

    if (flight == nullptr) {

        cout << "Flight Not Found" << endl;
        return 0;
    }

    cout << "Flight Found : "
         << flight->getFlightNumber()
         << endl;

    //================== Book Flight ==================//

    Booking* booking = bookingService.bookFlight(
        user,
        flight,
        1001,
        5001,
        6500
    );

    //================== Display Booking ==================//

    bookingService.displayBooking(booking);

    //================== Cancel Booking ==================//

    cout << endl;
    cout << "Cancelling Booking..." << endl;

    bookingService.cancelBooking(booking);

    cout << endl;

    if (booking->getSeat()->isAvailable())
        cout << "Seat Released Successfully" << endl;

    return 0;
}

