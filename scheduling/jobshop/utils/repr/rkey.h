// Copyright 2011-2015 Ewerton Assis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __jobshop_repr_h__
#define __jobshop_repr_h__

#include <evolve/evolve_common.h>
#include <evolve/evolve_rng.h>
#include <evolve/evolve_randomkey.h>
#include <evolve/evolve_rkgaes.h>
#include <evolve/evolve_ivfrkgaes.h>
#include <evolve/evolve_o_rkgaes.h>
#include "criteria.h"

static jobshop_t *global_problem;
static size_t global_evaluations_counted;

unsigned int *
jobs_indexes (evolve_real_chrom_t *chrom)
{
  size_t i;
  size_t dimension = global_problem->n_jobs * global_problem->m_machines;
  unsigned int *integer_serie = evolve_integer_series ((unsigned int) dimension, chrom);
  unsigned int *operations = (unsigned int *) calloc (dimension, sizeof (unsigned int));
  for (i = 0; i < dimension; i++)
    operations[i] = (integer_serie[i] % global_problem->n_jobs) + 1;
  free (integer_serie);
  return operations;
}

double
fitness (evolve_real_chrom_t *chrom)
{
  unsigned int *jobs_index = jobs_indexes (chrom);
  long int makespan_value = makespan (jobs_index, global_problem);
  free (jobs_index);
  global_evaluations_counted++;
  return (double) - makespan_value;
}

evolve_stats_t *
executor (jobshop_t *problem,
          algorithm_t alg_type)
{
  evolve_real_pop_t *population;
  evolve_stats_t *stats;
  size_t dimension, interval;

  global_problem = problem;
  dimension = problem->n_jobs * problem->m_machines;
  population = evolve_init_real_pop (POP_SIZE);
  evolve_set_rng (SEED);
  evolve_random_real_pop (population, dimension, 0, dimension, &evolve_mock_check);
  global_evaluations_counted = 0;
  if (alg_type == ivf)
    {
      interval = time (NULL);
      stats = evolve_ivfrkgaes (population, MAX_GEN, &fitness, &evolve_mock_check);
      interval = time (NULL) - interval;
      stats->time = interval;
    }
  else
    {
      interval = time (NULL);
      stats = evolve_rkgaes (population, MAX_GEN, &fitness, &evolve_mock_check);
      interval = time (NULL) - interval;
      stats->time = interval;
    }
  evolve_tear_rng ();
  stats->evaluations_counted = global_evaluations_counted;
  stats->population_size = population->size;
  evolve_del_real_pop_indiv (population);
  evolve_del_real_pop (population);
  return stats;
}

#endif

