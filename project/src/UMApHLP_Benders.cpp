#include "UMApHLP_Benders.h"
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


UMAHLP_Benders::UMAHLP_Benders() {
    // It does nothing.
}

UMAHLP_Benders::~UMAHLP_Benders() {
    // It does nothing.
}

Solution UMAHLP_Benders::solve(const Problem& problem, const Properties* const opt_input, Properties* const opt_output) {
    
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
    
    // Log
    if (verbose) {
        std::printf("--------------------------------------------------------------------------\n");
        std::printf("| Iteration |  Lower Bound  |  Upper Bound  |     Gap     |   Time (s)   |\n");
        std::printf("--------------------------------------------------------------------------\n");
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
        model.getEnv().set(GRB_IntParam_OutputFlag, 0);
        //model.getEnv().set(GRB_DoubleParam_TimeLimit, 86400); // 24 h in sec
        model.getEnv().set(GRB_IntParam_Threads, threads);
        
        // Variables of the Master Problem
        GRBVar eta = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
        GRBVar* y = new GRBVar[problem.count_nodes()];
        for (int k = 0; k < problem.count_nodes(); ++k) {
            y[k] = model.addVar(0, 1, 0, GRB_BINARY);
        }
        
        // Variables to compute solution of the Worker Problem
        double*** u = new double**[problem.count_nodes()];
        double** alpha = new double*[problem.count_nodes()];
        for (int i = 0; i < problem.count_nodes(); ++i) {
            u[i] = new double*[problem.count_nodes()];
            alpha[i] = new double[problem.count_nodes()];
            for (int j = 0; j < problem.count_nodes(); ++j) {
                alpha[i][j] = 0.0;
                u[i][j] = new double[problem.count_nodes()];
                for (int k = 0; k < problem.count_nodes(); ++k) {
                    u[i][j][k] = 0.0;
                }
            }
        }
        
        model.update();
        
        // Master Problem's objective function
        GRBLinExpr objective = 0;
        objective += eta;
        //for (int k = 0; k < problem.count_nodes(); ++k) {
        //    objective += problem.get_installation_cost(k) * y[k];
        //}
        model.setObjective(objective, GRB_MINIMIZE);
        
        // Master Problem constraints
        GRBLinExpr constr = 0;
        for (int k = 0; k < problem.count_nodes(); ++k) {
            constr += y[k];
        }
        model.addConstr(constr == problem.count_hubs());
        
        // Upper and Lower bounds
        double upper_bound = GRB_INFINITY;
        double lower_bound = -GRB_INFINITY;
        
        // Optional output parameters
        double gap = 0.0;
        double elapsed_time = 0.0;
        long current_iteration = 0;
        
        // Benders algorithm
        while (upper_bound - lower_bound > 1e-5) {
            
            // Solve Master Problem
            model.optimize();
            
            // Update lower bound
            lower_bound = model.get(GRB_DoubleAttr_ObjVal);
            
            // Solve the Worker Problem
            for (int i = 0; i < problem.count_nodes(); ++i) {
                for (int j = 0; j < problem.count_nodes(); ++j) {
                    alpha[i][j] = GRB_INFINITY;
                    for (int k = 0; k < problem.count_nodes(); ++k) {
                        if (y[k].get(GRB_DoubleAttr_X) > 0.5) {
                            for (int m = 0; m < problem.count_nodes(); ++m) {
                                if (y[m].get(GRB_DoubleAttr_X) > 0.5) {
                                    double path_cost = problem.get_path_cost(i,j,k,m);
                                    if (path_cost < alpha[i][j]) {
                                        alpha[i][j] = path_cost;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            for (int i = 0; i < problem.count_nodes(); ++i) {
                for (int j = 0; j < problem.count_nodes(); ++j) {
                    for (int k = 0; k < problem.count_nodes(); ++k) {
                        if (y[k].get(GRB_DoubleAttr_X) > 0.5) { // y[k] == 1
                           u[i][j][k] = 0.0; 
                        } else {
                            u[i][j][k] = std::max(0.0, alpha[i][j] - problem.get_path_cost(i,j,k,k));
                            
                            double aux = -GRB_INFINITY;
                            for (int m = 0; m < problem.count_nodes(); ++m) {
                                if (m != k && y[m].get(GRB_DoubleAttr_X) > 0.5) {
                                    double aux_0 = std::max(alpha[i][j] - problem.get_path_cost(i,j,k,m), alpha[i][j] - problem.get_path_cost(i,j,m,k));
                                    aux = std::max(aux, aux_0);
                                }
                            }
                            
                            u[i][j][k] = std::max(u[i][j][k], aux);
                        }
                    }
                }
            }
            
            for (int i = 0; i < problem.count_nodes(); ++i) {
                for (int j = 0; j < problem.count_nodes(); ++j) {
                    for (int k = 0; k < problem.count_nodes(); ++k) {
                        if (y[k].get(GRB_DoubleAttr_X) < 0.5) { // y[k] == 0
                            
                            double aux = -GRB_INFINITY;
                            for (int m = 0; m < problem.count_nodes(); ++m) {
                                if (m != k && y[m].get(GRB_DoubleAttr_X) < 0.5) {
                                    double aux_0 = std::max(alpha[i][j] - problem.get_path_cost(i,j,k,m) - u[i][j][m], alpha[i][j] - problem.get_path_cost(i,j,m,k) - u[i][j][m]);
                                    aux = std::max(aux, aux_0);
                                }
                            }
                            
                            u[i][j][k] = std::max(u[i][j][k], aux);
                        }
                    }
                }
            }
            
            // Add Benders cut to the Master Problem
            GRBLinExpr cut_left = 0;
            double cut_right = 0.0;
            cut_left += eta;
            for (int i = 0; i < problem.count_nodes(); ++i) {
                for (int j = 0; j < problem.count_nodes(); ++j) {
                    cut_right += alpha[i][j] * problem.get_demand(i,j);
                    for (int k = 0; k < problem.count_nodes(); ++k) {
                        cut_left += u[i][j][k] * problem.get_demand(i,j) * y[k];
                    }
                }
            }
            
            model.addConstr(cut_left >= cut_right);
            
            // Update upper bound
            double new_ub = 0.0;
            for (int i = 0; i < problem.count_nodes(); ++i) {
                for (int j = 0; j < problem.count_nodes(); ++j) {
                    new_ub += alpha[i][j] * problem.get_demand(i,j);
                    for (int k = 0; k < problem.count_nodes(); ++k) {
                        new_ub -= u[i][j][k] * problem.get_demand(i,j) * y[k].get(GRB_DoubleAttr_X);
                    }
                }
            }
            
            if (new_ub < upper_bound) {
                upper_bound = new_ub;
            }
            
            // Increment the iteration counter
            ++current_iteration;
            
            // Compute elapsed time so far
            std::chrono::steady_clock::time_point timer_now = std::chrono::steady_clock::now();
            elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(timer_now - timer_start).count();
            
            // Compute the relative gap
            gap = std::abs(upper_bound - lower_bound) / (1e-10 + upper_bound);
            
            // Log
            if (verbose) {
                std::printf("| %9ld | %13.3lf | %13.3lf | %10.3lf%% | %12.3lf |\n", 
                        current_iteration, lower_bound, upper_bound, (gap * 100), elapsed_time);
            }
            
            // Check time limit
            if (elapsed_time > time_limit) {
                break;
            }
            
        }
        
        
        
        // Log
        if (verbose) {
            std::printf("--------------------------------------------------------------------------\n\n");
        }
       
        // Set optional output parameters
       
        if (opt_output != nullptr) {
            opt_output->add("ub", (double) upper_bound);
            opt_output->add("lb", lower_bound);
            opt_output->add("gap", gap);
            opt_output->add("runtime", elapsed_time);
            opt_output->add("iterations", current_iteration);
        }
        
        solution.get_upper_bound() = upper_bound;
        
        // Get the solution found
        for (int k = 0; k < problem.count_nodes(); ++k) {
            if (y[k].get(GRB_DoubleAttr_X) > 0.5) {
                solution.get_hubs()[k] = true;
            }
        }

        for(int i = 0; i < problem.count_nodes(); i++){
            for(int j = 0; j < problem.count_nodes(); j++){
                double path_ij = DBL_MAX;
                int hub = -1;
                for(int k = 0; k < solution.get_hubs().size(); k++){
                    for(int m = 0; m < solution.get_hubs().size();m++){
                        if(solution.get_hubs()[k] && solution.get_hubs()[m]){
                            if(path_ij > problem.get_path_cost(i,j,k,m)){
                                hub = k;
                                path_ij = problem.get_path_cost(i,j,k,m);
                            }
                        }
                    }
                }
                solution.get_assignments()[i].insert(hub);
            }
        }
        
        
        // Deallocate memory
        delete[] y;
        y = nullptr;
        
        for (int i = 0; i < problem.count_nodes(); ++i) {
            for (int j = 0; j < problem.count_nodes(); ++j) {
                delete[] u[i][j];
                u[i][j] = nullptr;
            }
            
            delete[] u[i];
            u[i] = nullptr;
            
            delete[] alpha[i];
            alpha[i] = nullptr;
        }
        
        delete[] u;
        u = nullptr;
        
        delete[] alpha;
        alpha = nullptr;
    
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