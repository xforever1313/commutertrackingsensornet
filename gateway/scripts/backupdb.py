import datetime
import os
import subprocess
import sys

sys.path.append("../../")
from Secrets import *

date = datetime.datetime.now()
dateStr = date.strftime("%d_%m_%y_%X")

backupDir = os.path.join(os.environ['HOME'], 'backups', 'ctsn')

if (not os.path.exists(backupDir)):
    os.makedirs(backupDir)

process = subprocess.Popen(['mysqldump', '-u', MARIADB_USER, '-p' + MARIADB_PASSWORD, '--host=' + GATEWAY_LOCAL_IP, 'ctsn'], stdout=subprocess.PIPE)

stdout = process.communicate()[0]

outfile = open(os.path.join(backupDir, 'ctsn_' + dateStr + '.sql'), 'w')
outfile.write(stdout)
outfile.close()

