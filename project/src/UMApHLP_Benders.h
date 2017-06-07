#ifndef UMAHLP_BENDERS_H
#define UMAHLP_BENDERS_H

#include "Problem.h"
#include "Solution.h"
#include "Algorithm.h"


class UMAHLP_Benders : public Algorithm {

public:

    /**
     * Constructor.
     */
    UMAHLP_Benders();

    /**
     * Destructor.
     */
    virtual ~UMAHLP_Benders();
    
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
    Solution solve(const Problem& problem, const Properties* const opt_input, Properties* const opt_output);
    
    /**
     * Copy constructor is disabled.
     */
    UMAHLP_Benders(const UMAHLP_Benders& orig) = delete;
    
    /**
     * Assignment operator is disabled.
     */
    const UMAHLP_Benders& operator=(const UMAHLP_Benders& orig) = delete;

private:

};

#endif
