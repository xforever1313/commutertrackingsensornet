#/usr/bin/python3
import subprocess
import mysql.connector
import sys

sys.path.append ("../..")
from CTSNSharedGlobals import *
from Secrets import *

db = mysql.connector.connect(user = MARIADB_USER,
                             password = MARIADB_PASSWORD,
                             host = "localhost",
                             database = "ctsn")

curlProc = subprocess.Popen(['curl', '-sL', '-w', '%{http_code}', "http://localhost:" + str(GATEWAY_COMMAND_PORT), "-o", "/dev/null"], 
                            stdout = subprocess.PIPE, stderr=subprocess.PIPE)

status = int(curlProc.communicate()[0])

cursor = db.cursor()

#If running...
if (status == 200):
    cursor.execute("UPDATE node SET status=1 WHERE id=1;")

#If down...
else:
    cursor.execute("UPDATE node SET status=8 WHERE id=1;")

db.commit()
cursor.close()
db.close()

