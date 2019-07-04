#!/usr/local/bin/python3.6

# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

# Quick and dirty tests for the VVhitespace interpreter.

import os, subprocess

compiler_path = './vvc'
interpreter_path = './vvi'
temp_file = './test.vvs'
path_to_tests = './tests/'
src_extension = '.pvvs'

tests = [
        ['0001_push_printchar_exit', 'A'],
        ['1001_stack_operations', 'BB']
        ] 

for test in tests:
    subprocess.run([compiler_path, '-i', path_to_tests + test[0] + src_extension, '-o', temp_file])
    result = subprocess.run([interpreter_path, '-i', temp_file], stdout=subprocess.PIPE)
    if result.stdout.decode('utf-8') != test[1]:
        print(test[0])
    else:
        print('.', end='')
    os.remove(temp_file)

print("")
