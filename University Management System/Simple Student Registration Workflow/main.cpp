
#include <iostream>
#include <string>
#include <memory>
#include <utility>
using namespace std;

class Student {
private:
    int studentId;
    string name;

public:
    Student(int id, const string& studentName)
        : studentId(id), name(studentName) {}

    int getStudentId() const {
        return studentId;
    }

    const string& getName() const {
        return name;
    }
};

class Database {
public:
    virtual void saveStudent(const Student& student) = 0;
    virtual ~Database() = default;
};

class MySQLDatabase : public Database {
public:
    void saveStudent(const Student& student) override {
        cout << "Saving to MySQL: "
             << student.getName() << endl;
    }
};

class PostgreSQLDatabase : public Database {
public:
    void saveStudent(const Student& student) override {
        cout << "Saving to PostgreSQL: "
             << student.getName() << endl;
    }
};

class StudentService {
private:
    unique_ptr<Database> db;

public:
    explicit StudentService(unique_ptr<Database> database)
        : db(move(database)) {
        if (!db) {
            throw invalid_argument("Database cannot be null");
        }
    }

    void registerStudent(const Student& student) {
        db->saveStudent(student);
    }
};

int main() {
    auto database = make_unique<MySQLDatabase>();

    StudentService service(move(database));

    Student student(101, "Jas");
    service.registerStudent(student);

    return 0;
}
