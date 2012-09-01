#ifndef _eoCollect_h_
#define _eoCollect_h_

#include <eoPop.h>
#include <eoFunctor.h>

template<class EOT> class eoCollect : public eoBF<eoPop<EOT>&, eoPop<EOT>&, void>
{
  public:
    virtual void operator()(eoPop<EOT> &, eoPop<EOT> &) = 0;
};

template<class EOT> class eoSelectBestFitness : public eoCollect<EOT>
{
  public:
    eoSelectBestFitness(double _percentage = 0.3) : percentage(_percentage) { }

    virtual std::string className() const { return "eoSelectBestFitness"; }

    virtual void operator()(eoPop<EOT>& _source, eoPop<EOT>& _dest)
      {
        /* Father was already selected by default in eoFertilization operator */
        unsigned pop_size = _source.size();
        unsigned selected_number = (pop_size * percentage);
        _source.sort();
        _dest.resize(selected_number);
        for (unsigned i = 0; i < selected_number; i++)
            _dest[i] = _source[i];
        _dest.sort();
      }

  private:
    double percentage;
};

#endif

