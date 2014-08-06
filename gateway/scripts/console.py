import subprocess
import sys

sys.path.append("../..")

from CTSNSharedGlobals import *
from Secrets import *

def performPostRequest(data, page):
    commandArgs = ['curl', '-X', 'POST', '-A', USER_AGENT, '--data']

    dataStr = ""

    for k,v in data.items():
        dataStr += k + "=" + v + "&"

    #Get rid of unneeded &
    dataStr = dataStr[:-1]

    commandArgs += [dataStr, 'http://localhost:' + str(GATEWAY_COMMAND_PORT) + page]

    subprocess.call(commandArgs) 

def sendUartTx(message):
    data = {}
    data['message'] = message
    performPostRequest(data, UART_TX_URI)

def performShutdown():
    data = {}
    data['shutdown'] = "true"
    performPostRequest(data, SHUTDOWN_URI)

if __name__ == '__main__':
    keepGoing = True
    while (keepGoing):
        command = input("\nEnter a number:\n\t1.  Uart Tx\n\t2.  Send Email\n\t3." + \
                        "  Send Text Message\n\t4.  Shutdown Gateway\n\t0.  Exit\n>")

        if (command == "1"):
            messageToSend = input("\nEnter a message to send:\n>")
            sendUartTx(messageToSend)
        elif (command == "2"):
            print ("Not implemented in console yet.")
        elif (command == "3"):
            print ("Not implemented in console yet.")
        elif (command == "4"):
            performShutdown()
            print ("\n**WARNING!** Any more commands will not work.  Recommend Exiting.")
        elif (command == "0"):
            keepGoing = False

