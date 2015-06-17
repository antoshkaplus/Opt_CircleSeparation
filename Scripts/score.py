
import os
import sys
from subprocess import Popen, PIPE
from tempfile import TemporaryFile

try:
    N = int(sys.argv[1])
except:
    sys.stderr.write("Unable to read test count to run" + "\n")
    raise

# solver redirects errors to standard output stream
for i in range(N):
    temp = TemporaryFile("r+t")
    args = {"stdout" : temp, "stderr" : None, "stdin" : None}
    Popen(["java", "-jar", "CirclesSeparationVis.jar", "-exec", "python run.py", "-novis", "-seed", str(i)], 
      **args).wait()
    temp.seek(0)
    out = temp.read()
    ss = out.split("\n")
    score = next(float(s.split("=")[1]) for s in ss if s.startswith("Score"))
    sys.stderr.write(out)
    sys.stdout.write(str(score) + "\n")
    