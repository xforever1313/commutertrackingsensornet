import datetime
import os
import subprocess
import sys

from Secrets import *

date = datetime.datetime.now()
dateStr = date.strftime("%d_%m_%y_%X")

backupDir = os.path.join(os.environ['HOME'], 'backups', 'ctsn')

if (not os.path.exists(backupDir)):
    os.makedirs(backupDir)

process = subprocess.Popen(['mysqldump', '-u', mariadb_user, '-p' + mariadb_pass, '--host=' + mariadb_ip, mariadb_name], stdout=subprocess.PIPE)

stdout = process.communicate()[0]

outfile = open(os.path.join(backupDir, 'ctsn_' + dateStr + '.sql'), 'w')
outfile.write(stdout)
outfile.close()

