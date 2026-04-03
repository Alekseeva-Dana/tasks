#ifndef CAR_H
#define CAR_H

#include "Transport.h"

class Car : virtual public Transport {
protected:
    int passengerCapacity;
    double trunkVolume; /*в литрах*/
    
public:
    Car(const std::string& model, int year, int passengerCapacity, double trunkVolume);
    virtual ~Car();
    
    double calculateMaxCargoWeight() const override;
    void displayInfo() const override;
    
    int getPassengerCapacity() const { return passengerCapacity; }
    double getTrunkVolume() const { return trunkVolume; }
};

#endif 
