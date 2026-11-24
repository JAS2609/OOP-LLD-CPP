#include<iostream>
#include<string>
using namespace std;

class PaymentProcessor {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentProcessor() = default;
};

//third party api
class PayPalAPI {
public:
    void executeTransaction(const string& email, double amount) {
        cout << "PayPal transaction: " << amount;
    }
};
class PayPalAdapter : public PaymentProcessor {
private:
    PayPalAPI& paypal;
    string email;
public:
    PayPalAdapter(PayPalAPI& paypal, const string& email) : paypal(paypal), email(email) {}

    void pay(double amount) override {
        paypal.executeTransaction(email, amount);
    }
};