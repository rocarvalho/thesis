/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UMApHLP_Model.h
 * Author: rodrigo
 *
 * Created on 16 de Maio de 2017, 12:16
 */

#ifndef UMApHLP_MODEL_H
#define UMApHLP_MODEL_H

#include "Problem.h"
#include "Solution.h"
#include "Algorithm.h"

class UMApHLP_Model : public Algorithm {

public:

    /**
     * Constructor.
     */
    UMApHLP_Model();

    /**
     * Destructor.
     */
    virtual ~UMApHLP_Model();
    
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
    UMApHLP_Model(const UMApHLP_Model& orig) = delete;
    
    /**
     * Assignment operator is disabled.
     */
    const UMApHLP_Model& operator=(const UMApHLP_Model& orig) = delete;

private:

};


#endif /* UMAHLP_MODEL_H */

