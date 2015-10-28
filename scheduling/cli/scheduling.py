#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2015 Ewerton Assis
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

import argparse
import sys
import os

try:
    import classes
except ImportError, e:
    project_root = os.path.realpath(os.path.join(os.path.realpath(__file__), '../'))
    sys.path.append(os.path.join(project_root, 'cli'))
    import classes

parser = argparse.ArgumentParser(description='Scheduling problems CLI tool')
subparsers = parser.add_subparsers(dest='parent')
jobshop = subparsers.add_parser('jobshop', help='JobShop Scheduling Problems')
jobshop.add_argument('descriptor', action='store', help='Configuration descriptor file')
jobshop.add_argument('-u', type=int,
    dest='upper_bound', help="Descriptor's experiments array begin slice, inclusive")
jobshop.add_argument('-l', type=int,
    dest='lower_bound', help="Descriptor's experiments array end slice, inclusive")


class SchedulingCLI(object):

    def __init__(self, namespace = None, original_cwd = ''):
        self.original_cwd = original_cwd
        self.namespace = namespace
        self

    def action(self):
        namespace = self.namespace
        function = getattr(classes, namespace.parent)
        return function(experiments_file=namespace.descriptor,
                        start=namespace.upper_bound,
                        stop=namespace.lower_bound)

    @staticmethod
    def apply(argv):
        original_cwd = os.getcwd()
        project_root = os.path.realpath(os.path.join(os.path.realpath(__file__), '../../'))
        os.chdir(project_root)
        namespace = parser.parse_args(argv[1:])
        return SchedulingCLI(namespace, original_cwd).action()


if __name__ == '__main__':
    code = SchedulingCLI.apply(sys.argv)
    sys.exit(code)
