from django.shortcuts import render
from django.shortcuts import render_to_response
from django.template import RequestContext

NavBarURLs = [('Home', 'index.html'), ('Node Status', 'node_status.html'),
              ('Statistics', 'node_stats.html'),
              ('Admin', 'admin_messages.html')]

def HomeView(request):
    context = { 'NavBarURLs' : NavBarURLs, 'pageID' : NavBarURLs[0][1], 
                'title' : 'CTSN Home' }
    return render_to_response(NavBarURLs[0][1], context, context_instance=RequestContext(request))

