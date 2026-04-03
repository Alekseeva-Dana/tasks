#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>

class Transport {
protected:
    std::string model;
    int year;
    
public:
    Transport(const std::string& model, int year);
    virtual ~Transport();
    
    virtual double calculateMaxCargoWeight() const = 0;
    
    virtual void displayInfo() const;
    
    /* геттеры*/
    std::string getModel() const { return model; }
    int getYear() const { return year; }
};

#endif 
