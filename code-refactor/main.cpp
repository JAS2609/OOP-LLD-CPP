/*class OrderService {
public:
    void processOrder(string paymentType, double amount) {

        // Calculate total
        double total = amount + (amount * 0.18);

        // Payment processing
        if (paymentType == "UPI") {
            cout << "Processing UPI payment\n";
        }
        else if (paymentType == "CARD") {
            cout << "Processing Card payment\n";
        }

        // Save order
        cout << "Saving order to MySQL\n";

        // Generate invoice
        cout << "Generating PDF invoice\n";

        // Send notification
        cout << "Sending email notification\n";
    }
};*/

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Order {
private:
    string orderID;
    double amount;

public:
    Order(const string& id, double amt)
        : orderID(id), amount(amt) {
        if (amt < 0)
            throw invalid_argument("Amount cannot be negative");
    }

    double calculateTotal() const {
        return amount * 1.18;
    }
};

class Payment {
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~Payment() = default;
};

class UpiPayment : public Payment {
public:
    bool processPayment(double amount) override {
        cout << "Processing UPI payment: " << amount << endl;
        return true;
    }
};

class CardPayment : public Payment {
public:
    bool processPayment(double amount) override {
        cout << "Processing Card payment: " << amount << endl;
        return true;
    }
};
class Database {
public:
    virtual bool saveOrder(const Order& order) = 0;
    virtual ~Database() = default;
};
class MySQLDatabase : public Database {
public:
    bool saveOrder(const Order& order) override {
        cout << "Saving order to MySQL database\n";
        return true;
    }
};
class MongoDBDatabase : public Database {
public:
    bool saveOrder(const Order& order) override {
        cout << "Saving order to MongoDB database\n";
        return true;
    }
};
class InvoiceGenerator {
public:
    void generateInvoice(const Order& order) {
        cout << "Generating PDF invoice for order\n";
    }
};
class NotificationService {
public:
    virtual void sendNotification(const Order& order) = 0;
    virtual ~NotificationService() = default;
};
class EmailNotification : public NotificationService {
public:
    void sendNotification(const Order& order) override {
        cout << "Sending email notification for order\n";
    }
};
class SMSNotification : public NotificationService {
public:
    void sendNotification(const Order& order) override {
        cout << "Sending SMS notification for order\n";
    }
};
class WhatsappNotification : public NotificationService {
public:
    void sendNotification(const Order& order) override {
        cout << "Sending WhatsApp notification for order\n";
    }
};
class OrderService {
private:
    Payment& payment;
    Database& database;
    InvoiceGenerator& invoice;
    NotificationService& notification;
public:
    OrderService (Payment& pay, Database& db, InvoiceGenerator& inv, NotificationService& notif)
        : payment(pay), database(db), invoice(inv), notification(notif) {}
    bool processOrder(const Order& order) {
        double total = order.calculateTotal();
        if (!payment.processPayment(total)) {
            cout << "Payment failed\n";
            return false;
        }
        if (!database.saveOrder(order)) {
            cout << "Failed to save order\n";
            return false;
        }
        invoice.generateInvoice(order);
        notification.sendNotification(order);
        return true;
    }
};