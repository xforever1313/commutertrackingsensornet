import argparse
import subprocess
import sys

sys.path.append("../..")

from CTSNSharedGlobals import *
from Secrets import *

gatewayUrl = ""

def performPostRequest(data, page):
    commandArgs = ['curl', '-X', 'POST', '-A', USER_AGENT, '--data']

    dataStr = ""

    for k,v in data.items():
        dataStr += k + "=" + v + "&"

    #Get rid of unneeded &
    dataStr = dataStr[:-1]

    commandArgs += [dataStr, 'http://' + gatewayUrl + ":" + str(PICTURE_PARSER_COMMAND_PORT) + page]

    print ("\nCurl output:")
    subprocess.call(commandArgs) 

def performShutdown():
    data = {}
    data['shutdown'] = "true"
    performPostRequest(data, SHUTDOWN_URI)

if __name__ == '__main__':
    argParser = argparse.ArgumentParser(description="Debug Console for the CTSN picture parser")
    argParser.add_argument("--url", dest='url', action = "store", default="localhost", help="The url to post to.")
    args = argParser.parse_args()

    global gatewayUrl
    gatewayUrl = args.url

    keepGoing = True
    while (keepGoing):
        command = input("\nEnter a number:\n\t1.  Shutdown Picture Parser\n\t0.  Exit\n>")

        if (command == "1"):
            performShutdown()
            print ("\nPicture Parser shutting down, anymore commands won't work")
        elif (command == "0"):
            keepGoing = False
