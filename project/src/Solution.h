#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <set>


/**
 * This class encodes a solution to the Uncapacitated Multi Allocation Hub 
 * Location Problem.
 */
class Solution {
    
public:
    
    /**
     * Constructor.
     * 
     * @param   n_nodes
     *          The number of nodes in the problem.
     */
    Solution(int n_nodes);
    
    /**
     * Copy constructor.
     * 
     * @param   other
     *          The solution to be copied.
     */
    Solution(const Solution& other);
    
    /**
     * Destructor.
     */
    virtual ~Solution();
    
    /**
     * Assignment operator.
     * 
     * @param   other
     *          The solution so be copied.
     * 
     * @return  A constant reference to this object.
     */
    virtual const Solution& operator=(const Solution& other);
    
    /**
     * Return the number of nodes in the problem.
     * 
     * @return  The number of nodes in the problem.
     */
    int count_nodes() const;
    
    double& get_upper_bound();
    
    /**
     * Return a reference to a boolean array that indicates nodes set as hubs.
     * 
     * @return  An array indicating nodes set as hubs.
     */
    std::vector<bool>& get_hubs();
    
    /**
     * Return a reference to a boolean array that indicates nodes set as hubs.
     * 
     * @return  An array indicating nodes set as hubs.
     */
    const std::vector<bool>& get_hubs() const;
    
    /**
     * Return the flow.
     * 
     * @return  The flow.
     */
    std::vector< std::set<int> >& get_assignments();
    
    /**
     * Return the flow.
     * 
     * @return  The flow.
     */
    const std::vector< std::set<int> >& get_assignments() const;
    
private:
    
    int _n_nodes;
    std::vector<bool> _hubs;
    std::vector< std::set<int> > _assignments;
    double _upper_bound;

};

#endif
