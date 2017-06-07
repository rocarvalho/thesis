#include "Problem.h"
#include <cmath>
#include <fstream>
#include <string>
#include <limits>
#include <iostream>


Problem::Problem(const std::string& filename) {
    
    // Open the file
    std::ifstream file(filename.c_str());
    std::string token;
    
    // Read the instance size
    file >> token;
    _n_nodes = std::stoi(token);
    
    //Read the number of hubs
    file >> token;
    _n_hubs = std::stoi(token);
    
    // Read discount factor (collect)
    file >> token;
    _discount_collect = std::stod(token);
    
    // Read discount factor (transfer)
    file >> token;
    _discount_transfer = std::stod(token);
    
    // Read discount factor (distribute)    
    file >> token;
    _discount_distribute = std::stod(token);            
    
    //Ignoring the coordinates
    for(int i = 0; i < _n_nodes; i++){
        file >> token;        
        file >> token;        
    }
    
    // Read demands
    _demands = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _demands[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            file >> token;
            _demands[i][j] = std::stod(token);
        }
    }
    
    // Read transportation costs
    _transportation_costs = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _transportation_costs[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            file >> token;
            _transportation_costs[i][j] = std::stod(token);
        }
    }
    
    // Close the file
    file.close();
}

Problem::Problem(const Problem& other) {
    
    // Instance size
    _n_nodes = other._n_nodes;
    
    // Discount factor - collect
    _discount_collect = other._discount_collect;
    
    // Discount factor - transfer
     _discount_transfer = other._discount_transfer;     
     
    // Discount factor - distribute
    _discount_distribute = other._discount_distribute;
    
    // Demands
    _demands = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _demands[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            _demands[i][j] = other._demands[i][j];
        }
    }
    
    // Transportation costs
    _transportation_costs = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _transportation_costs[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            _transportation_costs[i][j] = other._transportation_costs[i][j];
        }
    }
}

Problem::~Problem() {
    
    // Deallocate resources
    
    for (int i = 0; i < _n_nodes; ++i) {
        
        delete[] _demands[i];
        _demands[i] = nullptr;
        
        delete[] _transportation_costs[i];
    }
    
    delete[] _demands;
    _demands = nullptr;
    
    delete[] _transportation_costs;
    _transportation_costs = nullptr;
    
}

const Problem& Problem::operator=(const Problem& other) {
    
    // Deallocate resources
    for (int i = 0; i < _n_nodes; ++i) {

        delete[] _demands[i];
        _demands[i] = nullptr;

        delete[] _transportation_costs[i];
    }

    delete[] _demands;
    _demands = nullptr;

    delete[] _transportation_costs;
    _transportation_costs = nullptr;
    
    // Instance size
    _n_nodes = other._n_nodes;
    
    // Discount factor
    _discount_collect = other._discount_collect;
    _discount_distribute = other._discount_distribute;
    _discount_transfer = other._discount_transfer;
    
    // Demands
    _demands = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _demands[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            _demands[i][j] = other._demands[i][j];
        }
    }
    
    // Transportation costs
    _transportation_costs = new double*[_n_nodes];
    for (int i = 0; i < _n_nodes; ++i) {
        _transportation_costs[i] = new double[_n_nodes];
        for (int j = 0; j < _n_nodes; ++j) {
            _transportation_costs[i][j] = other._transportation_costs[i][j];
        }
    }
    
    return *this;
}

double Problem::evaluate(const Solution& solution) const {
    double obj_val = 0.0;       
    
    // Sum path costs - PROBLEM!!! DO NOT USE
    for (int i = 0; i < _n_nodes; ++i) {
        for (int j = 0; j < _n_nodes; ++j) {
            if (i != j) {
                double best_path = std::numeric_limits<double>::max();
                for (auto k = solution.get_assignments()[i].begin(); k != solution.get_assignments()[i].end(); ++k) {
                    for (auto l = solution.get_assignments()[j].begin(); l != solution.get_assignments()[j].end(); ++l) {
                        double current_path = get_path_cost(i, j, *k, *l);
                        if (current_path < best_path) {
                            best_path = current_path;
                        }
                    }
                }
                obj_val += _demands[i][j] * best_path;
            }
        }
    }
    
    return obj_val;
}

bool Problem::is_feasible(const Solution& solution, bool is_multi) const {
    
    // Constraint: at least one hub is installed
    int count_hubs = 0;
    for (int i = 0; i < _n_nodes; ++i) {
        if (solution.get_hubs()[i]) {
            ++count_hubs;
        }
    }
    
    if (count_hubs < 1) {
        return false;
    }
    
    // Constraint: check assignments
    for (int i = 0; i < _n_nodes; ++i) {
        
        // Assigned to at least one hub
        if ((is_multi && solution.get_assignments()[i].size() < 1) || (!is_multi && solution.get_assignments()[i].size() != 1))  {
            return false;
        }
        
        // All assignments to hubs only
        if (solution.get_hubs()[i]) {
            if (solution.get_assignments()[i].size() != 1 || *(solution.get_assignments()[i].begin()) != i) {
                return false;
            }
        } else {
            for (auto k = solution.get_assignments()[i].begin(); k != solution.get_assignments()[i].end(); ++k) {
                if (!solution.get_hubs()[*k]) {
                    return false;
                }
            }
        }
    }
    
    // All constraints are met
    return true;
    
}

const int& Problem::count_nodes() const {
    return _n_nodes;
}

const int& Problem::count_hubs() const {
    return _n_hubs;
}

const double& Problem::get_discount_collect() const {
    return _discount_collect;
}

const double& Problem::get_discount_transfer() const {
    return _discount_transfer;
}

const double& Problem::get_discount_distribute() const {
    return _discount_distribute;
}

const double& Problem::get_demand(int i, int j) const {
    return _demands[i][j];
}

const double& Problem::get_transportation_cost(int i, int j) const {
    return _transportation_costs[i][j];
}

double Problem::get_path_cost(int i, int j, int k, int l) const {
    return _discount_collect*_transportation_costs[i][k] + _discount_transfer* _transportation_costs[k][l] + _discount_distribute*_transportation_costs[l][j];
}