import subprocess
import sys
sys.path.append('../')

from CTSNSharedGlobals import *
from Globals import *
import datetime

curlProc = subprocess.Popen(['curl', '-k', '-sL', '-w', '%{http_code}', "https://" + CTSN_DOMAIN, "-o", "/dev/null"],
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

timestamp = datetime.datetime.now()

outputFile = open(statusFile, "w")
outputFile.write('<p><strong>' + statusStr  + '</strong></p>')
outputFile.write("\n<p>Updated at: " + timestamp.date().isoformat() + " " + timestamp.time().isoformat() + ' UTC</p>')
outputFile.close()

