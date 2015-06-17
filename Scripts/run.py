#!python

import os
import sys
from subprocess import Popen, PIPE

errwrite = lambda x: sys.stderr.write(x + "\n")

temp_dir = "./../temp/"
executable = "./../bin/solver"

if not os.path.isdir(temp_dir):
    os.mkdir(temp_dir)

input_name = temp_dir + "input.txt"
output_name = temp_dir + "output.txt"
log_name = temp_dir + "log.txt"

input = open(input_name, "r+")

errwrite("reading input")

n = int(raw_input())
input.write(str(n) + "\n");
for i in range(4*n):
    s = raw_input()
    input.write(s + "\n")


errwrite("solving problem")

output = open(output_name, "r+")
input.seek(0)
args = {"stdout" : output, "stderr" : sys.stderr, "stdin" : input}
Popen([executable], 
      **args).wait()

input.close()

output.seek(0)
s = output.read()
print s
sys.stdout.flush()
output.close()