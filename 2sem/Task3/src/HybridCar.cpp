#include "../include/HybridCar.h"
#include <iostream>

HybridCar::HybridCar(const std::string& model, int year, int passengerCapacity, double trunkVolume,
                     double batteryCapacity, double maxBatteryWeight, double fuelTankVolume)
    : Transport(model, year),
      Car(model, year, passengerCapacity, trunkVolume),
      ElectricCar(model, year, batteryCapacity, maxBatteryWeight),
      fuelTankVolume(fuelTankVolume) {}

HybridCar::~HybridCar() {}

double HybridCar::calculateMaxCargoWeight() const {
    double carWeight = Car::calculateMaxCargoWeight();
    double electricWeight = ElectricCar::calculateMaxCargoWeight();
    return std::min(carWeight, electricWeight) * 0.9; 
}

void HybridCar::displayInfo() const {
    Transport::displayInfo();
    std::cout << ", Type: Hybrid Car, Passengers: " << passengerCapacity 
              << ", Trunk: " << trunkVolume << "L"
              << ", Battery: " << batteryCapacity << "kWh"
              << ", Fuel tank: " << fuelTankVolume << "L";
}
