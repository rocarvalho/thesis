/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "UMApHLP_Model.h"
#include "Properties.h"
#include "Problem.h"
#include "Solution.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <gurobi_c++.h>
#include <chrono>


UMApHLP_Model::UMApHLP_Model() {
    // It does nothing.
}

UMApHLP_Model::~UMApHLP_Model() {
    // It does nothing.
}


Solution UMApHLP_Model::solve(const Problem& problem, const Properties* const opt_input, Properties* const opt_output) {
    
    // Input parameters
    bool verbose = false;
    double time_limit = DBL_MAX;
    int threads = 1;
    
    if (opt_input != nullptr) {
        
        // Verbosity
        if (opt_input->contains("verbose")) {
            verbose = opt_input->get<bool>("verbose");
        }
        
        // Maximum running time
        if (opt_input->contains("time_limit")) {
            time_limit = opt_input->get<double>("time_limit");
        }
        
        // Number of threads
        if (opt_input->contains("threads")) {
            threads = opt_input->get<int>("threads");
        }
    }
        
    // Keep the best solution found
    Solution solution(problem.count_nodes());
    
    // Keep an instance to Gurobi environment
    GRBEnv* env = nullptr;
    
    // Start timer
    std::chrono::steady_clock::time_point timer_start = std::chrono::steady_clock::now();        
    
    try {
        
        // Gurobi environment and model
        env = new GRBEnv();
        GRBModel model(*env);
        
        // Set some settings of Gurobi solver
        //model.getEnv().set(GRB_IntParam_OutputFlag, 0);
        //model.getEnv().set(GRB_DoubleParam_TimeLimit, 86400); // 24 h in sec
        model.getEnv().set(GRB_IntParam_Threads, threads);
        
        //x_ijkm                 
        GRBVar**** x;
        x = new GRBVar***[problem.count_nodes()];
        for (int i=0; i < problem.count_nodes(); i++) {
            x[i] = new GRBVar**[problem.count_nodes()];
            for (int k=0; k < problem.count_nodes(); k++) {
                x[i][k] = new GRBVar*[problem.count_nodes()];                
                for(int m = 0; m < problem.count_nodes(); m++){
                    x[i][k][m] = new GRBVar[problem.count_nodes()];                
                }
             }            
        }
        
        for(int i=0; i < problem.count_nodes(); ++i){
            for(int j=0; j < problem.count_nodes(); ++j){
                for(int k=0; k < problem.count_nodes(); ++k){
                    for(int m=0; m < problem.count_nodes(); ++m){                        
                        x[i][j][k][m] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS);                         
                    }
                }
            }
        }            
                
        
        GRBVar y[problem.count_nodes()];
        for (int j=0; j<problem.count_nodes(); ++j){
            y[j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY); 
        }
        
        model.update();
        
        //Objective Function
        //min z = sum_ijkm c_ijkm x_ijkm
        GRBLinExpr objective;      
        for(int i=0; i < problem.count_nodes(); ++i){
            for(int j=0; j < problem.count_nodes(); ++j){
                for(int k=0; k < problem.count_nodes(); ++k){
                    for(int m=0; m < problem.count_nodes(); ++m){                        
                       objective += problem.get_path_cost(i,j,k,m) * x[i][j][k][m]; 
                    }
                }
            }
        }
        
        model.setObjective(objective, GRB_MINIMIZE);
        
        //Constraint 1
        //sum_m|m<>K x_ijmk + sum_m x_ijkm <= w_ij y_k forall i,j,k
        for(int i=0; i < problem.count_nodes(); ++i){            
            for(int j=0; j < problem.count_nodes(); ++j){
                for(int k=0; k < problem.count_nodes(); ++k){
                    GRBLinExpr constraint1;
                    for(int m = 0; m < problem.count_nodes(); ++m){                        
                        if(m != k){
                            constraint1 += x[i][j][m][k];                            
                        }   
                        constraint1 += x[i][j][k][m];
                        
                    }
                    model.addConstr(constraint1 <= problem.get_demand(i,j)*y[k]);
                }
            }            
        }
        
        //Constraint 2
        //sum_km x_ijkm - w_ij forall i,j
        for(int i=0; i < problem.count_nodes(); ++i){            
            for(int j=0; j < problem.count_nodes(); ++j){
                GRBLinExpr constraint2;
                for(int k=0; k < problem.count_nodes(); ++k){         
                    for(int m = 0; m < problem.count_nodes(); ++m){                        
                        constraint2 += x[i][j][k][m];
                    }
                }
                model.addConstr(constraint2 == problem.get_demand(i,j));
            }
        }
        
        //Constraint 3
        GRBLinExpr constraint3;
        for(int i = 0; i<problem.count_nodes();i++){
            constraint3 += y[i];
        }
        model.addConstr(constraint3 == problem.count_hubs());
                
        model.update();
        model.optimize();                    
        std::cout << "Profit: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
        std::cout << "Elapsed time: " << model.get(GRB_DoubleAttr_Runtime) << std::endl; 
        std::cout << "Hubs: " << std::endl;
        
        for(int i = 0; i<problem.count_nodes(); i++){
            if(y[i].get(GRB_DoubleAttr_X) == 1)
            std::printf(" %d ", i);            
        }
        
        int sol[problem.count_nodes()];
        for(int l = 0; l < problem.count_nodes();l++) sol[l] = 0;
        
        std::cout << "\nAssignments: " << std::endl;        
        for(int i=0; i < problem.count_nodes(); ++i){            
            std::printf(" %d : ", i);  
            
            for(int j=0; j < problem.count_nodes(); ++j){
                for(int k=0; k < problem.count_nodes(); ++k){        
                    for(int m = 0; m < problem.count_nodes(); ++m){                        
                        if(x[i][j][k][m].get(GRB_DoubleAttr_X) > 0.0001){
                            sol[k] = 1;
                        }
                    }
                }
            }
            
            for(int l = 0; l < problem.count_nodes(); l++){
                if(sol[l]==1)
                    std::printf(" %d ",l);                
                sol[l] = 0;
            }
            std::printf("\n");            
        }
                
        
    } catch (GRBException e) {
        std::cerr << "Gurobi error " << e.getErrorCode() << ": " << e.getMessage();
        std::cerr << std::endl << std::endl;
        std::exit(EXIT_FAILURE);
    } catch (...) {
        std::cerr << "Unexpected error." << std::endl << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    
    // Return the best solution found
    return solution;
}