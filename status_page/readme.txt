This is for the "Status" webpage so external users can tell if the site is now or not.

Instructions:
1. Ensure some webserver (e.g Apache) is installed
2. Run python install.py
    This will create a "www" folder in the home directory if one does not exist
    This will also create a "ctsnstatus" folder in "www" as well.
    The default index.html file will go in here too.
    A default status.html file will also go in here.
3. Add a cron job that will querty the CTSN main server
    crontab -e
    5,10,15,20,25,30,35,40,45,50,55, * * * * cd /path/To/CTSN/repo/status_page/ && python CheckCTSNStatus.py &> Update.log
    0 * * * * * cd /path/To/CTSN/repo/ && git pull &> pull.log
    
    This will run the updater every 5 minutes, and do a clone of the repo every hour, in case there's a change.
    
4. Wherever the server's "root directory" is, add a symlink from the root, and point it to ~/www/ctsnstatus, so its viewable to from the webpage.
