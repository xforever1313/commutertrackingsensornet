import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from CTSNGlobals import *

sethCommonDir = getCommonPath(baseDir)
sys.path.append(sethCommonDir)
from Globals import getRedirectString

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

logDir = os.path.join(thisDir, "logs", sys.argv[1] + "-" + currentTime)
os.makedirs(logDir)

if ('expand_logs=1' in sys.argv):
    expandLogs = True
else:
    expandLogs = False

if ('pi_build=1' in sys.argv):
    del(targets['4picture_parser'])

args = ""

for arg in sys.argv[1:]:
    args += arg
    args += ' '

if (sys.platform == 'win32'):
    sconsCommand = 'scons.bat'
else:
    sconsCommand = 'scons'

for target in sorted(targets.keys()):
    location = targets[target]
    logFile = os.path.join(logDir, target[1:] + ".log")

    print("Running scons " + args + "for " + target[1:])

    process = subprocess.Popen([sconsCommand] + sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=location)
    log = process.communicate()[0]
    f = open(logFile, 'w')
    f.write(log.decode('utf-8'))
    f.close()
    status = process.wait()
    if (status != 0):
        sys.stderr.write('** A compile error occured for target: ' + target[1:] + ".  Please refer to: " + logFile + " for more information **\n")
        if (expandLogs):
            print ("\nPrinting log:")
            f = open(logFile, 'r')
            for line in f:
                print(line)
            f.close()
        exit(5)

