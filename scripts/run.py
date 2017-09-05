import sys
import os
from optparse import OptionParser

parser = OptionParser(usage="usage: %prog --version=%executable")
parser.add_option("--version", dest="version", type="string")

(options, args) = parser.parse_args()

if options.version:
    VERSION = options.version
else:
    parser.error("version not specified")


command = "java -jar ./CirclesSeparationVis.jar -exec './../bin/" + VERSION + " -d' -seed 0"

os.system(command)
