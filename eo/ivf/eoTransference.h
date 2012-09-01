#ifndef _eoTransference_h_
#define _eoTransference_h_

#include <eoPop.h>

template<class EOType> class eoTransference : public eoOp<EOType>
{
  public:
    eoTransference() : eoOp<EOType>(eoOp<EOType>::general) { }

    virtual void operator()(eoPop<EOType> &, EOType *, EOType &) = 0;
};

template<class EOT> class eoParentSubstitution : public eoTransference<EOT>
{
  public:
    virtual std::string className() const { return "eoParentSubstitution"; }

    virtual void operator()(eoPop<EOT> & _pop, EOT * _father, EOT & _embryo)
      {
        typename eoPop<EOT>::iterator best_one = _pop.it_best_element();
        while (best_one != _pop.end())
          {
            /* FIXME Aparentemente o ponteiro que referencia o pai na geração anterior
             * deixa de existir na nova população; pelo visto a nova população é composta
             * de novas instâncias (alocações de memória). A substituição acaba sendo
             * feita por fitness.
             */
            if ((*_father) == (*best_one))
              {
                _father = &_embryo;
                return;
              }
            best_one++;
          }
      }
};

template<class EOT> class eoWorstSubstitution : public eoTransference<EOT>
{
  public:
    virtual std::string className() const { return "eoWorstSubstitution"; }

    virtual void operator()(eoPop<EOT> & _pop, EOT * _father, EOT & _embryo)
      {
        typename eoPop<EOT>::iterator worst_one = _pop.it_worse_element();
        (*worst_one) = _embryo;
      }
};

template<class EOT> class eoRandomSubstitution : public eoTransference<EOT>
{
  public:
    virtual std::string className() const { return "eoRandomSubstitution"; }

    virtual void operator()(eoPop<EOT> & _pop, EOT * _father, EOT & _embryo)
      {
        _pop[eo::rng.random(_pop.size()-1)] = _embryo;
      }
};

#endif

