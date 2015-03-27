#include <iostream>
#include <cstring>

#include <knapsack/eoKnapsack.h>

int main(int argc, char *argv[])
{
    try
      {
        std::vector<long> bestest, averages;
        bool using_easy_ea = false;
        unsigned instances;

        if (argc > 1)
            if (not std::strcmp(argv[1], "--easy")) // If argv[1] == "--easy"
                using_easy_ea = true;

        rng.reseed(time(0));
        std::cin >> instances;
        for (unsigned i = 0; i < instances; i++)
          {
            std::cout.width(2);
            std::cout << i << ": ";
            KnapsackProblem<Individual> problem(std::cin);
            if (using_easy_ea)
              {
                AlgorithmEasyEA<Individual> algorithm(problem);
                algorithm();
                bestest = algorithm.statistical_data().best_values();
                averages = algorithm.statistical_data().average_values();
              }
            else
              {
                AlgorithmIVFGA<Individual> algorithm(problem);
                algorithm();
                bestest = algorithm.statistical_data().best_values();
                averages = algorithm.statistical_data().average_values();
              }

            std::cout.width(13);
            std::cout << "first best: ";
            std::cout << bestest[0];
            std::cout.width(17);
            std::cout << "first average: ";
            std::cout << averages[0];
            std::cout.width(13);
            std::cout << "last best: ";
            std::cout << bestest[bestest.size()-1];
            std::cout.width(16);
            std::cout << "last average: ";
            std::cout << averages[averages.size()-1] << std::endl;
          }
      }
    catch(std::exception& e)
      {
        std::cout << "Exception: " << e.what() << "\n" << std::endl;
        return 1;
      }
    return 0;
}

