#ifndef _eoFertilization_h_
#define _eoFertilization_h_

#include <eoOp.h>

#include <ivf/eoCollect.h>
#include <ivf/eoManipulation.h>
#include <ivf/eoTransference.h>

template<class EOT> class eoFertilization : public eoOp<EOT>
{
  public:
    eoFertilization(eoCollect<EOT>& _collect, eoManipulation<EOT>& _manipulation, eoTransference<EOT>& _transference):
        eoOp<EOT>(eoOp<EOT>::general), collect(_collect), manipulation(_manipulation), transference(_transference) { }

    virtual std::string className() const { return "eoFertilization"; }

    virtual void operator()(eoEvalFunc<EOT> & _eval, eoPop<EOT> & _old, eoPop<EOT> & _young)
      {
        EOT embryo, *father_reference, father;
        eoPop<EOT> possible_mothers;
        eoPop<EOT> mothers_selected;

        father_reference = &(*_old.it_best_element());
        father = (*father_reference);

        _old.sort();
        possible_mothers.clear();
        mothers_selected.clear();
        possible_mothers.resize(_old.size() - 1);
        for (unsigned i = 1; i < (_old.size() - 1); i++)
            possible_mothers[i] = _old[i];
        apply(_eval, possible_mothers);
        collect(possible_mothers, mothers_selected);

        manipulation(_eval, mothers_selected, father, embryo);
        transference(_young, father_reference, embryo);

        apply(_eval, _young);
        /* FIXME Used for debug
        std::cout << "Best  : " << _young.best_element() << std::endl; // Best element from actual generation
        std::cout << "Father: " << father << std::endl;
        std::cout << "Embryo: " << embryo << std::endl << std::endl;
        */
      }

  private:
    eoCollect<EOT>& collect;
    eoManipulation<EOT>& manipulation;
    eoTransference<EOT>& transference;
};

#endif

