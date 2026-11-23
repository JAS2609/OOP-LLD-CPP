#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;


class Payment{
public:
    virtual void process(const double& amount) = 0;
    virtual ~Payment() = default;
};
class CreditCardPayment : public Payment{
    private:
    string cardNumber;
public:
    explicit CreditCardPayment(const string& cardNumber)
    : cardNumber(cardNumber) {}
    void process(const double& amount) override{
        cout << "Processing credit card payment of $" << amount << " using card number: " << cardNumber << endl;
    };
};
class PayPalPayment : public Payment{
private:
    string email;
public:
    explicit PayPalPayment(const string& email)
    : email(email) {}
    void process(const double& amount) override{
        cout << "Processing PayPal payment of $" << amount << " using email: " << email << endl;
    };
};
class UPIpayment : public Payment{
    private:
    string upiId;
public:
    explicit UPIpayment(const string& upiId)
    : upiId(upiId) {}
    void process(const double& amount) override{
        cout << "Processing UPI payment of $" << amount << " using UPI ID: " << upiId << endl;
    };
};
class PaymentCreator{
    public:
    virtual unique_ptr<Payment> createPayment() = 0;
    virtual ~PaymentCreator() = default;
};
class CreditCardPaymentCreator : public PaymentCreator{
    private:
    string cardNumber;
public:
    explicit CreditCardPaymentCreator(const string& cardNumber)
    : cardNumber(cardNumber) {}
    unique_ptr<Payment> createPayment() override{
        return make_unique<CreditCardPayment>(cardNumber);
    };
};
class PayPalPaymentCreator : public PaymentCreator{
    private:
    string email;
public:
    explicit PayPalPaymentCreator(const string& email)
    : email(email) {}
    unique_ptr<Payment> createPayment() override{
        return make_unique<PayPalPayment>(email);
    };  
};
class UPIpaymentCreator : public PaymentCreator{
    private:
    string upiId;
public:
    explicit UPIpaymentCreator(const string& upiId)
    : upiId(upiId) {}
    unique_ptr<Payment> createPayment() override{
        return make_unique<UPIpayment>(upiId);
    };  
};
class PaymentService{
    public:
    void processPayment(PaymentCreator& creator, const double& amount){
        auto payment = creator.createPayment();
        payment->process(amount);
    }
};