#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
# Copyright 2011 Éwerton Assis
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
import math

import utils

# CVS file header:
#  Instance, Best, Mean, Worst, Interval, Evaluations, "Population Size"

HEADER_KEYS = ("Best of all", "Best mean", "Worst of all", "Populations fitness mean",
  "Population size", "Interval mean", "Evaluations mean")

def update_results(result, values):
  if not result or len(result) == 0:
    final = dict()
    for key, value in values.items():
      local_values = { "Best of all" : value[0], "Best mean" : value[0], "Populations fitness mean" : value[1],
        "Worst of all" : value[2], "Interval mean" : value[3], "Evaluations mean" : value[4],
        "Population size" : value[5] }
      final[key] = local_values
    return final
  for key, value in values.items():
    final_values = dict()
    known_values = result[key]
    local_values = { "Best of all" : value[0], "Best mean" : value[0], "Populations fitness mean" : value[1],
      "Worst of all" : value[2], "Interval mean" : value[3], "Evaluations mean" : value[4],
      "Population size" : value[5] }
    for k, v in known_values.items():
      if k == "Best of all":
        if local_values[k] < v:
          final_values[k] = local_values[k]
        else:
          final_values[k] = v
        continue
      elif k == "Worst of all":
        if local_values[k] > v:
          final_values[k] = local_values[k]
        else:
          final_values[k] = v
        continue
      elif k == "Best mean" or k == "Populations fitness mean" or k == "Interval mean" or k == "Evaluations mean":
        final_values[k] = float(local_values[k] + v) / 2.0
        continue
      else:
        final_values[k] = v
    result[key] = final_values
    del known_values, local_values
  return result

def save_results(results, path, prefix, suffix="results"):
  results_file = open("{0}{1}-{2}.csv".format(path, prefix, suffix), 'w')
  file_keys = sorted(results.keys())
  header_text = ",".join(['"{0}"'.format(header) for header in HEADER_KEYS])
  results_file.write("Instance,{0}\n".format(header_text))
  for key in file_keys:
    known_values = results[key]
    values = ",".join([str(known_values[header]) for header in HEADER_KEYS])
    results_file.write("{0},{1}\n".format(key, values))
  results_file.close()

def process_file(file_name):
  experiment_file = open(file_name)
  experiment_file.readline()
  instances_values = dict()
  for line in experiment_file:
    elements = line.split(',')
    instances_values [elements[0]] = [int(math.fabs(float(element))) for element in elements[1:]]
  return instances_values

def experiments_processor(path, prefix):
  path = utils.folder_forward_slash(path)
  directory_files = os.listdir(path)
  directory_files.sort()
  results = dict()
  for f in directory_files:
    if f.startswith(prefix):
      experiment_values = process_file("{0}{1}".format(path, f))
      results = update_results(results, experiment_values)
  save_results(results, path, prefix)

def process_arguments(arguments):
  path = prefix = None
  for i in range(1, len(arguments)):
    if arguments[i] == '-path' and i + 1 < len(arguments):
      path = arguments[i + 1]
    elif arguments[i] == '-prefix' and i + 1 < len(arguments):
      prefix = arguments[i + 1]
  return(path, prefix)

if __name__ == '__main__':
  path, prefix = process_arguments(sys.argv)
  if os.path.ispath(path):
    experiments_processor(path, prefix)

