#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "../include/Transport.h"
#include "../include/Car.h"
#include "../include/Truck.h"
#include "../include/ElectricCar.h"
#include "../include/HybridCar.h"

using namespace std;

/* тестировочная функция*/
void runPolymorphicTest(const vector<unique_ptr<Transport>>& vehicles) {
    cout << "\n========== TEST ==========" << endl;
    double totalCargoWeight = 0;
    
    for (const auto& vehicle : vehicles) {
        vehicle->displayInfo();
        cout << "\n  -> Max cargo weight: " << vehicle->calculateMaxCargoWeight() << " tons" << endl;
        totalCargoWeight += vehicle->calculateMaxCargoWeight();
    }
    
    cout << "\nTotal cargo capacity of all vehicles: " << totalCargoWeight << " tons" << endl;
    cout << "=======================================\n" << endl;
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showMenu() {
    cout << "\n========== TRANSPORT MANAGER ==========" << endl;
    cout << "1. Add Car" << endl;
    cout << "2. Add Truck" << endl;
    cout << "3. Add Electric Car" << endl;
    cout << "4. Add Hybrid Car" << endl;
    cout << "5. Show all vehicles" << endl;
    cout << "6. Run polymorphic test (calculate total cargo capacity)" << endl;
    cout << "7. Delete a vehicle" << endl;
    cout << "8. Clear all vehicles" << endl;
    cout << "0. Exit" << endl;
    cout << "Choice: ";
}

int main() {
    vector<unique_ptr<Transport>> vehicles;
    int choice;
    
    do {
        showMenu();
        cin >> choice;
        
        if (cin.fail()) {
            clearInput();
            cout << "Invalid input!" << endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                string model;
                int year, passengers;
                double trunkVolume;
                
                cout << "Enter model: ";
                cin >> model;
                cout << "Enter year: ";
                cin >> year;
                cout << "Enter passenger capacity: ";
                cin >> passengers;
                cout << "Enter trunk volume (liters): ";
                cin >> trunkVolume;
                
                vehicles.push_back(make_unique<Car>(model, year, passengers, trunkVolume));
                cout << "Car added successfully!" << endl;
                break;
            }
            
            case 2: {
                string model;
                int year;
                double cargoVolume, maxPayload;
                
                cout << "Enter model: ";
                cin >> model;
                cout << "Enter year: ";
                cin >> year;
                cout << "Enter cargo bay volume (m³): ";
                cin >> cargoVolume;
                cout << "Enter max payload (tons): ";
                cin >> maxPayload;
                
                vehicles.push_back(make_unique<Truck>(model, year, cargoVolume, maxPayload));
                cout << "Truck added successfully!" << endl;
                break;
            }
            
            case 3: {
                string model;
                int year;
                double batteryCapacity, batteryWeight;
                
                cout << "Enter model: ";
                cin >> model;
                cout << "Enter year: ";
                cin >> year;
                cout << "Enter battery capacity (kWh): ";
                cin >> batteryCapacity;
                cout << "Enter battery weight (kg): ";
                cin >> batteryWeight;
                
                vehicles.push_back(make_unique<ElectricCar>(model, year, batteryCapacity, batteryWeight));
                cout << "Electric Car added successfully!" << endl;
                break;
            }
            
            case 4: {
                string model;
                int year, passengers;
                double trunkVolume, batteryCapacity, batteryWeight, fuelTank;
                
                cout << "Enter model: ";
                cin >> model;
                cout << "Enter year: ";
                cin >> year;
                cout << "Enter passenger capacity: ";
                cin >> passengers;
                cout << "Enter trunk volume (liters): ";
                cin >> trunkVolume;
                cout << "Enter battery capacity (kWh): ";
                cin >> batteryCapacity;
                cout << "Enter battery weight (kg): ";
                cin >> batteryWeight;
                cout << "Enter fuel tank volume (liters): ";
                cin >> fuelTank;
                
                vehicles.push_back(make_unique<HybridCar>(model, year, passengers, trunkVolume,
                                                           batteryCapacity, batteryWeight, fuelTank));
                cout << "Hybrid Car added successfully!" << endl;
                break;
            }
            
            case 5: {
                if (vehicles.empty()) {
                    cout << "No vehicles in the system." << endl;
                } else {
                    cout << "\n--- All Vehicles ---" << endl;
                    for (size_t i = 0; i < vehicles.size(); ++i) {
                        cout << i + 1 << ". ";
                        vehicles[i]->displayInfo();
                        cout << " | Cargo capacity: " << vehicles[i]->calculateMaxCargoWeight() << "t" << endl;
                    }
                }
                break;
            }
            
            case 6: {
                if (vehicles.empty()) {
                    cout << "No vehicles to test. Please add some vehicles first." << endl;
                } else {
                    runPolymorphicTest(vehicles);
                }
                break;
            }
            
            case 7: {
                if (vehicles.empty()) {
                    cout << "No vehicles to delete." << endl;
                    break;
                }
                
                cout << "Select vehicle to delete (1-" << vehicles.size() << "): ";
                int index;
                cin >> index;
                
                if (index >= 1 && index <= static_cast<int>(vehicles.size())) {
                    vehicles.erase(vehicles.begin() + index - 1);
                    cout << "Vehicle deleted successfully!" << endl;
                } else {
                    cout << "Invalid index!" << endl;
                }
                break;
            }
            
            case 8: {
                vehicles.clear();
                cout << "All vehicles cleared!" << endl;
                break;
            }
            
            case 0:
                cout << "Exiting..." << endl;
                break;
                
            default:
                cout << "Invalid choice!" << endl;
        }
        
    } while (choice != 0);
    
    return 0;
}
