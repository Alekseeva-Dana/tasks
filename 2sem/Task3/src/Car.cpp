#include "../include/Car.h"
#include <iostream>

Car::Car(const std::string& model, int year, int passengerCapacity, double trunkVolume)
    : Transport(model, year), passengerCapacity(passengerCapacity), trunkVolume(trunkVolume) {}

Car::~Car() {}

double Car::calculateMaxCargoWeight() const {
    double passengerWeight = passengerCapacity * 75.0; 
    double trunkCapacity = trunkVolume * 30.0; 
    return (passengerWeight + trunkCapacity) / 1000.0; 
}

void Car::displayInfo() const {
    Transport::displayInfo();
    std::cout << ", Type: Car, Passengers: " << passengerCapacity 
              << ", Trunk volume: " << trunkVolume << "L";
}
