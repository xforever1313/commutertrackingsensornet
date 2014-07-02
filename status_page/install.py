import os
import shutil

from Globals import *

if (not os.path.exists(wwwPath)):
    os.mkdir(wwwPath)

if (not os.path.exists(ctsnStatusPath)):
    os.mkdir(ctsnStatusPath)

if (not os.path.exists(statusFile)):
    f = open(statusFile, 'w')
    f.write('Unknown')
    f.close()

shutil.copy('index.shtml', indexFile)

print ("Done! Now make sure your server is pointing to ~/www/ctsnstatus")

