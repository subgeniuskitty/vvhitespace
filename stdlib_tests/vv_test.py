#!/usr/local/bin/python3.6

# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# See LICENSE.txt file for copyright and license details.

# Quick and dirty tests for the VVhitespace stdlib.
# Invoke directly or see README.md in this folder for more details.

# Configuration Options
# All paths are relative to the PWD environment variable of the invoked script.
preprocessor = 'cpp'
include_path = '-I../stdlib'
cpp_temp_file = 'test.pvvs'
compiler_path = '../vvc'
interpreter_path = '../vvi'
temp_file = './test.vvs'
path_to_tests = './'
src_extension = '.pvvs'

# List of tests to perform.
# Tests should be ordered such that later tests rely exclusively on previously tested commands.
# Format: ['filename_without_extension', 'string for stdin', 'string for expected stdout']
tests = [
        ['0001_dumpstack', '', 'TOS:\n2:\t+42\n1:\t+255\n'],
        ['0002_stdlib_version', '', 'TOS:\n1:\t+1\n'],
        ['0003_dumpheap', '', '32:\t+255\n33:\t+42\n'],
        ['1001_stackrotate', '', 'TOS:\n14:\t+1\n13:\t+244\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+243\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+242\n'],
        ['1002_stackrotatereverse', '', 'TOS:\n14:\t+1\n13:\t+244\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+1\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+1\n'],
        ['1003_deepdup', '', 'TOS:\n15:\t+1\n14:\t+244\n13:\t+1\n12:\t+1\n11:\t+1\n10:\t+1\n9:\t+1\n8:\t+1\n7:\t+1\n6:\t+1\n5:\t+1\n4:\t+1\n3:\t+1\n2:\t+1\n1:\t+244\n'],
        ['2001_spew', '', '32:\t+242\n33:\t+243\n34:\t+244\n'],
        ['2002_slurp', '', 'TOS:\n3:\t+242\n2:\t+243\n1:\t+244\n'],
        ['2003_memset', '', '32:\t+42\n33:\t+42\n'],
        ['2004_memcpy', '', '34:\t+42\n35:\t+42\n'],
        ['2005_memcmp', '', '+1'],
        ['2006_memsrch', '', '+32'],
        ['2007_memrand', '', ''],
        ['3001_not', '', '-1+0-6148914691236517206+6148914691236517205'],
        ['3002_lshift', '', '+1+2+4611686018427387904-9223372036854775808-9223372036854775808+0'],
        ['3003_rshift', '', '+1+1+1-4611686018427387903-576460752303423478-1-1'],
        ['3004_and', '', '+0+0+1+1+42'],
        ['3005_or', '', '+0+1+1-1-1'],
        ['3006_xor', '', '+0+1+0-2-1'],
        ['4001_strlen', '', '+11'],
        ['4002_isdigit', '', '+0+1+1+0'],
        ['5001_abs', '', '+1+1+0+0'],
        ['5002_random', '', ''],
        ['5003_gcd', '', '+0+4+4+3+3+3'],
        ['6001_printstackstring', '', 'test'],
        ['6002_printheapstring', '', 'test'],
        ['6003_printnumbersign', '', '+-'],
        ['6004_printnumbermagnitude', '', '323209223372036854775808'],
        ['6005_printstacknumber', '', '+32-32+0-9223372036854775808'],
        ['6006_printf_staticstackstring', '', 'test'],
        ['6007_printf_staticheapstring', '', 'test'],
        ['6008_printf_escapedstackstring', '', '\\%\t\n'],
        ['6009_printf_substitutedstackstring', '', 'Atest142+42'],
        ['7001_atoi', '', '+42+42-42+0+0+0'],
        ] 

# ------------------------------------------------------------------------------

import os, subprocess

for test in tests:
    subprocess.run([preprocessor, include_path, "-o", cpp_temp_file, path_to_tests + test[0] + src_extension])
    subprocess.run([compiler_path, '-i', cpp_temp_file, '-o', temp_file])
    result = subprocess.run([interpreter_path, '-i', temp_file], stdout=subprocess.PIPE, input=test[1].encode('utf-8'))
    if result.stdout.decode('utf-8') != test[2]:
        print('\n' + test[0])
        print('\tExpected: ' + test[2])
        print('\tReceived: ' + result.stdout.decode('utf-8'))
    else:
        print('.', end='', flush=True)
    os.remove(cpp_temp_file)
    os.remove(temp_file)

print("")
