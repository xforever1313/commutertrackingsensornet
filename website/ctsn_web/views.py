from django.shortcuts import render
from django.shortcuts import render_to_response
from django.template import RequestContext
from ctsn_web.models import *

NavBarURLs = [('Home', 'index.html'), ('Node Status', 'node_status.html'),
              ('Statistics', 'node_stats.html'),
              ('Admin', 'admin_messages.html')]

AdminBarURLs = [('Messages', 'admin_messages.html'),
              ('Maintenance', 'admin_maintenance.html')]

def HomeView(request):
    context = { 'NavBarURLs' : NavBarURLs, 'pageID' : NavBarURLs[0][1], 
                'title' : 'CTSN Home' }
    return render_to_response(NavBarURLs[0][1], context, context_instance=RequestContext(request))

def AdminMessageView(request):
    errorMessages = ErrorLog.objects.all()
    context = { 'NavBarURLs' : NavBarURLs, 'pageID' : NavBarURLs[3][1],
                'title' : 'CTSN Trail Node Log', 'AdminBarURLs' : AdminBarURLs,
                'errorMessages' : errorMessages }

    return render_to_response(NavBarURLs[3][1], context, context_instance=RequestContext(request))

def NodeStatusView(request):
    context = {'NavBarURLs' : NavBarURLs, 'pageID' : NavBarURLs[1][1], 
               'title' : 'CTSN Node Status',
               'nodes' : Node.objects.all()}

    return render_to_response(NavBarURLs[1][1], context, context_instance=RequestContext(request))
