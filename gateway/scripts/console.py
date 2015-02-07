import argparse
import time
import subprocess
import sys

from Secrets import *

sys.path.append("../..")

from CTSNSharedGlobals import *

gatewayUrl = ""

def performPostRequest(data, page):
    commandArgs = ['curl', '-X', 'POST', '-A', gateway_user_agent, '--data']

    dataStr = ""

    for k,v in data.items():
        dataStr += k + "=" + v + "&"

    #Get rid of unneeded &
    dataStr = dataStr[:-1]

    commandArgs += [dataStr, 'http://' + gatewayUrl + ":" + str(gateway_port) + page]

    print ("\nCurl output:")
    subprocess.call(commandArgs) 

def sendUartTx(message):
    data = {}
    data['message'] = message
    performPostRequest(data, UART_TX_URI)

def sendData(dataType):
    data = {}
    data['node'] = "1" #Gateway node is 1
    data['type'] = dataType
    performPostRequest(data, DATA_RESULT_URI)

def sendXBeeMessage(message, nodeNumber):
    data = {}
    data['message'] = message
    data['node'] = nodeNumber
    performPostRequest(data, XBEE_TX_URI)

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
 
def logTestMessage():
    data = {}
    data['node'] = '1'    #Gateway node is node 1
    data['message'] = '2' #Error message is TEST_ERROR

    performPostRequest(data, LOG_MESSAGE_URI)

def sendErrorMessage():
    data = {}
    data['node'] = '1'    #Gateway node is node 1
    data['message'] = '2' #Error message is TEST_ERROR

    performPostRequest(data, ERROR_MESSAGE_URI)

def pokeDatabase():
    data = {}
    data['poke'] = 'true'

    performPostRequest(data, DATABASE_POKE_URI)

def updateNodeStatus(node, status):
    data = {}
    data['node'] = node
    data['status'] = status
    performPostRequest(data, NODE_STATUS_UPDATE_URI)

def nodeCheck():
    data = {}
    data['check'] = 'true'
    performPostRequest(data, NODE_CHECK_URI)

def performData(nodeID, picturePart, pictureData):
    data = {}
    data['node'] = nodeID
    data['part'] = picturePart
    data['data'] = pictureData
    performPostRequest(data, DATA_URI)

def performEncodedDataSend(nodeID, dataFile):
    f = open(dataFile, 'r')
    fileContents = ""
    for line in f:
        fileContents += line
    f.close()

    data1 = {}
    data1['node'] = nodeID
    data1['part'] = '1'
    data1['data'] = fileContents[:int(len(fileContents) / 2)]
    performPostRequest(data1, DATA_URI)

    data2 = {}
    data2['node'] = nodeID
    data2['part'] = '2'
    data2['data'] = fileContents[int(len(fileContents) / 2):]
    performPostRequest(data2, DATA_URI)

    data3 = {}
    data3['node'] = nodeID
    data3['part'] = '0'
    data3['data'] = 'derp'
    performPostRequest(data3, DATA_URI)

def performEncodedDataSendWithXBee(nodeID, dataFile):
    f = open(dataFile, 'r')
    i = 1
    for line in f:
        data = "node=" + nodeID + "|part=" + str(i) + "|data=" + line
        sendXBeeMessage(DATA_URI + "\t" + data, "1") #Can only send this to gateway.
        i += 1
        time.sleep(0.5)
    f.close()

    data = "node=" + nodeID + "|part=0|data=derp" 
    sendXBeeMessage(DATA_URI + "\t" + data, "1") #Can only send this to gateway.

def sendStressTest(iterations):
    messageToSend = DATA_URI + "\tnode=2|part=1|data="
    i = 0
    while i < int(iterations):
        messageToSend += "a"
        i += 1

    sendXBeeMessage(messageToSend, "1")
    print ("Number of bytes: " + str(len(messageToSend)))

if __name__ == '__main__':
    argParser = argparse.ArgumentParser(description="Debug Console for the CTSN gateway")
    argParser.add_argument("--url", dest='url', action = "store", default="localhost", help="The url to post to.")
    args = argParser.parse_args()

    global gatewayUrl
    gatewayUrl = args.url

    keepGoing = True
    while (keepGoing):
        command = input("\nEnter a number:\n\t1.  Uart Tx\n\t2.  Send Email\n\t3." + \
                        "  Send Text Message\n\t4.  Shutdown Gateway\n\t5.  Log Test Message\n\t6.  Send Error Message\n\t" + \
                        "7.  Poke Database\n\t8.  Send XBee Tx\n\t9.  Send Result\n\t" +\
                        "10  Send HTTP over XBee\n\t" + \
                        "11. Change Node Status\n\t12. Node Check\n\t13. Send data\n\t" + \
                        "14. Send encoded file\n\t15. Send encoded file over XBee\n\t" + \
                        "16. Send Bytes Stress Test\n\t0.  Exit\n>")

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
        elif (command == "5"):
            logTestMessage()
        elif (command == "6"):
            sendErrorMessage()
        elif (command == "7"):
            pokeDatabase()
        elif (command == "8"):
            messageToSend = input("\nEnter a message to send:\n>")
            nodeNumber = input("\nEnter a node number:\n>")
            sendXBeeMessage(messageToSend, nodeNumber)
        elif (command == "9"):
            dataType = input("\nEnter a data type (walker-2, biker-3, horse-4) as an int\n>")
            sendData(dataType)
        elif (command == "10"):
            uri = input("\nEnter uri: (remember the '/' before it)\n>")
            data = input ("\nEnter the data (formatted like the http post header, but with | instead of &)\n>")
            sendXBeeMessage(uri + "\t" + data, "1") #Can only send this to gateway.
        elif (command == "11"):
            nodeNumber = input("\nEnter a node number> ")
            status = input("\nEnter a status as an int> ")
            updateNodeStatus(nodeNumber, status)
        elif (command == "12"):
            nodeCheck()
        elif (command == "13"):
            nodeID = input ("Enter Node ID> ")
            picturePart = input("Enter Picture Part> ")
            pictureData = input("Enter encoded picture data> ")
            performData(nodeID, picturePart, pictureData)
        elif (command == "14"):
            nodeID = input ("Enter Node ID> ")
            data = input ("Enter file location> ")
            performEncodedDataSend(nodeID, data)
        elif (command == "15"):
            nodeID = input ("Enter Node ID> ")
            data = input ("Enter file location> ")
            performEncodedDataSendWithXBee(nodeID, data)
        elif (command == "16"):
            numberOfBytes = input("Enter number of bytes to send> ")
            sendStressTest(numberOfBytes)
        elif (command == "0"):
            keepGoing = False

