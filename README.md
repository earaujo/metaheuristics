
# Metaheuristics Research Algorithms

Each folder has a different purpose:

* **eo**: it contains an extension for the *Evolving Objects* (EO) Library, written in C++; that extension is a hybridization module for Genetic
Algorithms, named In Vitro Fertilization Module (IVFm), or In Vitro Fertilization Genetic Algorithm (IVF-GA). That folder also contains an
application code, an algorithm that uses IVF-GA as a solver to some Multidimensional 0-1 Knapsack instances. (For more information about *Evolving Objects*:
http://eodev.sourceforge.net/)

* **problems**: it contains application code for some well known problems in Computer Science, using the "evolve" code, noted above. These are:
the Multidimensional 0-1 Knapsack problem and the Travelling Salesman Problem (TSP). Each problem class has some instances for results analysis
and algorithm tuning, but they are not mine and should be disregarded from the LICENSE scope.

* **scheduling**: it contains application code for some well known scheduling problem classes, mainly Flow-shop and Job-shop. Each one has its
problem instances files, used for results analysis and algorithm tuning. Those instances files are not mine and should be disregarded from the
LICENSE scope. That application code is also written upon the "evolve" code noted above.

## Dependencies

 * [Evolve](http://evolvecode.org/)

## License

Please refer to the included *LICENSE file* for terms of use.

Apache License, Version 2.0. Copyright 2011-2014 &copy; Ewerton Assis.
