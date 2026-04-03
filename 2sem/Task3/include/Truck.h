#ifndef TRUCK_H
#define TRUCK_H

#include "Transport.h"

class Truck : virtual public Transport {
protected:
    double cargoBayVolume; 
    double maxPayload;    
    
public:
    Truck(const std::string& model, int year, double cargoBayVolume, double maxPayload);
    virtual ~Truck();
    
    double calculateMaxCargoWeight() const override;
    void displayInfo() const override;
    
    double getCargoBayVolume() const { return cargoBayVolume; }
    double getMaxPayload() const { return maxPayload; }
};

#endif 
