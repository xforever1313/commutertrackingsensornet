import os

homePath = os.environ['HOME']
wwwPath = os.path.join(homePath, 'www')
ctsnStatusPath = os.path.join(wwwPath, 'ctsnstatus')
statusFile = os.path.join(ctsnStatusPath, 'status.txt')
indexFile = os.path.join(ctsnStatusPath, 'index.shtml')

