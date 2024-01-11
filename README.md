# re-toy paper name  todo

Welcome to the RE-toy algorithm and our reachability testing platform!

This repository includes the C++ implementation of the RE-toy algorithm, as described in our paper todo todo[insert paper title]. Additionally, it provides a platform that facilitates the validation and testing of reachability algorithms.

## Code Structure

The code is organized as follows:

- `Graph` class: represents the input graph. It can be constructed by random Directed Acyclic Graphs (DAGs) or conversion of input files representing directed graphs into DAGs. All algorithms within the platform utilize the `Graph` class as a unified input, enabling convenient and standardized testing.
- `AutoTest` class: provides functionality for testing reachability algorithms.
- `Timer` class: provides timing utilities.
- `Profile` struct: stores the results obtained from the testing process.
- `algorithms` directory: contains implementations of various reachability algorithms. To facilitate testing, we have made slight modifications to these algorithms, ensuring that they all inherit from the abstract class `Algorithm` and implement the same interface.
- `include` directory: contains library dependencies of some baseline algorithms.
- `graphs` directory: contains real DAGs used in our experiments.
- `main.cpp`: executes a single experiment, including the construction of index and query testing for of an algorithm with specific parameters on a given graph.
- `test.sh` and `experiments.sh`: automate the execution of `main.cpp` to perform multiple tests.

## Reachability Program Usage

The `reachability` program is designed to be used in conjunction with the `test.sh` or `experiments.sh` script rather than being used independently. Users can customize the testing parameters within the `test.sh` script according to their specific needs, or perform the experiments mentioned in our paper by directly running `experiments.sh`.

### Preparation

Compile the project and generate the `reachability` executable file.

### Parameters

To execute the program, use the following command:

```
./reachability [options]
```

#### Options

- `--help`: Show help message
- `--time <seconds>`: Set the maximum execution time in seconds for the program (required, unless --accuracy is specified)
- `--accuracy`: Run tests to validate the correctness of the algorithm (optional, --time parameter is not required)
- `--query_num <number>`: Set number of queries, default value is 100000 (optional)
- `--result_file <file>`: Set the file path for result output (optional)
- `--result_dir <dir>`: Set the directory path for query time output (optional)
- `--graph <type>`: Specify input graph type (required)
  - `--random <n> <d>`: Generate a random DAG with \<n\> nodes and \<d\> average degree
  - `--file <file_path>`: Load a directed graph from file and convert it to a DAG
- `--seed <seed>`: Set the seed for the generation of the random graph (optional)
- `--algorithm <algorithm_name> [algorithm_params]`: Specify the reachability algorithm to use and its parameters (required)

Here are the available reachability algorithms and their usages:

- `re_toy <x> <r>`: RE-toy algorithm
- `bfl <K>`: BFL algorithm
- `grail <t> <ltype> <dim>`: GRAIL algorithm
- `pathtree <alg_type>`: PathTree algorithm
- `tol <style> <opr>`: TOL algorithm
- `gripp`: GRIPP
- `ferrari <k> <seeds> <global>`: Ferrari algorithm
- `ip <k> <h> <mu>`: IP algorithm
- `pll <use_RQPrunedLandmarkLabeling>`: PLL algorithm
- `preach`: PReaCH algorithm
- `dbl`: DBL algorithm

#### Examples

Here are some examples of how to use the commands:

- Run the reachability algorithm RE-toy of specific parameters on a randomly generated DAG with 100 nodes and an average degree of 3 with a maximum execution time of 10 seconds:

  ````
  ./reachability --time 10 --graph --random 100 3 --algorithm re_toy 32 2.0
  ```

- Run the reachability algorithm BFL of specific parameters on a specified graph file:

  ````
  ./reachability --time 1000 --graph --file /path/to/graph.txt --algorithm bfl 5
  ```

- Run tests to validate the correctness of the algorithm PLL on a random graph with a specific seed without limiting the maximum execution time:

  ````
  ./reachability --accuracy --graph --random 1000 1 --seed 29 --algorithm pll 1
  ```

Please note that the above examples are meant to illustrate the usage of command-line arguments, and you should replace specific parameter values and file paths with your own.

### Input Graph File Format

The input graph file should be in the `.txt` format and has `n` lines, representing a valid directed graph with `n` nodes, numbered from 0 to `n-1`. Each line of the file represents the neighbors of a node, with the lines ordered in ascending order. The format for each line is as follows:

```
[node]: [neighbor1] [neighbor2] ...
```

For example, a valid `graph.txt` file could look like:

```
0: 1 2
1: 2
2:
3: 1
4: 2 3
```

This file represents a graph with five nodes, where node 0 has edges to nodes 1 and 2, node 1 has an edge to node 2, and so on.

## Test Script `tesh.sh` Usage

The `test.sh` script is an example on how to automate multiple tests with different parameters using the `reachability` program. You can follow the steps below to use the `test.sh` script:

1. Make sure you have successfully compiled the `reachability` program.

2. Modify the following variables within the script to customize the testing parameters:

   - `executable`: Provide the path to the `reachability` executable file generated after compilation.
   - `input_directory`: Specify the common parent directory path where the input graph files are located. Ensure that the graph files are present in this directory and update the paths in the `graphs` array accordingly.
   - `test_accuracy`: Set this variable to true if you want to perform accuracy tests, or false if you want to test index size, construction time, and query time.
   - `output_file`: Set the path to the output `result.csv` file where the test results will be stored.
   - `output_query_time_dir`: Set the path to the directory where the output query time files will be stored. Please note that the script will delete all `.txt` files under this directory before writing new query time files. Make sure to save any important files before running this script.
   - `query_num`: Define the number of query pairs to be used in each test.
   - `max_time`: Set the maximum execution time in seconds for each test.

3. Modify the `graphs` and `algorithms` arrays to include specific graphs and reachability algorithms you want to test.

4. Run the following command to execute the script:

   ```
   ./test.sh
   ```
   
   The script will start executing the tests based on the specified parameters.

5. If `test_accuracy` is set to true, results will be shown in the terminal. Otherwise, once the script completes, the test results will be saved in the file specified by the `output_file` variable and the query time files will be stored in the directory specified by the `output_query_time_dir` variable.

## Test Script `experiments.sh` Usage

The `experiments.sh` runs all the four experiments mentioned in the paper, namely tradeoff, real_graph, scale_up, and dense_up.

Most configuration parameters of this script are similar to those in `test.sh`.

`seed_cnt`: Specify the number of seeds used to generate a random graph. Each algorithm would be tested `seed_cnt` times on different random graphs with a specific `n` and `d`.

## Citation

If you use the RE-toy algorithm or our testing platform for your research or any other work, please cite our paper.

todo: bib

## Disclaimer

While our repository provides the implementations of many reachability algorithms, we take no responsibility for them, other than that of our own algorithm, RE-toy. The implementations of other reachability algorithms provided are for reference purposes only. Please refer to the original articles and official source code.
