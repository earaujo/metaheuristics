#ifndef _eoKnapsackEval_h
#define _eoKnapsackEval_h

#include <eoEvalFunc.h>

template <class EOT> class eoKnapsackEvalFunc : public eoEvalFunc<EOT>
{
  public:
    eoKnapsackEvalFunc(std::vector<int> _values, std::vector< std::vector <int> > _weights,
      std::vector<int> _capacity): values(_values), weights(_weights), capacity(_capacity) { }

    virtual std::string className() const { return "eoKnapsackEvalFunc"; }

    void operator()(EOT & _individual)
      {
        if(_individual.invalid())
          {
            double fitness;
            fitness = 0;
            repair(_individual);
            for(unsigned int i = 0; i < _individual.size(); i++)
                fitness += _individual[i] * values[i];
            _individual.fitness(fitness);
          }
      }

 private:
    std::vector<int> values;
    std::vector< std::vector <int> > weights;
    std::vector<int> capacity;

    void repair(EOT & _individual)
      {
        while (not is_subjected(_individual))
          mutate(_individual);
      }

    bool is_subjected(EOT & _individual)
      {
        for (int compartment = 0; compartment < capacity.size(); compartment++)
          {
            long total_weight = 0;
            for (int item = 0; item < _individual.size(); item++)
              total_weight += _individual[item] * weights[compartment][item];
            if (total_weight > capacity[compartment])
              return false;
          }
        return true;
      }

    /* FIXME Função obtida do Vitor com fins de comparação (tempo de execução) */
    bool is_valid(EOT & _individual)
      {
        bool valid = true;
        std::vector<int> cost;

        cost.resize(capacity.size());
        for (unsigned i = 0; i < cost.size(); i++)
          cost[i] = 0;
        for (unsigned i = 0; i < _individual.size(); i++)
          if (_individual[i] == 1)
            for (unsigned j = 0; j < cost.size(); j++)
              cost[j] += weights[j][i];
        for (unsigned i = 0; i < cost.size(); i++)
          if (cost[i] > capacity[i])
            valid = false;
        return valid;
      }

    void mutate(EOT & _individual)
      {
        int index;
        index = eo::rng.random(_individual.size() - 1);
        while (_individual[index] != 1)
          index = eo::rng.random(_individual.size() - 1);
        _individual[index] = 0;
      }
};

#endif

