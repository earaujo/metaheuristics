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
from processor import HEADER_KEYS

IVF_RESULTS = "ivf-or-library-test-results.csv"
GA_RESULTS = "ga-or-library-test-results.csv"

def save_final_results(ivf_results, ga_results, path, fname="final_results"):
  results_file = open("{0}{1}.csv".format(path, fname), 'w')
  file_keys = sorted(ivf_results.keys())
  header_names = list()
  for header_name in HEADER_KEYS:
    header_names += ["{0} {1}".format(header_name, "(IVF)")]
    header_names += ["{0} {1}".format(header_name, "(GA)")]
  header_text = ",".join(['"{0}"'.format(header) for header in header_names])
  del header_names
  results_file.write("Instance,{0}\n".format(header_text))
  for key in file_keys:
    ivf_values = ivf_results[key]
    ga_values = ga_results[key]
    final_values = list()
    for header_name in HEADER_KEYS:
      final_values += [ivf_values[header_name]]
      final_values += [ga_values[header_name]]
    values = ",".join([str(value) for value in final_values])
    del final_values, ga_values, ivf_values
    results_file.write("{0},{1}\n".format(key, values))
  results_file.close()

def process_file(file_name):
  results_file = open(file_name)
  results_file.readline()
  results_dict = dict()
  for line in results_file:
    elements = line.split(',')
    values = [int(float(element)) for element in elements[1:]]
    local_values = { "Best of all" : values[0], "Best mean" : values[1], "Worst of all" : values[2], "Populations fitness mean" : values[3],
                     "Population size" : values[4], "Interval mean" : values[5], "Evaluations mean" : values[6] }
    results_dict[elements[0]] = local_values
  return results_dict

def results_processor(path):
  path = utils.folder_forward_slash(path)
  ivf_results = process_file("{0}{1}".format(path, IVF_RESULTS))
  ga_results = process_file("{0}{1}".format(path, GA_RESULTS))
  save_final_results(ivf_results, ga_results, path)

def process_arguments(arguments):
  path = None
  for i in range(1, len(arguments)):
    if arguments[i] == '-path' and i + 1 < len(arguments):
      path = arguments[i + 1]
  if os.path.ispath(path):
    return path
  return None

if __name__ == "__main__":
  path = process_arguments(sys.argv)
  if path:
    results_processor(path)

