import subprocess
import sys
sys.path.append('../')

from CTSNSharedGlobals import *

curlProc = subprocess.Popen(['curl', '--get', CTSN_DOMAIN], stdout = subprocess.PIPE, stderr=subprocess.PIPE)
status = curlProc.communicate()[0]

if (status == ""):
    print "SERVER DOWN!"
else:
    print "SERVER UP!"
