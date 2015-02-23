from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

import os

def getLibb64Path(baseDir):
    return os.path.join(baseDir, 'dependencies/libb64')

def getCTSNCommonPath(baseDir):
    return os.path.join(baseDir, 'common')

def getCommonPath(baseDir):
    return os.path.join(baseDir, 'sethcommon')

def getPiNodePath(baseDir):
    return os.path.join(baseDir, 'pi_node')

def getGatewayPath(baseDir):
    return os.path.join(baseDir, 'gateway')

