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

    commandArgs += [dataStr, 'http://' + gatewayUrl + ":" + str(GATEWAY_COMMAND_PORT) + page]

    print ("\nCurl output:")
    subprocess.call(commandArgs) 

def sendUartTx(message):
    data = {}
    data['message'] = message
    performPostRequest(data, UART_TX_URI)

def performShutdown():
    data = {}
    data['shutdown'] = "true"
    performPostRequest(data, SHUTDOWN_URI)

def performTextMessage(numbers, providers, subject, message):
    data = {}
    data['numbers'] = ""
    data['providers'] = ""
    data['subject'] = subject
    data['message'] = message

    for num in numbers:
        data['numbers'] += num + ','
    data['numbers'] = data['numbers'][:-1]

    for p in providers:
        data['providers'] += p + ','
    data['providers'] = data['providers'][:-1]

    performPostRequest(data, TEXT_MESSAGE_URI)

def getTextMessageInfo():
    subject = input ("Subject: ")
    message = input ("Message to send: ")

    numbers = []
    providers = []

    try:
        while True:
            number = input ("Give a phone number (xxxyyyzzzz), eof to stop: ")
            provider = input("Select provider:\n1. ATT\n2. Verizon\n3. Tmobile\n4. Sprint\n5. Virgin Mobile\n6. US Cellular" + \
                             "\n7. Nextel\n8. Boost\n9. Alltel\n> ")

            numbers += [number]
            providers += [provider]
        
    except EOFError:
        pass

    performTextMessage(numbers, providers, subject, message)
    
def performEmail(addresses, names, subject, message):
    data = {}
    data['addresses'] = ""
    data['names'] = ""
    data['subject'] = subject
    data['message'] = message

    for addr in addresses:
        data['addresses'] += addr + ','
    data['addresses'] = data['addresses'][:-1]

    for name in names:
        data['names'] += name + ','
    data['names'] = data['names'][:-1]

    performPostRequest(data, EMAIL_URI)

def getEmailInfo():
    subject = input ("Subject: ")
    message = input ("Message to send: ")

    addresses = []
    names = []

    try:
        while True:
            address = input ("Give an email (someone@something.com), eof to stop: ")
            name = input("Enter Recipient Name: ")

            addresses += [address]
            names += [name]
        
    except EOFError:
        pass

    performEmail(addresses, names, subject, message)
 
if __name__ == '__main__':
    argParser = argparse.ArgumentParser(description="Debug Console for the CTSN gateway")
    argParser.add_argument("--url", dest='url', action = "store", default="localhost", help="The url to post to.")
    args = argParser.parse_args()

    global gatewayUrl
    gatewayUrl = args.url

    keepGoing = True
    while (keepGoing):
        command = input("\nEnter a number:\n\t1.  Uart Tx\n\t2.  Send Email\n\t3." + \
                        "  Send Text Message\n\t4.  Shutdown Gateway\n\t0.  Exit\n>")

        if (command == "1"):
            messageToSend = input("\nEnter a message to send:\n>")
            sendUartTx(messageToSend)
        elif (command == "2"):
            getEmailInfo()
        elif (command == "3"):
            getTextMessageInfo()
        elif (command == "4"):
            performShutdown()
            print ("\n**WARNING!** Any more commands will not work.  Recommend Exiting.")
        elif (command == "0"):
            keepGoing = False

