#include "../include/Truck.h"
#include <iostream>
#include <algorithm>

Truck::Truck(const std::string& model, int year, double cargoBayVolume, double maxPayload)
    : Transport(model, year), cargoBayVolume(cargoBayVolume), maxPayload(maxPayload) {}

Truck::~Truck() {}

double Truck::calculateMaxCargoWeight() const {
    double volumeCapacity = cargoBayVolume * 0.5;
    return std::min(volumeCapacity, maxPayload);
}

void Truck::displayInfo() const {
    Transport::displayInfo();
    std::cout << ", Type: Truck, Cargo volume: " << cargoBayVolume << "m³"
              << ", Max payload: " << maxPayload << "t";
}
