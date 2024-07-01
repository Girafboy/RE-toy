# Reachability-state Encoding: Bridging tradeoffs between the Minimal Storage and the Fastest Query for Reachability Query

Welcome to the RE-toy algorithm and our reachability testing platform!

This repository includes the C++ implementation of the RE-toy algorithm, as described in our paper Reachability-state Encoding: Bridging tradeoffs between the Minimal Storage and the Fastest Query for Reachability Query. Additionally, it provides a platform that facilitates the validation and testing of reachability algorithms.

## Code Structure

The code is organized as follows:

- `Graph` class: represents the input graph. It can be constructed by random/complete Directed Acyclic Graphs (DAGs) or conversion of input files representing directed graphs into DAGs. All algorithms within the platform utilize the `Graph` class as a unified input, enabling convenient and standardized testing.
- `AutoTest` class: provides functionality for testing reachability algorithms.
- `Timer` class: provides timing utilities.
- `Profile` struct: stores the results obtained from the testing process.
- `algorithms` directory: contains implementations of various reachability algorithms. To facilitate testing, we have made slight modifications to these algorithms, ensuring that they all inherit from the abstract class `Algorithm` and implement the same interface.
- `include` directory: contains library dependencies of some baseline algorithms.
- `graphs` directory: contains real DAGs used in our experiments.
- `main.cpp`: executes a single experiment, including the construction of index and query testing for of an algorithm with specific parameters on a given graph.
- `demo.sh` and `experiments.sh`: automate the execution of `main.cpp` to perform multiple tests.

## Usage

Here are the instructions for using the `reachability` program. Additionally, we have included a convenient script called `demo.sh` to facilitate the usage of this program. The `demo.sh` script serves as an example of automating multiple tests with various parameters using the `reachability` program. You can refer to the comments within the script to customize your own tests.

### Preparation

Compile the project and generate the `reachability` executable file.

### Parameters

To execute the program, use the following command:

```bash
./reachability [options]
```

#### Options

- `--help`: Show help message
- `--time <seconds>`: Set the maximum execution time in seconds for the program (required, unless --accuracy is specified)
- `--accuracy`: Run tests to validate the correctness of the algorithm (optional, --time parameter is not required)
- `--query_num <number>`: Set number of queries, default value is 100000 (optional)
- `--result_file <file>`: Set the file path for result output (optional, use when generating the result file)
- `--graph <type>`: Specify input graph type (required)
  - `--random <n> <d>`: Generate a random DAG with \<n\> nodes and \<d\> average degree
  - `--complete <n>`: Generate a complete DAG with \<n\> nodes
  - `--file <file_path>`: Load a directed graph from file and convert it to a DAG
- `--seed <seed>`: Set the seed for the generation of the random graph (optional)
- `--algorithm <algorithm_name> [algorithm_params]`: Specify the reachability algorithm to use and its parameters (required)

Here are the available reachability algorithms and their usages:

- `re_toy <x> <delta>`: RE-toy algorithm
- `bfl <K>`: BFL algorithm
- `grail <t> <ltype> <dim>`: GRAIL algorithm
- `pathtree <alg_type>`: PathTree algorithm
- `tol <style> <opr>`: TOL algorithm
- `ferrari <k> <seeds> <global>`: Ferrari algorithm
- `ip <k> <h> <mu>`: IP algorithm
- `pll <use_RQPrunedLandmarkLabeling>`: PLL algorithm
- `oreach`: O'Reach algorithm

#### Examples

Here are some examples of how to use the commands:

- Run the reachability algorithm RE-toy of specific parameters on a complete DAG with 1000 nodes with a maximum execution time of 100 seconds, and store the results in the specified path:

  ```bash
  ./reachability --time 100 --graph --complete 1000 --algorithm re_toy 32 2.0 --result_file ./result.csv
  ```

- Run tests to validate the correctness of the algorithm PLL on a randomly generated DAG with 100 nodes and an average degree of 3 with a specific seed without limiting the maximum execution time:

  ```bash
  ./reachability --accuracy --graph --random 100 3 --seed 29 --algorithm pll 1
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

## Reproducing Our Experiments

The `experiments.sh` runs all the five experiments mentioned in the paper, namely parameter, tradeoff, real_graph, dense_up, and index_size.

To reproduce our experimental results, please begin by compiling the project and generating the `reachability` executable file. Once complete, you can execute the program using the following command:

```bash
./experiments.sh
```

## Credits

Here are the references and source code origins of the algorithms in this repository. For more information, please refer to the original articles and official source code.

| Algorithm | Reference                                                    | Source Code                                                  |
|-----------| ------------------------------------------------------------ | ------------------------------------------------------------ |
| BFL       | Su J, Zhu Q, Wei H, et al. Reachability querying: Can it be even faster?[J]. IEEE Transactions on Knowledge and Data Engineering, 2016, 29(3): 683-697. | [GitHub](https://github.com/BoleynSu/bfl)                    |
| FERRARI   | Seufert S, Anand A, Bedathur S, et al. Ferrari: Flexible and efficient reachability range assignment for graph indexing[C]//2013 IEEE 29th International Conference on Data Engineering (ICDE). IEEE, 2013: 1009-1020. | [GitHub](https://github.com/steps/Ferrari)                   |
| GRAIL     | Yildirim H, Chaoji V, Zaki M J. Grail: Scalable reachability index for large graphs[J]. Proceedings of the VLDB Endowment, 2010, 3(1-2): 276-284. | [GitHub](https://github.com/zakimjz/grail)                   |
| IP        | Wei H, Yu J X, Lu C, et al. Reachability querying: An independent permutation labeling approach[J]. Proceedings of the VLDB Endowment, 2014, 7(12): 1191-1202.<br />Wei H, Yu J X, Lu C, et al. Reachability querying: an independent permutation labeling approach[J]. The VLDB Journal, 2018, 27: 1-26. | [GitHub](https://github.com/datourat/IP-label-for-graph-reachability) |
| O'Reach   | Hanauer K, Schulz C, Trummer J. O’reach: Even faster reachability in large graphs[J]. ACM Journal of Experimental Algorithmics, 2022, 27: 1-27. | [GitHub](https://github.com/o-reach/O-Reach) |
| Path-Tree | Jin R, Xiang Y, Ruan N, et al. Efficiently answering reachability queries on very large directed graphs[C]//Proceedings of the 2008 ACM SIGMOD international conference on Management of data. 2008: 595-608.<br />Jin R, Ruan N, Xiang Y, et al. Path-tree: An efficient reachability indexing scheme for large directed graphs[J]. ACM Transactions on Database Systems (TODS), 2011, 36(1): 1-44. | [Author Homepage](https://www.cs.kent.edu/%7Enruan/soft.html) |
| PLL       | Yano Y, Akiba T, Iwata Y, et al. Fast and scalable reachability queries on graphs by pruned labeling with landmarks and paths[C]//Proceedings of the 22nd ACM international conference on Information & Knowledge Management. 2013: 1601-1606. | [GitHub](https://github.com/y3eadgbe/PrunedLabeling)         |
| TOL       | Zhu A D, Lin W, Wang S, et al. Reachability queries on large dynamic graphs: a total order approach[C]//Proceedings of the 2014 ACM SIGMOD international conference on Management of data. 2014: 1323-1334. | [SourceForge](https://sourceforge.net/projects/totalorderlabeling/files/) |
