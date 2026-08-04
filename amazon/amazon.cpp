#include <bits/stdc++.h>
using namespace std;

// ===================== ENUMS =====================

enum class ProductCategory {
    ELECTRONICS,
    GROCERY,
    CLOTHING
};

enum class OrderStatus {
    CREATED,
    SHIPPED,
    DELIVERED
};

enum class PaymentStatus {
    PENDING,
    SUCCESS,
    FAILED
};

// ===================== USER =====================

class User {

private:
    int userId;
    string name;

public:

    User(int userId, string name) {
        this->userId = userId;
        this->name = name;
    }

    int getUserId() {
        return userId;
    }

    string getName() {
        return name;
    }
};

// ===================== PRODUCT =====================

class Product {

private:

    int productId;
    string name;
    ProductCategory category;
    double price;

public:

    Product(
        int productId,
        string name,
        ProductCategory category,
        double price
    ) {

        this->productId = productId;
        this->name = name;
        this->category = category;
        this->price = price;
    }

    int getProductId() {
        return productId;
    }

    string getName() {
        return name;
    }

    ProductCategory getCategory() {
        return category;
    }

    double getPrice() {
        return price;
    }
};

// ===================== CART ITEM =====================

class CartItem {

private:

    Product* product;
    int quantity;

public:

    CartItem(Product* product, int quantity) {

        this->product = product;
        this->quantity = quantity;
    }

    Product* getProduct() {
        return product;
    }

    int getQuantity() {
        return quantity;
    }

    void updateQuantity(int quantity) {
        this->quantity = quantity;
    }

    double getTotalPrice() {
        return quantity * product->getPrice();
    }
};

// ===================== CART =====================

class Cart {

private:

    vector<CartItem*> items;

public:

    void addProduct(Product* product, int quantity) {

        for (CartItem* item : items) {

            if (item->getProduct() == product) {

                item->updateQuantity(
                    item->getQuantity() + quantity
                );

                return;
            }
        }

        items.push_back(
            new CartItem(product, quantity)
        );
    }

    void removeProduct(Product* product) {

        for (auto it = items.begin(); it != items.end(); it++) {

            if ((*it)->getProduct() == product) {

                delete *it;
                items.erase(it);
                return;
            }
        }
    }

    void updateQuantity(Product* product, int quantity) {

        for (CartItem* item : items) {

            if (item->getProduct() == product) {

                item->updateQuantity(quantity);
                return;
            }
        }
    }

    double getTotalPrice() {

        double total = 0;

        for (CartItem* item : items) {

            total += item->getTotalPrice();
        }

        return total;
    }

    vector<CartItem*> getItems() {
        return items;
    }

    ~Cart() {

        for (CartItem* item : items) {
            delete item;
        }
    }
};

// ===================== PAYMENT =====================

class Payment {

private:

    int transactionId;
    double amount;
    PaymentStatus status;

public:

    Payment(int transactionId, double amount) {

        this->transactionId = transactionId;
        this->amount = amount;
        this->status = PaymentStatus::PENDING;
    }

    int getTransactionId() {
        return transactionId;
    }

    double getAmount() {
        return amount;
    }

    PaymentStatus getStatus() {
        return status;
    }

    void pay() {

        // Simulate successful payment
        status = PaymentStatus::SUCCESS;

        cout << "Payment Successful\n";
    }
};



// ===================== ORDER ITEM =====================

class OrderItem {

private:

    Product* product;

    int quantity;

    double priceAtPurchase;

public:

    OrderItem(Product* product, int quantity) {

        this->product = product;
        this->quantity = quantity;

        // Store price at purchase time
        this->priceAtPurchase = product->getPrice();
    }

    Product* getProduct() {
        return product;
    }

    int getQuantity() {
        return quantity;
    }

    double getPriceAtPurchase() {
        return priceAtPurchase;
    }

    double getTotalPrice() {

        return quantity * priceAtPurchase;
    }
};



// ===================== ORDER =====================

class Order {

private:

    int orderId;

    vector<OrderItem*> items;

    Payment* payment;

    OrderStatus status;

public:

    Order(int orderId) {

        this->orderId = orderId;

        payment = nullptr;

        status = OrderStatus::CREATED;
    }

    int getOrderId() {

        return orderId;
    }

    void addOrderItem(OrderItem* item) {

        items.push_back(item);
    }

    vector<OrderItem*> getOrderItems() {

        return items;
    }

    double getTotalAmount() {

        double total = 0;

        for (OrderItem* item : items) {

            total += item->getTotalPrice();
        }

        return total;
    }

    void setPayment(Payment* payment) {

        this->payment = payment;
    }

    Payment* getPayment() {

        return payment;
    }

    OrderStatus getOrderStatus() {

        return status;
    }

    void updateOrderStatus(OrderStatus status) {

        this->status = status;
    }

    ~Order() {

        for (OrderItem* item : items) {
            delete item;
        }

        if (payment != nullptr) {
            delete payment;
        }
    }
};

// ===================== INVENTORY =====================

class Inventory {

private:

    unordered_map<int, int> stock;

public:

    void addStock(Product* product, int quantity) {

        stock[product->getProductId()] += quantity;
    }

    bool isAvailable(Product* product, int quantity) {

        return stock[product->getProductId()] >= quantity;
    }

    void reduceStock(Product* product, int quantity) {

        if (isAvailable(product, quantity)) {

            stock[product->getProductId()] -= quantity;
        }
    }

    int getAvailableQuantity(Product* product) {

        return stock[product->getProductId()];
    }
};



// ===================== AMAZON =====================

class Amazon {

private:

    vector<Product*> products;

    Inventory* inventory;

    unordered_map<int, Order*> orders;

    int nextOrderId;

    int nextTransactionId;

public:

    Amazon() {

        inventory = new Inventory();

        nextOrderId = 1;

        nextTransactionId = 1001;
    }

    ~Amazon() {

        for (Product* product : products)
            delete product;

        for (auto entry : orders)
            delete entry.second;

        delete inventory;
    }

    // ---------------- Product ----------------

    void addProduct(Product* product, int quantity) {

        products.push_back(product);

        inventory->addStock(product, quantity);
    }

    vector<Product*> searchProduct(string name) {

        vector<Product*> result;

        for (Product* product : products) {

            if (product->getName() == name) {

                result.push_back(product);
            }
        }

        return result;
    }

    // ---------------- Order ----------------

    Order* placeOrder(Cart* cart) {

        Order* order = new Order(nextOrderId++);

        for (CartItem* item : cart->getItems()) {

            Product* product = item->getProduct();

            int quantity = item->getQuantity();

            if (!inventory->isAvailable(product, quantity)) {

                cout << product->getName()
                     << " Out Of Stock\n";

                delete order;

                return nullptr;
            }

            inventory->reduceStock(product, quantity);

            OrderItem* orderItem =
                new OrderItem(product, quantity);

            order->addOrderItem(orderItem);
        }

        orders[order->getOrderId()] = order;

        cout << "Order Created Successfully\n";

        return order;
        
    }

    // ---------------- Payment ----------------

    void makePayment(Order* order) {

        Payment* payment =
            new Payment(
                nextTransactionId++,
                order->getTotalAmount()
            );

        payment->pay();

        order->setPayment(payment);
    }

    // ---------------- Track ----------------

    void trackOrder(int orderId) {

        if (orders.find(orderId) == orders.end()) {

            cout << "Order Not Found\n";

            return;
        }

        Order* order = orders[orderId];

        switch (order->getOrderStatus()) {

            case OrderStatus::CREATED:

                cout << "CREATED\n";
                break;

            case OrderStatus::SHIPPED:

                cout << "SHIPPED\n";
                break;

            case OrderStatus::DELIVERED:

                cout << "DELIVERED\n";
                break;
        }
    }

    void updateOrderStatus(
        int orderId,
        OrderStatus status
    ) {

        if (orders.find(orderId) != orders.end()) {

            orders[orderId]->updateOrderStatus(status);
        }
    }

    Inventory* getInventory() {

        return inventory;
    }
};

int main() {

    // ================= Create Amazon =================

    Amazon* amazon = new Amazon();


    // ================= Create Products =================

    Product* laptop =
        new Product(
            1,
            "MacBook Pro",
            ProductCategory::ELECTRONICS,
            200000
        );

    Product* iphone =
        new Product(
            2,
            "iPhone 16",
            ProductCategory::ELECTRONICS,
            100000
        );


    // ================= Add Products to Inventory =================

    amazon->addProduct(laptop, 10);

    amazon->addProduct(iphone, 20);


    // ================= Create User =================

    User* user =
        new User(
            101,
            "Rahul"
        );


    // ================= Search Product =================

    cout << "Search Result\n";

    vector<Product*> result =
        amazon->searchProduct("MacBook Pro");

    for (Product* product : result) {

        cout << product->getName()
             << "  Price : "
             << product->getPrice()
             << endl;
    }


    // ================= Create Cart =================

    Cart* cart = new Cart();


    // ================= Add Products =================

    cart->addProduct(laptop, 2);

    cart->addProduct(iphone, 1);


    cout << "\nCart Total : "
         << cart->getTotalPrice()
         << endl;


    // ================= Place Order =================

    Order* order =
        amazon->placeOrder(cart);

    if (order == nullptr) {

        cout << "Order Failed\n";

        delete cart;
        delete user;
        delete amazon;

        return 0;
    }


    cout << "\nOrder Id : "
         << order->getOrderId()
         << endl;

    cout << "Total Amount : "
         << order->getTotalAmount()
         << endl;


    // ================= Payment =================

    amazon->makePayment(order);


    // ================= Track Order =================

    cout << "\nCurrent Status : ";

    amazon->trackOrder(
        order->getOrderId()
    );


    // ================= Update Status =================

    amazon->updateOrderStatus(
        order->getOrderId(),
        OrderStatus::SHIPPED
    );


    cout << "After Shipping : ";

    amazon->trackOrder(
        order->getOrderId()
    );


    amazon->updateOrderStatus(
        order->getOrderId(),
        OrderStatus::DELIVERED
    );


    cout << "After Delivery : ";

    amazon->trackOrder(
        order->getOrderId()
    );


    // ================= Cleanup =================

    delete cart;

    delete user;

    delete amazon;

    return 0;
}