#ifndef HYBRIDCAR_H
#define HYBRIDCAR_H

#include "Car.h"
#include "ElectricCar.h"

/* используем множественное наследование*/
class HybridCar : public Car, public ElectricCar {
private:
    double fuelTankVolume; 
    
public:
    HybridCar(const std::string& model, int year, int passengerCapacity, double trunkVolume,
              double batteryCapacity, double maxBatteryWeight, double fuelTankVolume);
    virtual ~HybridCar();
    
    double calculateMaxCargoWeight() const override;
    void displayInfo() const override;
    
    double getFuelTankVolume() const { return fuelTankVolume; }
};

#endif 
