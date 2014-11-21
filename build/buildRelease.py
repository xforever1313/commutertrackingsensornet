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
from Globals import thisDir, getRevisionNumber

versionFile = os.path.abspath('version.txt')

#See if doxygen exists
try:
    doxygenCheck = subprocess.Popen(['doxygen', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    doxygenStatus = doxygenCheck.wait()
except:
    doxygenStatus = 1

def getVersionString():
    versionFile = open('version.txt', 'r')
    versionString = versionFile.readline().strip()
    versionFile.close()
    try:
        revisionFile = open('revision.txt', 'r')
        versionString += '+' + revisionFile.readline().strip()
        revisionFile.close()
    except:
        versionString += '+' + getRevisionNumber()
    return versionString

versionString = getVersionString() 

args = ['create_lib', 'doxygen', 'version="' + versionString + '"', 'version_file=' + versionFile]
if (doxygenStatus != 0):
    print ("**Warning** Doxygen not detected.  Documentation will not be built, but everything else will")
    args.remove('doxygen')

for arg in sys.argv[1:]:
    if ((arg != 'delta') and (arg != 'nightly')):
        args += [arg]

status = subprocess.call(['python', 'build.py'] + args)
if (status != 0):
    exit(5)
