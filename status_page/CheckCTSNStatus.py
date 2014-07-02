import subprocess
import sys
sys.path.append('../')

from CTSNSharedGlobals import *
from Globals import *

curlProc = subprocess.Popen(['curl', '--get', CTSN_DOMAIN], stdout = subprocess.PIPE, stderr=subprocess.PIPE)
status = curlProc.communicate()[0]

if (status == ""):
    status = "Down"
else:
    status = "Ok"

outputFile = open(statusFile, "w")
outputFile.write(status)
outputFile.close()

