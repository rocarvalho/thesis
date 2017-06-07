#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <cxxopts.h>
#include <set>
#include "Problem.h"
#include "Algorithm.h"
#include "Properties.h"
#include "UMApHLP_Benders.h"
#include "UMApHLP_Model.h"
#include "UMApHLP_Benders.h"


int main(int argc, char** argv) {
    
    //try {
        
        // Command line parser
        cxxopts::Options options(argv[0], "Hub Location Problems\n");
        
        options.add_options()
                ("h,help", "Show this help message and exit.")
                ("f,file", "Path to the instance file with data of the problem "
                        "to be solved.", 
                        cxxopts::value<std::string>(), "FILE")
                ("problem", "The problem to be solved. Valid options are: "
                        "\"usahlp\" for Uncapacitated Single Allocation Hub "
                        "Location Problem, or \"umahlp\" Uncapacitated Multi "
                        "Allocation Hub Location Problem, or \"umaphlp\" Uncapacitated Multi"
                        "llocation p Hub Location Problem, or \"cycle\" Cycle Hub Locaation.", 
                        cxxopts::value<std::string>(), "PROBLEM");
        
        options.add_options("Algorithms")
                ("algorithm", "Algorithms available for Uncapacitated Single "
                        "Allocation Hub Location Problem are: \"benders1\", "
                        "\"benders2\", \"ils-rvnd1\", \"ils-rvnd2\" and \"tabu search\"."
                        "For the Uncapacitated Multi Allocation Hub Location problem, "
                        "the available algorithms are: \"benders\", \"sa\", \"ga\" and \"ma\"."
                        "For the Uncapacitated Multi Allocation p-Hub Location problem,"
                        "the available algorithms are: \"benders\" and \"model\"."
                        "For the Cycle Hub problem the available algorithms are: \"ils-rvnd1\""
                        "\"ils-rvnd2\",\"grasp\" and \"model\"",
                        cxxopts::value<std::string>(), "ALGORITHM");                
        
        options.add_options("umaphlp parameters")
                 ("p,hubs","Amount of the hubs.",cxxopts::value<std::string>(),"HUBS")
                 ("t,timelimit","Maximum execution times of algorithm",cxxopts::value<std::string>(),"TIMELIMIT");
        
        options.add_options("Printing")
                ("v,verbose", "Display a log along the optimization process.")
                ("d,details", "Display a summary of the optimization process.")
                ("s,solution", "Display the best solution found.");
        
        // Parse the command line options
        options.parse(argc, argv);
        
        // Help message
        if (options.count("help") > 0) {
            std::cout << options.help({"", "Algorithms", "Printing", "umaphlp parameters"}) << std::endl;
            return EXIT_SUCCESS;
        }
        
        if (options.count("file") < 1 || options.count("algorithm") < 1) {
            std::cout << "There are some arguments missing." << std::endl;
            std::cout << options.help({"", "Algorithms", "Printing"}) << std::endl;
            return EXIT_SUCCESS;
        }
                
        // Load the problem
        Problem problem(options["file"].as<std::string>());
        
        // Initialize the algorithm selected to solve the problem
        Algorithm* algorithm = nullptr;
        Properties opt_input;
        Properties opt_output;
        
        if (options["problem"].as<std::string>() == "umaphlp") {
            if (options["algorithm"].as<std::string>() == "benders") {

            } if (options["algorithm"].as<std::string>() == "model") {                
                algorithm = new UMApHLP_Model();
                opt_input.add("verbose", options.count("verbose") > 0); // Show log
                opt_input.add("time_limit", 86400);                     // 24 h in sec
                opt_input.add("threads", 30);                            // Threads to solve MP
            } else {
                std::cout << "Invalid algorithm." << std::endl;
                std::cout << options.help({"", "Algorithms", "Printing"}) << std::endl;
                return EXIT_SUCCESS;
            }
        } else if (options["problem"].as<std::string>() == "UMAHLP") {
            if (options["algorithm"].as<std::string>() == "benders") {

            } else if (options["algorithm"].as<std::string>() == "tabusearch") {
                
            } else if (options["algorithm"].as<std::string>() == "ils") {
                // TODO
            } else {
                std::cout << "Invalid algorithm." << std::endl;
                std::cout << options.help({"", "Algorithms", "Printing"}) << std::endl;
                return EXIT_SUCCESS;
            }
        }else if(options["problem"].as<std::string>() == "UMApHLP"){ 
        
        }else if(options["problem"].as<std::string>() == "Cycle"){ 
        
        }else {
            std::cout << "Invalid problem." << std::endl;
            std::cout << options.help({"", "Algorithms", "Printing"}) << std::endl;
            return EXIT_SUCCESS;
        }
        
        // Solve the problem
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        Solution solution = algorithm->solve(problem, &opt_input, &opt_output);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        // Compute the elapsed time
        double elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // Compute the value of the objective function and feasibility
        double obj_val = problem.evaluate(solution);
        bool feasibility = problem.is_feasible(solution, (options["problem"].as<std::string>() == "multi"));
        
        // Show the output
        if (options.count("details") > 0 || options.count("solution") > 0) {
            if (options.count("details") > 0) {
                std::printf("======================================================================\n");
                std::printf("SUMMARY\n");
                std::printf("======================================================================\n");
                std::printf("Objective function:  %.6lf\n", obj_val);
                std::printf("Feasibility:         %s\n", (feasibility ? "feasible" : "infeasible"));
                std::printf("Elapsed time (ms):   %ld\n", ((long) elapsed_time));
                std::printf("======================================================================\n\n");
                
                if (opt_output.size() > 0) {
                    std::printf("======================================================================\n");
                    std::printf("ADDITIONAL OUTPUT DATA\n");
                    std::printf("======================================================================\n");
                    std::set<std::string> keys = opt_output.get_keys();
                    for (std::set<std::string>::iterator iter = keys.begin(); iter != keys.end(); ++iter) {
                        std::printf("%s: %s\n", (*iter).c_str(), (opt_output.get<std::string>(*iter)).c_str());
                    }
                    std::printf("======================================================================\n\n");
                }
            }
            if (options.count("solution") > 0) {
                std::printf("======================================================================\n");
                std::printf("SOLUTION\n");
                std::printf("======================================================================\n");
                std::printf("Hubs: ");
                for (int k = 0; k < solution.count_nodes(); ++k) {
                    if (solution.get_hubs()[k]) {
                        std::printf("%d ", k);
                    }
                }
                std::printf("\n\nAssignments:\n");
                for (int i = 0; i < solution.count_nodes(); ++i) {
                    std::printf("%4d : ", i);
                    for (auto k = solution.get_assignments()[i].begin(); k != solution.get_assignments()[i].end(); ++k) {
                        std::printf("%d ", *k);
                    }
                    std::printf("\n");
                }
                std::printf("\n");
                std::printf("======================================================================\n\n");
            }
        } else {
            std::printf("%.6lf\n", obj_val);
        }
        
        //save file
        std::ofstream arquivo;
        arquivo.open("saida.txt");
        arquivo << options["file"].as<std::string>() << "," << obj_val << std::endl;        
        arquivo.close();

        // Free resources
        if (algorithm != nullptr) {
            delete algorithm;
            algorithm = nullptr;
        }
        
    //} catch (const cxxopts::OptionException& e) {
    //    std::cerr << e.what() << std::endl;
    //    std::cerr << "Syntax error. Type the following command for a correct usage." << std::endl;
    //    std::cerr << argv[0] << " --help" << std::endl << std::endl;
    //    return EXIT_FAILURE;
    //} catch (...) {
    //    std::cerr << "Unexpected error." << std::endl << std::endl;
    //    return EXIT_FAILURE;
    //}
    
    return EXIT_SUCCESS;
}
