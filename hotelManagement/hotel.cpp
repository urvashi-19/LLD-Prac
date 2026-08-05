#include <bits/stdc++.h>
using namespace std;

//==================== ENUMS ====================//

enum class RoomStatus{
    AVAILABLE,
    BOOKED,
    OCCUPIED
};

enum class RoomType{
    SINGLE,
    DOUBLE,
    DELUXE,
    SUITE
};

enum class BookingStatus{
    PENDING,
    CONFIRMED,
    CANCELLED,
    CHECKED_IN,
    CHECKED_OUT
};

enum class PaymentStatus{
    PENDING,
    SUCCESS,
    FAILED
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

//==================== ROOM ====================//

class Room{

private:

    int roomNumber;
    RoomType roomType;
    int pricePerNight;
    RoomStatus status;

public:

    Room(int roomNumber,
         RoomType roomType,
         int pricePerNight){

        this->roomNumber = roomNumber;
        this->roomType = roomType;
        this->pricePerNight = pricePerNight;
        status = RoomStatus::AVAILABLE;
    }

    int getRoomNumber(){

        return roomNumber;
    }

    RoomType getRoomType(){

        return roomType;
    }

    int getPricePerNight(){

        return pricePerNight;
    }

    RoomStatus getRoomStatus(){

        return status;
    }

    bool isAvailable(){

        return status == RoomStatus::AVAILABLE;
    }

    bool book(){

        if(status != RoomStatus::AVAILABLE)
            return false;

        status = RoomStatus::BOOKED;
        return true;
    }

    void checkIn(){

        status = RoomStatus::OCCUPIED;
    }

    void release(){

        status = RoomStatus::AVAILABLE;
    }

};

//==================== HOTEL ====================//

class Hotel{

private:

    int hotelId;
    string hotelName;
    string location;

    vector<Room*> rooms;

public:

    Hotel(int hotelId,
          string hotelName,
          string location){

        this->hotelId = hotelId;
        this->hotelName = hotelName;
        this->location = location;
    }

    void addRoom(Room* room){

        rooms.push_back(room);
    }

    vector<Room*> getRooms(){

        return rooms;
    }

    string getHotelName(){

        return hotelName;
    }

    string getLocation(){

        return location;
    }

    Room* findAvailableRoom(){

        for(auto room : rooms){

            if(room->isAvailable())
                return room;
        }

        return nullptr;
    }

    bool hasAvailableRoom(){

        return findAvailableRoom()!=nullptr;
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

        cout << "Processing Payment..." << endl;

        paymentStatus = PaymentStatus::SUCCESS;

        cout << "Payment Successful" << endl;

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

    Hotel* hotel;

    Room* room;

    Payment* payment;

    BookingStatus bookingStatus;

    string checkInDate;

    string checkOutDate;

public:

    Booking(int bookingId,
            User* user,
            Hotel* hotel,
            Room* room,
            Payment* payment,
            string checkInDate,
            string checkOutDate){

        this->bookingId = bookingId;
        this->user = user;
        this->hotel = hotel;
        this->room = room;
        this->payment = payment;
        this->checkInDate = checkInDate;
        this->checkOutDate = checkOutDate;

        bookingStatus = BookingStatus::PENDING;
    }

    int getBookingId(){

        return bookingId;
    }

    User* getUser(){

        return user;
    }

    Hotel* getHotel(){

        return hotel;
    }

    Room* getRoom(){

        return room;
    }

    Payment* getPayment(){

        return payment;
    }

    string getCheckInDate(){

        return checkInDate;
    }

    string getCheckOutDate(){

        return checkOutDate;
    }

    BookingStatus getBookingStatus(){

        return bookingStatus;
    }

    void confirmBooking(){

        bookingStatus = BookingStatus::CONFIRMED;
    }

    void cancelBooking(){

        bookingStatus = BookingStatus::CANCELLED;
    }

    void checkIn(){

        bookingStatus = BookingStatus::CHECKED_IN;
        room->checkIn();
    }

    void checkOut(){

        bookingStatus = BookingStatus::CHECKED_OUT;
        room->release();
    }

    double calculateBill(){

        // Assuming one night stay for now
        // Later this can be extended using Date difference

        return room->getPricePerNight();
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

        cout << "Push Notification sent to "
             << user->getName() << endl;
    }

};

//==================== BOOKING SERVICE ====================//

class BookingService{

private:

    NotificationService notificationService;

public:

    Hotel* searchHotel(vector<Hotel*>& hotels,
                       string location){

        for(auto hotel : hotels){

            if(hotel->getLocation() == location)
                return hotel;
        }

        return nullptr;
    }

    Booking* bookRoom(User* user,
                      Hotel* hotel,
                      int bookingId,
                      int transactionId,
                      string checkInDate,
                      string checkOutDate){

        if(hotel == nullptr){

            cout<<"Hotel Not Found"<<endl;
            return nullptr;
        }

        Room* room = hotel->findAvailableRoom();

        if(room == nullptr){

            cout<<"No Room Available"<<endl;
            return nullptr;
        }

        double amount = room->getPricePerNight();

        Payment* payment =
            new Payment(transactionId,amount);

        if(!payment->pay()){

            cout<<"Payment Failed"<<endl;
            return nullptr;
        }

        Booking* booking =
            new Booking(
                bookingId,
                user,
                hotel,
                room,
                payment,
                checkInDate,
                checkOutDate
            );

        booking->confirmBooking();

        room->book();

        notificationService.sendEmail(user);
        notificationService.sendSMS(user);
        notificationService.sendPushNotification(user);

        cout<<"\nBooking Confirmed\n";

        return booking;
    }

    void checkIn(Booking* booking){

        if(booking == nullptr)
            return;

        booking->checkIn();

        cout<<"Check-In Successful"<<endl;
    }

    void checkOut(Booking* booking){

        if(booking == nullptr)
            return;

        booking->checkOut();

        cout<<"Check-Out Successful"<<endl;
    }

    void cancelBooking(Booking* booking){

        if(booking == nullptr)
            return;

        booking->cancelBooking();

        booking->getRoom()->release();

        cout<<"Booking Cancelled"<<endl;
    }

    void displayBooking(Booking* booking){

        if(booking == nullptr){

            cout<<"Booking Not Found"<<endl;
            return;
        }

        cout<<"\n========== BOOKING ==========\n";

        cout<<"Booking Id : "
            <<booking->getBookingId()<<endl;

        cout<<"Guest      : "
            <<booking->getUser()->getName()<<endl;

        cout<<"Hotel      : "
            <<booking->getHotel()->getHotelName()<<endl;

        cout<<"Room No    : "
            <<booking->getRoom()->getRoomNumber()<<endl;

        cout<<"Amount     : "
            <<booking->getPayment()->getAmount()<<endl;

        cout<<"Check In   : "
            <<booking->getCheckInDate()<<endl;

        cout<<"Check Out  : "
            <<booking->getCheckOutDate()<<endl;

        cout<<"=============================\n";
    }

};

int main(){

    //================ CREATE HOTELS ================//

    vector<Hotel*> hotels;

    Hotel* taj = new Hotel(
        1,
        "Taj Hotel",
        "Bangalore"
    );

    //================ ADD ROOMS ================//

    taj->addRoom(new Room(101,RoomType::SINGLE,3000));
    taj->addRoom(new Room(102,RoomType::DOUBLE,4500));
    taj->addRoom(new Room(201,RoomType::DELUXE,6500));
    taj->addRoom(new Room(301,RoomType::SUITE,10000));

    hotels.push_back(taj);

    //================ CREATE USER ================//

    User* user = new User(1,"Urvashi");

    //================ BOOKING SERVICE ================//

    BookingService bookingService;

    //================ SEARCH HOTEL ================//

    Hotel* hotel =
        bookingService.searchHotel(
            hotels,
            "Bangalore"
        );

    if(hotel == nullptr){

        cout<<"Hotel Not Found"<<endl;
        return 0;
    }

    cout<<"Hotel Found : "
        <<hotel->getHotelName()<<endl;

    //================ BOOK ROOM ================//

    Booking* booking =
        bookingService.bookRoom(
            user,
            hotel,
            1001,
            5001,
            "10-08-2026",
            "12-08-2026"
        );

    if(booking == nullptr)
        return 0;

    //================ DISPLAY BOOKING ================//

    bookingService.displayBooking(booking);

    //================ CHECK-IN ================//

    cout<<"\nGuest Checking In...\n";

    bookingService.checkIn(booking);

    //================ CHECK-OUT ================//

    cout<<"\nGuest Checking Out...\n";

    bookingService.checkOut(booking);

    //================ BOOK AGAIN ================//

    cout<<"\nTrying to Book Again...\n";

    Booking* booking2 =
        bookingService.bookRoom(
            user,
            hotel,
            1002,
            5002,
            "15-08-2026",
            "17-08-2026"
        );

    bookingService.displayBooking(booking2);

    //================ CANCEL BOOKING ================//

    cout<<"\nCancelling Second Booking...\n";

    bookingService.cancelBooking(booking2);

    return 0;
}
