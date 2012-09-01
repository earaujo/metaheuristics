#ifndef _eoKnapsackSolution_h_
#define _eoKnapsackSolution_h_

#include <eo>
#include <ga.h>

#include <ivf/eoIVFGA.h>

#include <knapsack/eoKnapsackEval.h>
#include <knapsack/eoKnapsackInit.h>
#include <knapsack/eoKnapsackAlter.h>
#include <knapsack/eoKnapsackStats.h>

typedef eoBit<long> Individual;

template<class Individual> class KnapsackProblem
{
  public:
    KnapsackProblem(std::vector<int> & _values, std::vector< std::vector<int> > & _weights,
        std::vector<int> & _capacity): items_values(_values), items_weights(_weights),
        compartments_capacity(_capacity) { }

    KnapsackProblem(std::istream & input)
      {
        unsigned instances, items, compartments, collector;
        std::vector<int> values, capacity;
        std::vector< std::vector<int> > weights;

        if (not input.good())
            std::runtime_error("cannot process input stream");

        values.clear();
        capacity.clear();
        weights.clear();

        input >> items >> compartments >> collector;
        if ((not items) or (not compartments))
            return;
        for (unsigned i = 0; i < items; i++)
          {
            input >> collector;
            values.push_back(collector);
          }
        for (unsigned i = 0; i < compartments; i++)
          {
            std::vector<int> aux;
            for (int j = 0; j < items; j++)
              {
                input >> collector;
                aux.push_back(collector);
              }
            weights.push_back(aux);
          }
        for (unsigned i = 0; i < compartments; i++)
          {
            input >> collector;
            capacity.push_back(collector);
          }
        items_values = values;
        items_weights = weights;
        compartments_capacity = capacity;
      }

    const std::vector<int> values() const
      {
        return items_values;
      }

    const std::vector<int> capacity() const
      {
        return compartments_capacity;
      }

    const std::vector< std::vector<int> > weights() const
      {
        return items_weights;
      }

  private:
    std::vector<int> items_values;
    std::vector<int> compartments_capacity;
    std::vector< std::vector<int> > items_weights;
};

template<class Individual> class Algorithm
{
  public:
    Algorithm(KnapsackProblem<Individual> _problem,
        unsigned _populationSize = 500, unsigned _maxGeneration= 500, unsigned _tournamentSize = 10,
        double _mutationRate = 0.015, double _crossoverRate = 0.75): problem(_problem),
            populationSize(_populationSize), maxGeneration(_maxGeneration), tournamentSize(_tournamentSize),
            mutationRate(_mutationRate), crossoverRate(_crossoverRate), chromosomeSize(_problem.values().size()) { }

    virtual void operator()(void) = 0;

    eoKnapsackStats<Individual> statistical_data()
      {
        return statistics;
      }

  protected:
    unsigned chromosomeSize, populationSize, maxGeneration, tournamentSize;
    double crossoverRate, mutationRate;
    eoKnapsackStats<Individual> statistics;
    KnapsackProblem<Individual> problem;
};

template<class Individual> class AlgorithmEasyEA : public Algorithm<Individual>
{
  public:
    AlgorithmEasyEA(KnapsackProblem<Individual> _problem,
        unsigned _populationSize = 500, unsigned _maxGeneration = 500, unsigned _tournamentSize = 10,
        double _mutationRate = 0.015, double _crossoverRate = 0.75):
            Algorithm<Individual>::Algorithm(_problem, _populationSize, _maxGeneration, _tournamentSize, _mutationRate, _crossoverRate) { }

    virtual void operator()(void)
      {
        eoKnapsackEvalFunc<Individual> evaluation(Algorithm<Individual>::problem.values(),
            Algorithm<Individual>::problem.weights(),
            Algorithm<Individual>::problem.capacity());
        eoKnapsackInit<Individual> initialisation(Algorithm<Individual>::chromosomeSize);
        eoPop<Individual> population(Algorithm<Individual>::populationSize, initialisation);

        eoDetTournamentSelect<Individual> selectTournament(Algorithm<Individual>::tournamentSize);
        eoSelectPerc<Individual> select(selectTournament);
        eoBitMutation<Individual> mutation(Algorithm<Individual>::mutationRate);
        eo1PtBitXover<Individual> crossover;
        eoGenContinue<Individual> continuator(Algorithm<Individual>::maxGeneration);

        eoKnapsackTransform<Individual> transform(crossover, Algorithm<Individual>::crossoverRate,
            mutation, Algorithm<Individual>::mutationRate);
        eoGenerationalReplacement<Individual> replace;
        eoWeakElitistReplacement<Individual> elitism(replace);

        eoCheckPoint<Individual> checkpoint(continuator);
        checkpoint.add(Algorithm<Individual>::statistics);

        eoEasyEA<Individual> genetic_algorithm(checkpoint, evaluation, select, transform, elitism);
        genetic_algorithm(population);
      }
};

template<class Individual> class AlgorithmIVFGA : public Algorithm<Individual>
{
  public:
    AlgorithmIVFGA(KnapsackProblem<Individual> _problem,
        unsigned _populationSize = 500, unsigned _maxGeneration = 500, unsigned _tournamentSize = 10,
        double _mutationRate = 0.015, double _crossoverRate = 0.75):
            Algorithm<Individual>::Algorithm(_problem, _populationSize, _maxGeneration, _tournamentSize, _mutationRate, _crossoverRate) { }

    virtual void operator()(void)
      {
        eoKnapsackEvalFunc<Individual> evaluation(Algorithm<Individual>::problem.values(),
            Algorithm<Individual>::problem.weights(),
            Algorithm<Individual>::problem.capacity());
        eoKnapsackInit<Individual> initialisation(Algorithm<Individual>::chromosomeSize);
        eoPop<Individual> population(Algorithm<Individual>::populationSize, initialisation);

        eoDetTournamentSelect<Individual> selectTournament(Algorithm<Individual>::tournamentSize);
        eoSelectPerc<Individual> select(selectTournament);
        eoBitMutation<Individual> mutation(Algorithm<Individual>::mutationRate);
        eo1PtBitXover<Individual> crossover;
        eoGenContinue<Individual> continuator(Algorithm<Individual>::maxGeneration);

        eoSGATransform<Individual> transform(crossover, Algorithm<Individual>::crossoverRate,
            mutation, Algorithm<Individual>::mutationRate);
        eoGenerationalReplacement<Individual> replace;
        eoWeakElitistReplacement<Individual> elitism(replace);

        eoSelectBestFitness<Individual> collect;
        eoGenManip<Individual> manipulation(4);
        eoWorstSubstitution<Individual> transference;

        eoCheckPoint<Individual> checkpoint(continuator);
        checkpoint.add(Algorithm<Individual>::statistics);

        eoIVFGA<Individual> genetic_algorithm(checkpoint, evaluation, select, transform,
            elitism, collect, manipulation, transference);
        genetic_algorithm(population);
      }
};

/*
void process_file(std::istream & input)
{
    unsigned instances, items, compartments, collector;
    std::vector<int> values, capacity;
    std::vector< std::vector<int> > weights;

    if (not input.good())
        return;

    input >> instances;
    std::cout << instances << std::endl;
    for (unsigned counter = 0; counter < instances; counter++)
      {
        values.clear();
        capacity.clear();
        weights.clear();

        input >> items >> compartments >> collector;
        std::cout << items << " " << compartments << std::endl;
        if ((not items) or (not compartments))
            return;
        for (unsigned i = 0; i < items; i++)
          {
            input >> collector;
            values.push_back(collector);
          }
        for (unsigned i = 0; i < compartments; i++)
          {
            std::vector<int> aux;
            for (int j = 0; j < items; j++)
              {
                input >> collector;
                aux.push_back(collector);
              }
            weights.push_back(aux);
          }
        for (unsigned i = 0; i < compartments; i++)
          {
            input >> collector;
            capacity.push_back(collector);
          }
        std::cout << std::endl;
      }
}
*/

#endif

