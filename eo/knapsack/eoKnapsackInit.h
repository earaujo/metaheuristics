#ifndef _eoKnapsackInit_h
#define _eoKnapsackInit_h

#include <eoInit.h>
#include <utils/eoRNG.h>

template <class EOT> class eoKnapsackInit : public eoInit<EOT>
{
  public:
    typedef typename EOT::AtomType AtomType;

    eoKnapsackInit(unsigned _chromosome_size) : chromosome_size(_chromosome_size) { }

    virtual std::string className() const { return "eoKnapsackInit"; }

    virtual void operator()(EOT & chrom)
      {
        chrom.resize(chromosome_size);
        for (unsigned gene = 0; gene < chromosome_size; gene++)
          chrom[gene] = eo::rng.flip();
        chrom.invalidate();
      }

  private:
    unsigned chromosome_size;
};

#endif

