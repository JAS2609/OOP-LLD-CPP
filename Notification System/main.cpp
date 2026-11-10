#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;


class Notification{
public:
    virtual void send(const string& message) = 0;
    virtual ~Notification() = default;
};
class EmailNotification : public Notification{
    private:
    string receiverEmail;
public:
    EmailNotification(string email) : receiverEmail(email) {}
    void send(const string& message) override {
        cout << "Sending email notification: " << message << " to " << receiverEmail << endl;
    }
};
class SMSNotification : public Notification{
private:
    string receiverPhone;
public:
    SMSNotification(string phone) : receiverPhone(phone) {}
    void send(const string& message) override{
        cout<<"Sending sms notification: "<<message <<" to "<< receiverPhone<<endl;   
     } 
};
class PushNotification:public Notification{
    private:
    string deviceToken;
    public:
    PushNotification(string token) : deviceToken(token) {}
    void send(const string& message) override{
        cout<<"Sending push notification: "<<message<<" to "<<deviceToken<<endl;
    }
};

class NotificationService{
    public:
    void sendNotification(Notification& notif, const string& message){
        notif.send(message);
    }
    void sendAll(const vector<unique_ptr<Notification>>& notifications,const string& message) {
        for (const auto& notification : notifications) {
            if (notification) {
                notification->send(message);
            }
        }
    }
};
int main() {
    NotificationService service;

    EmailNotification email("test@gmail.com");
    SMSNotification sms("9876543210");
    PushNotification push("DEVICE_TOKEN_123");

    service.sendNotification(email, "Welcome!");
    service.sendNotification(sms, "Your OTP is 1234");
    service.sendNotification(push, "You have a new message");



    vector<unique_ptr<Notification>> notifications;
    notifications.push_back( make_unique<EmailNotification>("alice@example.com"));
    notifications.push_back(make_unique<SMSNotification>("9999999999"));
    notifications.push_back( make_unique<PushNotification>("DEVICE_TOKEN_456"));
    cout << "\nNotifications stored in vector:\n";
    service.sendAll(notifications, "Your order has been shipped!");
    return 0;

}
