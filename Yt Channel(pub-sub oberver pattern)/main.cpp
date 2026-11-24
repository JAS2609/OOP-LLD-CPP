#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Observer {
public:
    virtual void update(const string& message) = 0;
    virtual ~Observer() = default;
};
class Subscriber : public Observer {
private:
    string name;
public:
    Subscriber(const string& name) : name(name) {}

    void update(const string& message) override {
        cout << "Subscriber " << name << " received message: " << message << endl;
    }
};


class Publisher {
private:
    vector<Observer*> observers;

public:
    void subscribe(Observer* observer) {
    if (observer &&
        find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
    }
}

    void unsubscribe(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const string& message) {
    auto snapshot = observers;

    for (Observer* observer : snapshot) {
        observer->update(message);
    }
}

};
