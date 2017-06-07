#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include "Solution.h"


/**
 * This class models Uncapacitated Multiple Allocation Hub Location Problem.
 */    
class Problem {
public:
    
    /**
     * Constructor. This constructor builds an object from a file.
     * 
     * @param   filename
     *          Path to file with problem's data to load.
     */
    Problem(const std::string& filename);
    
    /**
     * Copy constructor.
     * 
     * @param   other
     *          The object to be copied.
     */
    Problem(const Problem& other);
    
    /**
     * Destructor.
     */
    virtual ~Problem();
    
    /**
     * Assignment operator. Makes this object a copy of the the specified 
     * object.
     * 
     * @param   other
     *          The object to be copied.
     * 
     * @return  A constant reference to this object.
     */
    virtual const Problem& operator=(const Problem& other);
    
    /**
     * Evaluate the objective function for a given solution.
     * 
     * @param   solution
     *          The solution to be evaluated.
     * 
     * @return  The value of objective function for the given solution.
     */
    double evaluate(const Solution& solution) const;
    
    /**
     * Check the feasibility of a given solution.
     * 
     * @param   solution
     *          The solution that will be checked for feasibility.
     * 
     * @return  It returns true if the solution is feasible, false otherwise.
     */
    bool is_feasible(const Solution& solution, bool is_multi) const;
    
    /**
     * Return the number of nodes in this instance (problem size).
     * 
     * @return  The number of nodes.
     */
    const int& count_nodes() const;
    
    const int& count_hubs() const;
    
    /**
     * Return the discount factor used in this instance. The discount factor is 
     * used to compute the cost between to hubs. Given K and L two nodes and 
     * the transportation cost between K and L equal to c(K,L). If K and L are 
     * set as hubs, then the transportation cost between K and L becomes
     * alpha * c(K,L), where alpha is the discount factor.
     * 
     * @return  The discount factor.
     * 
     */
    const double& get_discount_transfer() const;
        
    const double& get_discount_collect() const;    
    const double& get_discount_distribute() const;
       
    
    /**
     * Return a matrix of demands between pair of nodes.
     * 
     * @return  Demands between pair of nodes.
     */
    const double& get_demand(int i, int j) const;
    
    /**
     * Return a matrix of transportation costs.
     * 
     * @return  Transportation costs.
     */
    const double& get_transportation_cost(int i, int j) const;
    
    /**
     * Compute the cost of an unit demand from i to j going through hubs k 
     * and l, i.e, the demand does through the path i -> k -> l -> j.
     * 
     * @param   i
     *          origin
     * @param   j
     *          destination
     * @param   k
     *          first hub
     * @param   l
     *          second hub
     * 
     * @return  The cost of an unit demand from i to j going through hubs k and 
     *          l, respectively.
     */
    double get_path_cost(int i, int j, int k, int l) const;
    
private:
    
    int _n_nodes;
    int _n_hubs;
    double _discount_collect;
    double _discount_transfer;
    double _discount_distribute;    
    double** _demands;
    double** _transportation_costs;
    
};


#endif
