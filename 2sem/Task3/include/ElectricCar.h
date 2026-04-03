#ifndef ELECTRICCAR_H
#define ELECTRICCAR_H

#include "Transport.h"

class ElectricCar : virtual public Transport {
protected:
    double batteryCapacity; 
    double maxBatteryWeight; 
    
public:
    ElectricCar(const std::string& model, int year, double batteryCapacity, double maxBatteryWeight);
    virtual ~ElectricCar();
    
    double calculateMaxCargoWeight() const override;
    void displayInfo() const override;
    
    double getBatteryCapacity() const { return batteryCapacity; }
    double getMaxBatteryWeight() const { return maxBatteryWeight; }
};

#endif 
