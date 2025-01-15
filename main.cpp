/*
Author: Leandro Alan Kim
Class: ECE4122/6122
Last Date Modified: 3 Dec 2024
Description: use OpenMPI to estimate the value of a definite integral using the Monte Carlo method.
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <string>
#include <iostream>
#include <cmath>

#define MASTER 0 // task ID of master task

int main(int argc, char *argv[])
{
    int taskid; // id for each mpi
    int numtasks; // total num of mpi
    int rc; // return
    MPI_Status status;
    
    // initialize mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // total num of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid); // get id

    int mode = 1;
    int samples = 1000;
    
    // check arguments
    if (taskid == MASTER) {
        if (argc != 5) {
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        // command line arg
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-P") {
                if (i + 1 < argc) {
                    try {
                        mode = std::stoi(argv[i + 1]);
                        if (mode != 1 && mode != 2) {
                            MPI_Abort(MPI_COMM_WORLD, 1);
                            return 1;
                        }
                        i++; // skip
                    } 
                    catch (const std::invalid_argument& e) {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                        return 1;
                    }
                }
            }
            else if (arg == "-N") {
                if (i + 1 < argc) {
                    try {
                        samples = std::stoi(argv[i + 1]);
                        if (samples <= 0) {
                            MPI_Abort(MPI_COMM_WORLD, 1);
                            return 1;
                        }
                        i++; // skip
                    } 
                    catch (const std::invalid_argument& e) {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                        return 1;
                    }
                }
            }
        }
    }

    // from master to other processes
    MPI_Bcast(&mode, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&samples, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    
    double integralSumLocal = 0; // sum for process
    double integralSum = 0; // total sum
    double integral = 0; // result
    
    // divide samples
    int samplesPerTask = samples / numtasks;
    if (taskid == MASTER) {
        samplesPerTask += samples % numtasks;
    }

    // rand num generator
    std::random_device rd;
    std::mt19937 generator(rd() + taskid);
    std::uniform_real_distribution<double> distr(0.0, 1.0);

    // monte carlo integ
    for (int i = 0; i < samplesPerTask; i++) {
        double x = distr(generator); // rand x in 0,1
        if (mode == 1) {
            integralSumLocal += x * x;  // x^2
        }
        else {
            integralSumLocal += std::exp(-x * x);  // e^(-x^2)
        }
    }

    // mpi to sum result
    rc = MPI_Reduce(&integralSumLocal, &integralSum, 1, MPI_DOUBLE, MPI_SUM, 
                    MASTER, MPI_COMM_WORLD);

    // print final result
    if (taskid == MASTER) {
        integral = integralSum / samples;
        printf("The estimate for integral %i is %f\nBye!\n", mode, integral);
    }

    MPI_Finalize();
    return 0;
}