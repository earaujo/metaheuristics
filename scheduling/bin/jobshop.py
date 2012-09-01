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
import subprocess
import json
import datetime

path = os.path.join(os.path.realpath(globals()["__file__"]), '../../jobshop/suite/')
sys.path.append(os.path.realpath(path))

import utils
import processor

def process_experiment(experiment, times, instance_folder, output_folder):
  experiment_options = ["-r", experiment["recombination"], "-s", experiment["selection"], "-m", experiment["mutation"]]
  instances_file = experiment["instances"]
  folder_name = experiment["folder_name"]
  file_prefix = experiment["file_prefix"]
  instances_file = instance_folder + instances_file
  folder_name = output_folder + folder_name
  folder_name = utils.folder_forward_slash(folder_name)
  for algorithm in experiment["algorithms"]:
    algorithm_argument = "--{0}".format(algorithm)
    for i in range(times):
      output_file = "{0}{1}-{2}{3:02}.csv".format(folder_name, algorithm, file_prefix, i)
      arguments = ["bin/jobshop", algorithm_argument, "-f", instances_file, "-o", output_file]
      arguments = arguments + experiment_options
      subprocess.call(arguments)
#     print(" ".join(arguments))
      del output_file, arguments
    prefix = "{0}-{1}".format(algorithm, file_prefix)
    processor.experiments_processor(folder_name, prefix)
    del prefix

def process_arguments(arguments):
  experiments = start = stop = None
  for i in range(1, len(arguments)):
    if arguments[i] == '-experiments':
      if i + 1 < len(arguments):
        experiments = arguments[i + 1]
    elif arguments[i] == '-start':
      if i + 1 < len(arguments):
        start = int(arguments[i + 1])
    elif arguments[i] == '-stop':
      if i + 1 < len(arguments):
        stop = int(arguments[i + 1])
  return (experiments, start, stop)

def main(argv):
  experiments_file, start, stop = process_arguments(argv)
  if not experiments_file or not os.path.isfile(experiments_file):
    sys.exit(-1)
  experiments_file = open(experiments_file)
  descriptor = json.load(experiments_file)
  experiments_file.close()
  experiments = descriptor["experiments"]
  times = descriptor["times"]
  instances_folder = descriptor["instances_folder"]
  instances_folder = utils.folder_forward_slash(instances_folder)
  output_folder = descriptor["output_folder"]
  output_folder = utils.folder_forward_slash(output_folder)
  selected_experiments = None
  if start and stop:
    selected_experiments = experiments[start:stop]
  elif start and not stop:
    selected_experiments = experiments[start:]
  elif not start and stop:
    selected_experiments = experiments[:stop]
  else:
    selected_experiments = experiments
  for experiment in selected_experiments:
    process_experiment(experiment, times, instances_folder, output_folder)
  
if __name__ == '__main__':
  print(datetime.datetime.now())
  main(sys.argv)
  print(datetime.datetime.now())

