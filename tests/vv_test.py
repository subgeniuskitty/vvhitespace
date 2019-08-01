#!/usr/local/bin/python3.6

# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

# Quick and dirty tests for the VVhitespace interpreter.

import os, subprocess

compiler_path = '../vvc'
interpreter_path = '../vvi'
temp_file = './test.vvs'
path_to_tests = './'
src_extension = '.pvvs'

tests = [
        # Format: ['filename_without_extension', 'string for stdin', 'string for expected stdout']
        ['0001_push_printchar_exit', '', 'A'],
        ['1001_stack_push', '', 'A'],
        ['1002_stack_dup', '', 'AA'],
        ['1003_stack_swap', '', 'AB'],
        ['1004_stack_drop', '', 'A'],
        ['2001_arithmetic_addition', '', 'B'],
        ['2002_arithmetic_subtraction', '', 'A'],
        ['2003_arithmetic_multiplication', '', 'B'],
        ['2004_arithmetic_division', '', 'A'],
        ['2005_arithmetic_remainder', '', 'A113'],
        ['3001_heap', '', 'BCA'],
        ['4001_flowcontrol_exit', '', ''],
        ['4002_flowcontrol_unconditional_jump_to_label', '', 'A'],
        ['4003_flowcontrol_jump_if_tos_is_zero', '', 'A'],
        ['4004_flowcontrol_jump_if_tos_is_negative', '', 'A'],
        ['4005_flowcontrol_jump_to_subroutine', '', 'A'],
        ['4006_flowcontrol_return_from_subroutine', '', 'A'],
        ['4100_flowcontrol_branches_not_taken', '', 'A'],
        ['5001_io_output_character', '', 'A'],
        ['5002_io_output_digit', '', '2'],
        ['5003_io_input_character', 'A', 'A'],
        ['5004_io_input_digit', '1', '1'],
        ['6001_push_intmin', '', '1'],
        ] 

for test in tests:
    subprocess.run([compiler_path, '-i', path_to_tests + test[0] + src_extension, '-o', temp_file])
    result = subprocess.run([interpreter_path, '-i', temp_file], stdout=subprocess.PIPE, input=test[1].encode('utf-8'))
    if result.stdout.decode('utf-8') != test[2]:
        print('\n' + test[0])
    else:
        print('.', end='', flush=True)
    os.remove(temp_file)

print("")
