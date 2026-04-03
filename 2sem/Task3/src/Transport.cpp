#include "../include/Transport.h"
#include <iostream>

Transport::Transport(const std::string& model, int year)
    : model(model), year(year) {}

Transport::~Transport() {}

void Transport::displayInfo() const {
    std::cout << "Model: " << model << ", Year: " << year;
}
