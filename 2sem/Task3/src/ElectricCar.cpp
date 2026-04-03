#include "../include/ElectricCar.h"
#include <iostream>

ElectricCar::ElectricCar(const std::string& model, int year, double batteryCapacity, double maxBatteryWeight)
    : Transport(model, year), batteryCapacity(batteryCapacity), maxBatteryWeight(maxBatteryWeight) {}

ElectricCar::~ElectricCar() {}

double ElectricCar::calculateMaxCargoWeight() const {
    double baseCapacity = 500.0; 
    double result = (baseCapacity - (maxBatteryWeight * 0.8)) / 1000.0;
    return result > 0 ? result : 0.1; 
}

void ElectricCar::displayInfo() const {
    Transport::displayInfo();
    std::cout << ", Type: Electric Car, Battery: " << batteryCapacity << "kWh"
              << ", Battery weight: " << maxBatteryWeight << "kg";
}
