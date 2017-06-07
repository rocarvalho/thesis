#include "Solution.h"
#include <vector>
#include <set>


Solution::Solution(int n_nodes) : 
_n_nodes(n_nodes), _hubs(n_nodes, false), _assignments(n_nodes, std::set<int>())
{
    // It does nothing.
}

Solution::Solution(const Solution& other) : 
_n_nodes(other._n_nodes), _hubs(other._hubs), _assignments(other._assignments)
{
    // It does nothing.
}

Solution::~Solution() {
    // It does nothing.
}

const Solution& Solution::operator=(const Solution& other) {
    _n_nodes = other._n_nodes;
    _hubs = other._hubs;
    _assignments = other._assignments;
    _upper_bound = other._upper_bound;
    return *this;
}

int Solution::count_nodes() const {
    return _n_nodes;
}

double& Solution::get_upper_bound(){
    return _upper_bound;
}

std::vector<bool>& Solution::get_hubs() {
    return _hubs;
}

const std::vector<bool>& Solution::get_hubs() const {
    return _hubs;
}

std::vector< std::set<int> >& Solution::get_assignments() {
    return _assignments;
}

const std::vector< std::set<int> >& Solution::get_assignments() const {
    return _assignments;
}
