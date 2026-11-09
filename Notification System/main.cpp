#include <iostream>
#include <string>
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
};
int main() {
    NotificationService service;

    EmailNotification email("test@gmail.com");
    SMSNotification sms("9876543210");
    PushNotification push("DEVICE_TOKEN_123");

    service.sendNotification(email, "Welcome!");
    service.sendNotification(sms, "Your OTP is 1234");
    service.sendNotification(push, "You have a new message");
    return 0;

}
