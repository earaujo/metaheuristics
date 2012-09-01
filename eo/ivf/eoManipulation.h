#ifndef _eoManipulation_h_
#define _eoManipulation_h_

#include <apply.h>
#include <eoPopEvalFunc.h>
#include <eoOp.h>
#include <eoPop.h>

template<class EOT> class eoManipulation : public eoOp<EOT>
{
  public:
    eoManipulation() : eoOp<EOT>(eoOp<EOT>::general) { }

    virtual void operator()(eoEvalFunc<EOT> &, const eoPop<EOT> &, const EOT &, EOT &) = 0;
};

template<class EOT> class eoGenManip : public eoManipulation<EOT>
{
  public:
    eoGenManip(unsigned _num_points = 1) : num_points(_num_points)
      {
        if (num_points < 1)
            std::runtime_error("eoGenManip: invalid number of points");
      }

    virtual std::string className() const { return "eoGenManip"; }

    virtual void operator()(eoEvalFunc<EOT> & _eval, const eoPop<EOT> & _mothers, const EOT & _father, EOT & _son)
      {
        EOT father = _father;
        eoPop<EOT> mothers = _mothers, super_individuals;
        unsigned chrom_size = father.size();
        std::vector<int> points;

        points.resize(num_points + 1);
        points[num_points] = chrom_size;
        for (unsigned i = 0; i < num_points; i++)
          {
            unsigned begin = i * (chrom_size / (num_points + 1));
            unsigned end = (i + 1) * (chrom_size / (num_points + 1));
            points[i] = eo::rng.random(end - begin) + begin;
          }

        while (true)
          {
            super_individuals.clear();
            for (unsigned m = 0; m < mothers.size(); m++)
              {
                EOT mother, first_child, second_child;
                mother = mothers[m];
                first_child.resize(chrom_size);
                unsigned deslocation = 0;
                for (unsigned point = 0; point < num_points + 1; point++)
                  {
                    for (unsigned i = deslocation; i < points[point]; i++)
                      if (point % 2) /* Mother */
                        first_child[i] = mother[i];
                      else /* Father */
                        first_child[i] = father[i];
                    deslocation = points[point];
                  }
                super_individuals.push_back(first_child);
                second_child.resize(chrom_size);
                deslocation = 0;
                for (unsigned point = 0; point < num_points + 1; point++)
                  {
                    for (unsigned i = deslocation; i < points[point]; i++)
                      if (point % 2) /* Father */
                        second_child[i] = father[i];
                      else /* Mother */
                        second_child[i] = mother[i];
                    deslocation = points[point];
                  }
                super_individuals.push_back(second_child);
              }

            apply(_eval, super_individuals);
            _eval(father);
            if (super_individuals.best_element().fitness() <= father.fitness())
              {
                _son = father;
                break;
              }
            father = super_individuals.best_element();
            _son = super_individuals.best_element();
          }
      }

  private:
    unsigned num_points;
};

#endif

