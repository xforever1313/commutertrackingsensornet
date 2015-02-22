import os
import subprocess
import sys

baseDir = "../"

sys.path.append(baseDir)

from CTSNGlobals import *

# Update Repo
subprocess.call(['git', 'pull'])

# Update CTSNCommon Doxygen
subprocess.call(['scons', 'doxygen'], cwd=getCTSNCommonPath(baseDir))

# Update gateway doxygen
subprocess.call(['scons', 'doxygen'], cwd=getGatewayPath(baseDir))

# Update pi node doxygen
subprocess.call(['scons', 'doxygen'], cwd=getPiNodePath(baseDir))


