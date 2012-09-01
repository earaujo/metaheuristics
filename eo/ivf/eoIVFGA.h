#ifndef _eoIVFGA_h_
#define _eoIVFGA_h_

#include <apply.h>
#include <eoAlgo.h>
#include <eoPopEvalFunc.h>
#include <eoContinue.h>
#include <eoSelect.h>
#include <eoTransform.h>
#include <eoBreed.h>
#include <utils/eoLogger.h>
#include <eoReplacement.h>

#include <ivf/eoFertilization.h>

template<class EOT> class eoIVFGA : public eoAlgo<EOT>
{
  public:
    eoIVFGA(eoContinue<EOT> & _continuator,
        eoEvalFunc<EOT> & _eval,
        eoSelect<EOT> & _select,
        eoTransform<EOT> & _transform,
        eoReplacement<EOT> & _replace,
        eoCollect<EOT> & _collect,
        eoManipulation<EOT> & _manipulation,
        eoTransference<EOT> & _transference):
            continuator(_continuator),
            eval(_eval),
            selectTransform(_select, _transform),
            breed(selectTransform),
            replace(_replace),
            fertilization(_collect, _manipulation, _transference)
    { }

    virtual std::string className() const { return "eoIVFGA"; }

    virtual void operator()(eoPop<EOT> & _pop)
      {
        unsigned population_size;
        eoPop<EOT> offspring, empty_pop, old_pop;
        apply(eval, _pop);
        do
          {
            try
              {
                population_size = _pop.size();
                offspring.clear();

                breed(_pop, offspring);
                apply(eval, offspring);

                old_pop = _pop;
                replace(_pop, offspring);
                fertilization(eval, old_pop, _pop);
                apply(eval, _pop);

                if (population_size > _pop.size())
                    throw std::runtime_error("Population shrinking!");
                else if (population_size < _pop.size())
                    throw std::runtime_error("Population growing");
              }
            catch(std::exception & e)
              {
                std::string s = e.what();
                s.append(" in eoIVFGA");
                throw std::runtime_error(s);
              }
          }
        while(continuator(_pop));
      }

  protected:
    eoContinue<EOT>& continuator;
    eoEvalFunc<EOT>& eval;
    eoSelectTransform<EOT> selectTransform;
    eoBreed<EOT>& breed;
    eoReplacement<EOT>& replace;
    eoFertilization<EOT> fertilization;
};

#endif

