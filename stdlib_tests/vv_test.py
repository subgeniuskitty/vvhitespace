#!/usr/local/bin/python3.6

# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

# Quick and dirty tests for the VVhitespace stdlib.

import os, subprocess

preprocessor = 'cpp'
include_path = '-I../stdlib'
cpp_temp_file = 'test.pvvs'
compiler_path = '../vvc'
interpreter_path = '../vvi'
temp_file = './test.vvs'
path_to_tests = './'
src_extension = '.pvvs'

tests = [
        # Format: ['filename_without_extension', 'string for stdin', 'string for expected stdout']
        ['0001_hello_world', '', 'Hello, world!\n'],
        ] 

for test in tests:
    subprocess.run([preprocessor, include_path, "-o", cpp_temp_file, path_to_tests + test[0] + src_extension])
    subprocess.run([compiler_path, '-i', cpp_temp_file, '-o', temp_file])
    result = subprocess.run([interpreter_path, '-i', temp_file], stdout=subprocess.PIPE, input=test[1].encode('utf-8'))
    if result.stdout.decode('utf-8') != test[2]:
        print('\n' + test[0])
    else:
        print('.', end='', flush=True)
    os.remove(temp_file)

print("")