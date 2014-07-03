import subprocess
import sys
sys.path.append('../')

from CTSNSharedGlobals import *
from Globals import *

curlProc = subprocess.Popen(['curl', '-sL', '-w', '%{http_code}', CTSN_DOMAIN, "-o", "/dev/null"], 
                            stdout = subprocess.PIPE, stderr=subprocess.PIPE)
status = int(curlProc.communicate()[0])

if (status == 200):
    statusStr = "Ok"
elif (status == 503):
    statusStr = "Down for maintenance"
elif (status == 404):
    statusStr = "Website Down"
else:
    statusStr = "Down"

outputFile = open(statusFile, "w")
outputFile.write(statusStr)
outputFile.close()

