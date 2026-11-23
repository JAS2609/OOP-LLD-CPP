#include <iostream>
#include <string>

using namespace std;
class Computer{
    private:
    string cpu;
    string gpu;
    string ram;
    string storage;
    public:
    Computer(const string& cpu, const string& gpu, const string& ram, const string& storage)
    : cpu(cpu), gpu(gpu), ram(ram), storage(storage) {}
    void displaySpecs() const{
        cout << "CPU: " << cpu << endl;
        cout << "GPU: " << gpu << endl;
        cout << "RAM: " << ram << endl;
        cout << "Storage: " << storage << endl;
    }
};
class ComputerBuilder{
    private:
    string cpu;
    string gpu;
    string ram;
    string storage;
    public:
    ComputerBuilder& setCPU(const string& cpu){
        this->cpu = cpu;
        return *this;
    }
    ComputerBuilder& setGPU(const string& gpu){
        this->gpu = gpu;
        return *this;
    }
    ComputerBuilder& setRAM(const string& ram){
        this->ram = ram;
        return *this;
    }
    ComputerBuilder& setStorage(const string& storage){
        this->storage = storage;
        return *this;
    }
    Computer build() const{
        return Computer(cpu, gpu, ram, storage);
    }
};