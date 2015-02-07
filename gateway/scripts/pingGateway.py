#/usr/bin/python3
import subprocess
import mysql.connector
import sys

from Secrets import *
sys.path.append ("../..")
from CTSNSharedGlobals import *

db = mysql.connector.connect(user = mariadb_user,
                             password = mariadb_pass,
                             host = mariadb_ip,
                             database = mariadb_name)

curlProc = subprocess.Popen(['curl', '-sL', '-w', '%{http_code}', '-A', gateway_user_agent, "http://localhost:" + str(mariadb_name), "-o", "/dev/null"], 
                            stdout = subprocess.PIPE, stderr=subprocess.PIPE)

status = int(curlProc.communicate()[0])

cursor = db.cursor()

#If running...
if (status == 200):
    cursor.execute("UPDATE node SET status=1,update_time=NOW() WHERE id=1;")

#If down...
else:
    cursor.execute("UPDATE node SET status=8,update_time=NOW() WHERE id=1;")

db.commit()
cursor.close()
db.close()

