#ifndef _eoKnapsackStats_h_
#define _eoKnapsackStats_h_

#include <eoPop.h>
#include <utils/eoStat.h>

template<class EOT> class eoKnapsackStats : public eoStatBase<EOT>
{
  public:
    typedef typename EOT::Fitness Fitness;

    virtual std::string className(void) const { return "eoKnapsackStats"; }

    virtual void lastCall(const eoPop<EOT> &) { }

    virtual void operator()(const eoPop<EOT> & _pop)
      {
        Fitness average_value;

        best.push_back(_pop.best_element().fitness());
        worst.push_back(_pop.worse_element().fitness());

        average_value = 0;
        for (unsigned i = 0; i < _pop.size(); i++)
            average_value += _pop[i].fitness();
        average_value /= _pop.size();
        average.push_back(average_value);
      }

    const std::vector<Fitness> average_values()
      {
        return average;
      }

    const std::vector<Fitness> best_values()
      {
        return best;
      }

    const Fitness best_value()
      {
        Fitness best_value = 0;
        for (unsigned i = 0; i < best.size(); i++)
            best_value = best[i] > best_value ? best[i] : best_value;
        return best_value;
      }

    const Fitness worst_value()
      {
        Fitness worst_value = worst[0];
        for (unsigned i = 0; i < worst.size(); i++)
            worst_value = worst[i] < worst_value ? worst[i] : worst_value;
        return worst_value;
      }

    void reset()
      {
        average.clear();
        best.clear();
        worst.clear();
      }

  private:
    std::vector<Fitness> average;
    std::vector<Fitness> best;
    std::vector<Fitness> worst;
};

#endif

