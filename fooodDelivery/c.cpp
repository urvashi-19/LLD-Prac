#include <bits/stdc++.h>
using namespace std;

/* =========================================================
   ENUMS
   ========================================================= */

enum class OrderStatus {
    CREATED,
    PAYMENT_FAILED,
    PLACED,
    ACCEPTED,
    PREPARING,
    READY,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

enum class PaymentStatus {
    INITIATED,
    SUCCESS,
    FAILED,
    REFUND_INITIATED,
    REFUNDED
};

enum class DeliveryStatus {
    NOT_ASSIGNED,
    ASSIGNED,
    PICKED_UP,
    DELIVERED,
    CANCELLED
};


/* =========================================================
   LOCATION
   ========================================================= */

class Location {
private:
    double x;
    double y;

public:
    Location(double x, double y) : x(x), y(y) {}

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }
};


/* =========================================================
   USER
   ========================================================= */

class User {
private:
    int id;
    string name;
    Location location;

public:
    User(int id, string name, Location location)
        : id(id), name(name), location(location) {}

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    Location getLocation() {
        return location;
    }
};


/* =========================================================
   MENU ITEM
   ========================================================= */

class MenuItem {
private:
    int id;
    string name;
    double price;

public:
    MenuItem(int id, string name, double price)
        : id(id), name(name), price(price) {}

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    double getPrice() {
        return price;
    }

    void setPrice(double price) {
        this->price = price;
    }
};


/* =========================================================
   MENU
   ========================================================= */

class Menu {
private:
    unordered_map<int, MenuItem*> items;

public:
    void addItem(MenuItem* item) {
        items[item->getId()] = item;
    }

    MenuItem* getItem(int id) {

        if (items.find(id) == items.end())
            return nullptr;

        return items[id];
    }

    void showMenu() {

        cout << "\n--- MENU ---\n";

        for (auto& entry : items) {

            MenuItem* item = entry.second;

            cout << item->getId()
                 << " : "
                 << item->getName()
                 << " : Rs "
                 << item->getPrice()
                 << endl;
        }
    }
};


/* =========================================================
   RESTAURANT
   ========================================================= */

class Restaurant {
private:
    int id;
    string name;
    Location location;
    Menu* menu;

public:
    Restaurant(
        int id,
        string name,
        Location location,
        Menu* menu
    )
        : id(id),
          name(name),
          location(location),
          menu(menu) {}

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    Location getLocation() {
        return location;
    }

    Menu* getMenu() {
        return menu;
    }
};


/* =========================================================
   CART ITEM
   ========================================================= */

class CartItem {
private:
    MenuItem* menuItem;
    int quantity;

public:
    CartItem(
        MenuItem* menuItem,
        int quantity
    )
        : menuItem(menuItem),
          quantity(quantity) {}

    MenuItem* getMenuItem() {
        return menuItem;
    }

    int getQuantity() {
        return quantity;
    }

    void setQuantity(int quantity) {
        this->quantity = quantity;
    }

    double getTotalPrice() {
        return menuItem->getPrice() * quantity;
    }
};


/* =========================================================
   CART
   ========================================================= */

class Cart {
private:
    int id;
    vector<CartItem*> items;

public:

    Cart(int id) : id(id) {}

    void addItem(
        MenuItem* menuItem,
        int quantity
    ) {

        for (CartItem* item : items) {

            if (item->getMenuItem()->getId()
                == menuItem->getId()) {

                item->setQuantity(
                    item->getQuantity() + quantity
                );

                return;
            }
        }

        items.push_back(
            new CartItem(
                menuItem,
                quantity
            )
        );
    }

    vector<CartItem*>& getItems() {
        return items;
    }

    double getTotalPrice() {

        double total = 0;

        for (CartItem* item : items) {
            total += item->getTotalPrice();
        }

        return total;
    }

    void showCart() {

        cout << "\n--- CART ---\n";

        for (CartItem* item : items) {

            cout << item->getMenuItem()->getName()
                 << " x "
                 << item->getQuantity()
                 << " = Rs "
                 << item->getTotalPrice()
                 << endl;
        }

        cout << "Total = Rs "
             << getTotalPrice()
             << endl;
    }
};


/* =========================================================
   ORDER ITEM
   ========================================================= */

class OrderItem {
private:
    int id;
    MenuItem* menuItem;
    int quantity;

    // Important: snapshot price at purchase time
    double priceAtPurchase;

public:

    OrderItem(
        int id,
        MenuItem* menuItem,
        int quantity
    )
        : id(id),
          menuItem(menuItem),
          quantity(quantity) {

        priceAtPurchase =
            menuItem->getPrice();
    }

    double getTotalPrice() {

        return priceAtPurchase * quantity;
    }

    MenuItem* getMenuItem() {
        return menuItem;
    }

    int getQuantity() {
        return quantity;
    }
};


/* =========================================================
   OBSERVER
   ========================================================= */

class OrderObserver {

public:

    virtual void update(
        OrderStatus status
    ) = 0;

    virtual ~OrderObserver() {}
};


/* =========================================================
   CUSTOMER NOTIFICATION
   ========================================================= */

class CustomerNotification
    : public OrderObserver {

private:
    User* user;

public:

    CustomerNotification(User* user)
        : user(user) {}

    void update(
        OrderStatus status
    ) override {

        cout << "\nNotification to "
             << user->getName()
             << " : ";

        switch (status) {

            case OrderStatus::PLACED:
                cout << "Order placed";
                break;

            case OrderStatus::ACCEPTED:
                cout << "Restaurant accepted order";
                break;

            case OrderStatus::PREPARING:
                cout << "Order is being prepared";
                break;

            case OrderStatus::READY:
                cout << "Order is ready";
                break;

            case OrderStatus::OUT_FOR_DELIVERY:
                cout << "Order is out for delivery";
                break;

            case OrderStatus::DELIVERED:
                cout << "Order delivered";
                break;

            case OrderStatus::CANCELLED:
                cout << "Order cancelled";
                break;

            default:
                break;
        }

        cout << endl;
    }
};


/* =========================================================
   ORDER
   ========================================================= */

class Order {
private:
    int id;

    User* user;
    Restaurant* restaurant;

    vector<OrderItem*> items;

    double totalAmount;

    OrderStatus status;

    vector<OrderObserver*> observers;

public:

    Order(
        int id,
        User* user,
        Restaurant* restaurant
    )
        : id(id),
          user(user),
          restaurant(restaurant),
          totalAmount(0),
          status(OrderStatus::CREATED) {}

    int getId() {
        return id;
    }

    User* getUser() {
        return user;
    }

    Restaurant* getRestaurant() {
        return restaurant;
    }

    double getTotalAmount() {
        return totalAmount;
    }

    OrderStatus getStatus() {
        return status;
    }

    void addItem(OrderItem* item) {

        items.push_back(item);

        totalAmount +=
            item->getTotalPrice();
    }

    void addObserver(
        OrderObserver* observer
    ) {

        observers.push_back(observer);
    }

    void setStatus(
        OrderStatus newStatus
    ) {

        status = newStatus;

        for (OrderObserver* observer :
             observers) {

            observer->update(status);
        }
    }
};


/* =========================================================
   PAYMENT STRATEGY
   ========================================================= */

class PaymentStrategy {

public:

    virtual bool pay(
        double amount
    ) = 0;

    virtual bool refund(
        double amount
    ) = 0;

    virtual ~PaymentStrategy() {}
};


/* =========================================================
   UPI
   ========================================================= */

class UPIPayment
    : public PaymentStrategy {

public:

    bool pay(double amount) override {

        cout << "\nPaid Rs "
             << amount
             << " using UPI\n";

        return true;
    }

    bool refund(double amount) override {

        cout << "Refunded Rs "
             << amount
             << " using UPI\n";

        return true;
    }
};


/* =========================================================
   CARD
   ========================================================= */

class CardPayment
    : public PaymentStrategy {

public:

    bool pay(double amount) override {

        cout << "\nPaid Rs "
             << amount
             << " using Card\n";

        return true;
    }

    bool refund(double amount) override {

        cout << "Refunded Rs "
             << amount
             << " using Card\n";

        return true;
    }
};


/* =========================================================
   PAYMENT
   ========================================================= */

class Payment {
private:

    double amount;

    PaymentStatus status;

    string idempotencyKey;

    PaymentStrategy* strategy;

public:

    Payment(
        double amount,
        string idempotencyKey,
        PaymentStrategy* strategy
    )
        : amount(amount),
          status(PaymentStatus::INITIATED),
          idempotencyKey(idempotencyKey),
          strategy(strategy) {}

    bool process() {

        if (status == PaymentStatus::SUCCESS) {

            return true;
        }

        bool success =
            strategy->pay(amount);

        if (success) {

            status =
                PaymentStatus::SUCCESS;

            return true;
        }

        status =
            PaymentStatus::FAILED;

        return false;
    }

    bool refund() {

        if (status != PaymentStatus::SUCCESS)
            return false;

        status =
            PaymentStatus::REFUND_INITIATED;

        bool success =
            strategy->refund(amount);

        if (success) {

            status =
                PaymentStatus::REFUNDED;

            return true;
        }

        return false;
    }

    PaymentStatus getStatus() {
        return status;
    }
};


/* =========================================================
   PAYMENT SERVICE
   ========================================================= */

class PaymentService {

private:

    unordered_map<
        string,
        Payment*
    > paymentMap;

public:

    Payment* pay(
        double amount,
        string idempotencyKey,
        PaymentStrategy* strategy
    ) {

        // Duplicate payment request
        if (paymentMap.find(idempotencyKey)
            != paymentMap.end()) {

            cout << "\nDuplicate payment request\n";

            return paymentMap[idempotencyKey];
        }

        Payment* payment =
            new Payment(
                amount,
                idempotencyKey,
                strategy
            );

        payment->process();

        paymentMap[idempotencyKey] =
            payment;

        return payment;
    }

    void refund(
        Payment* payment
    ) {

        payment->refund();
    }
};


/* =========================================================
   DELIVERY PARTNER
   ========================================================= */

class DeliveryPartner {
private:

    int id;
    string name;
    Location location;

    bool available;

public:

    DeliveryPartner(
        int id,
        string name,
        Location location
    )
        : id(id),
          name(name),
          location(location),
          available(true) {}

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    Location getLocation() {
        return location;
    }

    bool isAvailable() {
        return available;
    }

    void setAvailable(
        bool available
    ) {

        this->available =
            available;
    }
};


/* =========================================================
   DELIVERY
   ========================================================= */

class Delivery {

private:

    int id;

    Order* order;

    DeliveryPartner* partner;

    DeliveryStatus status;

public:

    Delivery(
        int id,
        Order* order
    )
        : id(id),
          order(order),
          partner(nullptr),
          status(
              DeliveryStatus::NOT_ASSIGNED
          ) {}

    void assignPartner(
        DeliveryPartner* partner
    ) {

        this->partner =
            partner;

        partner->setAvailable(false);

        status =
            DeliveryStatus::ASSIGNED;

        cout << "\nDelivery partner "
             << partner->getName()
             << " assigned\n";
    }

    void pickup() {

        if (partner == nullptr)
            return;

        status =
            DeliveryStatus::PICKED_UP;

        cout << "Order picked up\n";
    }

    void complete() {

        if (partner == nullptr)
            return;

        status =
            DeliveryStatus::DELIVERED;

        partner->setAvailable(true);

        cout << "Order delivered\n";
    }

    void cancel() {

        status =
            DeliveryStatus::CANCELLED;

        if (partner != nullptr) {

            partner->setAvailable(true);
        }

        cout << "Delivery cancelled\n";
    }
};


/* =========================================================
   DELIVERY SERVICE
   ========================================================= */

class DeliveryService {

private:

    vector<DeliveryPartner*> partners;

public:

    void addPartner(
        DeliveryPartner* partner
    ) {

        partners.push_back(partner);
    }

    Delivery* createDelivery(
        int deliveryId,
        Order* order
    ) {

        Delivery* delivery =
            new Delivery(
                deliveryId,
                order
            );

        /*
            For now simply assign
            the first available partner.

            No Strategy / Factory /
            DriverManager complexity.
        */

        for (DeliveryPartner* partner :
             partners) {

            if (partner->isAvailable()) {

                delivery->assignPartner(
                    partner
                );

                order->setStatus(
                    OrderStatus::OUT_FOR_DELIVERY
                );

                return delivery;
            }
        }

        cout << "\nNo delivery partner available\n";

        return delivery;
    }
};


/* =========================================================
   RESTAURANT SERVICE
   ========================================================= */

class RestaurantService {

public:

    void acceptOrder(
        Order* order
    ) {

        order->setStatus(
            OrderStatus::ACCEPTED
        );
    }

    void rejectOrder(
        Order* order
    ) {

        order->setStatus(
            OrderStatus::CANCELLED
        );
    }

    void startPreparing(
        Order* order
    ) {

        order->setStatus(
            OrderStatus::PREPARING
        );
    }

    void markReady(
        Order* order
    ) {

        order->setStatus(
            OrderStatus::READY
        );
    }
};


/* =========================================================
   ORDER SERVICE
   ========================================================= */

class OrderService {

private:

    int nextOrderId = 1;

    int nextOrderItemId = 1;

public:

    Order* createOrder(
        User* user,
        Restaurant* restaurant,
        Cart* cart
    ) {

        Order* order =
            new Order(
                nextOrderId++,
                user,
                restaurant
            );

        for (CartItem* cartItem :
             cart->getItems()) {

            OrderItem* orderItem =
                new OrderItem(
                    nextOrderItemId++,
                    cartItem->getMenuItem(),
                    cartItem->getQuantity()
                );

            order->addItem(
                orderItem
            );
        }

        return order;
    }

    bool cancelOrder(
        Order* order,
        PaymentService& paymentService,
        Payment* payment
    ) {

        if (order->getStatus()
            == OrderStatus::DELIVERED ||
            order->getStatus()
            == OrderStatus::CANCELLED) {

            cout << "\nCannot cancel order\n";

            return false;
        }

        order->setStatus(
            OrderStatus::CANCELLED
        );

        if (payment != nullptr &&
            payment->getStatus()
                == PaymentStatus::SUCCESS) {

            paymentService.refund(
                payment
            );
        }

        return true;
    }
};


/* =========================================================
   MAIN
   ========================================================= */

int main() {

    // USER

    User* user =
        new User(
            1,
            "Urvashi",
            Location(10, 10)
        );


    // MENU

    Menu* menu =
        new Menu();

    MenuItem* pizza =
        new MenuItem(
            1,
            "Pizza",
            250
        );

    MenuItem* burger =
        new MenuItem(
            2,
            "Burger",
            150
        );

    menu->addItem(pizza);
    menu->addItem(burger);


    // RESTAURANT

    Restaurant* restaurant =
        new Restaurant(
            1,
            "Burger Van",
            Location(5, 5),
            menu
        );


    // CART

    Cart* cart =
        new Cart(1);

    cart->addItem(
        pizza,
        2
    );

    cart->addItem(
        burger,
        1
    );

    cart->showCart();


    // ORDER

    OrderService orderService;

    Order* order =
        orderService.createOrder(
            user,
            restaurant,
            cart
        );


    cout << "\nOrder amount: Rs "
         << order->getTotalAmount()
         << endl;


    // OBSERVER

    CustomerNotification*
        notification =
            new CustomerNotification(user);

    order->addObserver(
        notification
    );


    // PAYMENT

    PaymentService paymentService;

    PaymentStrategy*
        paymentStrategy =
            new UPIPayment();

    Payment* payment =
        paymentService.pay(
            order->getTotalAmount(),
            "ORDER_1_PAYMENT_1",
            paymentStrategy
        );


    if (payment->getStatus()
        != PaymentStatus::SUCCESS) {

        order->setStatus(
            OrderStatus::PAYMENT_FAILED
        );

        return 0;
    }


    // ORDER PLACED

    order->setStatus(
        OrderStatus::PLACED
    );


    // RESTAURANT

    RestaurantService restaurantService;

    restaurantService.acceptOrder(
        order
    );

    restaurantService.startPreparing(
        order
    );

    restaurantService.markReady(
        order
    );


    // DELIVERY

    DeliveryService deliveryService;

    DeliveryPartner* partner =
        new DeliveryPartner(
            1,
            "Rahul",
            Location(7, 7)
        );

    deliveryService.addPartner(
        partner
    );


    Delivery* delivery =
        deliveryService.createDelivery(
            1,
            order
        );


    // DELIVERY FLOW

    delivery->pickup();

    delivery->complete();


    // ORDER COMPLETED

    order->setStatus(
        OrderStatus::DELIVERED
    );


    // CLEANUP

    delete notification;
    delete paymentStrategy;

    delete partner;
    delete delivery;

    delete order;

    delete cart;

    delete pizza;
    delete burger;

    delete menu;
    delete restaurant;

    delete user;

    return 0;
}