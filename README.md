# Monte Carlo Integration using OpenMPI

This repository contains a C++ program that uses **OpenMPI** to estimate definite integrals using the **Monte Carlo method**. The program is capable of parallel computation, distributing tasks across multiple processes for efficient computation.

---

## Objective

To estimate the value of the following two definite integrals using the Monte Carlo method:
1. $\int_0^1 x^2 dx$ (Expected result: $\frac{1}{3}$)
2. $\int_0^1 e^{-x^2} dx$

---

## Features

1. **Command-Line Arguments**:
   - **`-P <value>`**: Specifies the integral to compute (`1` for $x^2$, `2` for $e^{-x^2}$).
   - **`-N <value>`**: Number of random samples to generate.

2. **Parallel Computation**:
   - Utilizes OpenMPI for task distribution and parallel processing.
   - Uses `MPI_Bcast` to broadcast configuration data and `MPI_Reduce` to gather results.

3. **Random Number Generation**:
   - Generates uniform random samples for Monte Carlo integration.

4. **Error Handling**:
   - Validates command-line arguments.
   - Ensures proper distribution of tasks and samples among MPI processes.

---

## File Structure

### Source File
- **main.cpp**: Contains the complete implementation of the Monte Carlo integration with OpenMPI.

---

## How to Run

### Prerequisites

1. **MPI Library**: Install OpenMPI on your system.
2. C++ Compiler: A compiler with OpenMPI support

## Compilation
1. Clone the respository and navigate to the project directory.
2. Compile the code
3. Run the program
   ```bash
   mpirun -np <number_of_processes> ./monteCarlo -P <integral_option> -N <num_samples>
   ```
### Parameters
- `<number_of_processes>`: Number of MPI processes to use.
- `<integral_option>`:
  - `1`: Compute $\int_{0}^{1} x^2 ,dx$
  - `2`: Compute $\int_{0}^{1} e^{-x^2} ,dx$
- `<num_samples>`: Total number of random samples to use for the computation.


### Example Output:
```bash
The estimate for integral 1 is 0.333333
Bye!
```
---

## Testing and Debugging

#### Key Features to Test

1. **Command-Line Arguments**:
   - Validate default values when arguments are missing.
   - Test invalid inputs to ensure proper error handling.

2. **Parallel Task Distribution**:
   - Verify that tasks and samples are distributed correctly among processes.

3. **Monte Carlo Calculation**:
   - Ensure the computed result is accurate for both integrals.

4. **Random Number Generation**:
   - Check proper randomization of numbers across processes.

5. **Performance**:
   - Measure the computation time for varying numbers of processes and samples.

