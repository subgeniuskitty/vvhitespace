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
        ['0001_dumpstack', '', 'TOS:\n2:\t+42\n1:\t+255\n'],
        ['0002_dumpheap', '', '32:\t+255\n33:\t+42\n'],
        ['1001_stackrotate', '', 'TOS:\n14:\t+1\n13:\t+244\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+243\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+242\n'],
        ['1002_stackrotatereverse', '', 'TOS:\n14:\t+1\n13:\t+244\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+1\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+1\n'],
        ['1003_deepdup', '', 'TOS:\n15:\t+1\n14:\t+244\n13:\t+1\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+1\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+244\n'],
        ['2001_spew', '', '32:\t+242\n33:\t+243\n34:\t+244\n'],
        ['2002_slurp', '', 'TOS:\n3:\t+242\n2:\t+243\n1:\t+244\n'],
        ['2003_memset', '', '32:\t+42\n33:\t+42\n'],
        ['2004_memcpy', '', '34:\t+42\n35:\t+42\n'],
        ['2005_memcmp', '', '+1'],
        ] 

for test in tests:
    subprocess.run([preprocessor, include_path, "-o", cpp_temp_file, path_to_tests + test[0] + src_extension])
    subprocess.run([compiler_path, '-i', cpp_temp_file, '-o', temp_file])
    result = subprocess.run([interpreter_path, '-i', temp_file], stdout=subprocess.PIPE, input=test[1].encode('utf-8'))
    if result.stdout.decode('utf-8') != test[2]:
        print('\n' + test[0])
    else:
        print('.', end='', flush=True)
    os.remove(cpp_temp_file)
    os.remove(temp_file)

print("")
