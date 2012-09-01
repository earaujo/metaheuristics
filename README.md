
# About: Metaheuristics Research Algorithms

Version 0.0.9 // Copyright 2012 Éwerton Assis (earaujoassis at gmail dot com)

Please refer to the included **LICENCE file** for terms of use.

Each folder has different purposes:

* **eo**: it contains an extension for the *Evolving Objects* (EO) Library, written in C++; that extension is a hybridization module for Genetic
Algorithms, named In Vitro Fertilization Module (IVFm), or In Vitro Fertilization Genetic Algorithm (IVF-GA). That folder also contains an
application code, an algorithm that uses IVF-GA as a solver to some Multidimensional 0-1 Knapsack instances. (For more information about *Evolving Objects*:
http://eodev.sourceforge.net/)

* **evolve**: it contains a C library with some metaheuristics implemented.

* **grad-texts**: it contains some of the texts related to these projects I've been developing for my graduation program. In that folder there's
a README.markdown file where you can find more details. Those files has their own LICENSE scheme.

* **problems**: it contains application code for some well known problems in Computer Science, using the "evolve" code, noted above. These are:
the Multidimensional 0-1 Knapsack problem and the Travelling Salesman Problem (TSP). Each problem class has some instances for results analysis
and algorithm tuning, but they are not mine and should be disregarded from the LICENSE scope.

* **scheduling**: it contains application code for some well known scheduling problem classes, mainly Flow-shop and Job-shop. Each one has its
problem instances files, used for results analysis and algorithm tuning. Those instances files are not mine and should be disregarded from the
LICENSE scope. That application code is also written upon the "evolve" code noted above.

## Dependencies

 * GSL (GNU Scientific Library) -- http://www.gnu.org/software/gsl/
