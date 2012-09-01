#ifndef _eoKnapsackAlter_h
#define _eoKnapsackAlter_h

#include <eoTransform.h>
#include <utils/eoRNG.h>

template<class EOT> class eoKnapsackTransform : public eoTransform<EOT>
{
  public:
    eoKnapsackTransform(eoQuadOp<EOT>& _crossover, double _crossoverRate, eoMonOp<EOT>& _mutate, double _mutationRate):
      crossover(_crossover), crossoverRate(_crossoverRate), mutate(_mutate), mutationRate(_mutationRate) { }

    eoKnapsackTransform(eoQuadOp<EOT>& _crossover, double * _crossoverRate, eoMonOp<EOT>& _mutate, double * _mutationRate):
      crossover(_crossover), crossoverRate(*_crossoverRate), mutate(_mutate), mutationRate(*_mutationRate) { }

    virtual std::string className() const { return "eoKnapsackTransform"; }

    void operator()(eoPop<EOT>& _pop)
      {
        unsigned int i;
        for (i = 0; i < _pop.size()/2; i++)
          if (eo::rng.flip(crossoverRate))
            crossover(_pop[2*i], _pop[2*i+1]);
        for (i = 0; i < _pop.size(); i++)
          if (eo::rng.flip(mutationRate))
            mutate(_pop[i]);
      }

  private:
    eoInvalidateQuadOp<EOT> crossover;
    eoInvalidateMonOp<EOT> mutate;
    double crossoverRate;
    double mutationRate;
};

#endif

