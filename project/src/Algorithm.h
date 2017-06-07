#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Problem.h"
#include "Solution.h"
#include "Properties.h"


/**
 * Super-class inherited by those classes that implements algorithms for 
 * solving the problem.
 */
class Algorithm {
public:
    
    /**
     * This method must implement the algorithm to solve the problem.
     * 
     * @param   problem
     *          The object with data of the problem to be solved.
     * @param   opt_input
     *          Optional input parameters. It is useful for set algorithm's 
     *          parameters. It can be set to null.
     * @param   opt_output
     *          Optional output parameters. It is useful for returning information 
     *          beyond the solution of the problem. It can be set to null.
     * 
     * @return  A solution to the problem.
     */
    virtual Solution solve(const Problem& problem, const Properties* const opt_input, Properties* const opt_output) = 0;

};


#endif
