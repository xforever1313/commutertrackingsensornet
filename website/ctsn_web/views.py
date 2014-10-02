from django.shortcuts import render
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.contrib.auth import authenticate, login, logout
from django.http import HttpResponseRedirect, HttpResponse
from ctsn_web.models import *
from ctsn_web.forms import *

NavBarURLs = [('Home', 'index.html'), ('Node Status', 'node_status.html'),
              ('Statistics', 'node_stats.html'),
              ('Admin', 'admin_messages.html')]

AdminBarURLs = [('Messages', 'admin_messages.html'),
              ('Maintenance', 'admin_maintenance.html')]

def HomeView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
        return HttpResponseRedirect('login.html')

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1]

    context = { 'NavBarURLs' : urls, 'pageID' : NavBarURLs[0][1], 
                'title' : 'CTSN Home' }
    return render_to_response(NavBarURLs[0][1], context, context_instance=RequestContext(request))

def AdminMessageView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    elif not request.user.is_staff:
        return HttpResponse('Unauthorized', status=401)
    
    errorMessages = ErrorLog.objects.all()
    context = { 'NavBarURLs' : NavBarURLs, 'pageID' : NavBarURLs[3][1],
                'title' : 'CTSN Trail Node Log', 'AdminBarURLs' : AdminBarURLs,
                'errorMessages' : errorMessages }

    return render_to_response(NavBarURLs[3][1], context, context_instance=RequestContext(request))

def NodeStatusView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1] #Do not show admin link.

    context = {'NavBarURLs' : urls, 'pageID' : NavBarURLs[1][1], 
               'title' : 'CTSN Node Status',
               'nodes' : Node.objects.all()}

    return render_to_response(NavBarURLs[1][1], context, context_instance=RequestContext(request))

def NodeStatsView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1] #Do not show admin link.

    context = {'NavBarURLs' : urls, 'pageID' : NavBarURLs[2][1], 
               'title' : 'CTSN Statistics',
               'nodes' : Node.objects.all()}

    return render_to_response(NavBarURLs[2][1], context, context_instance=RequestContext(request))

def AdminMaintenanceView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    elif not request.user.is_staff:
        return HttpResponse('Unauthorized', status=401)

    context = {'NavBarURLs' : NavBarURLs, 'pageID' : AdminBarURLs[1][1], 
               'title' : 'CTSN Node Maintenance', 'AdminBarURLs' : AdminBarURLs,
               'nodes' : Node.objects.all()}

    return render_to_response(AdminBarURLs[1][1], context, context_instance=RequestContext(request))

###
# Auth Stuff
###
def LoginRequest(request):
    #If already autheticated, return to home
    if request.user.is_authenticated():
       return HttpResponseRedirect('/')
   
    #If submitting the form...
    if (request.method == 'POST'):
        form = LoginForm(request.POST)
        if (form.is_valid()):
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            ctsnUser = authenticate(username = username, password=password)
            if ctsnUser is not None:
                login(request, ctsnUser)
                return HttpResponseRedirect('/') 
            else:
                return render_to_response('login.html', {'form':form}, context_instance=RequestContext(request))

        else: #Form invalid
            return render_to_response('login.html', {'form':form}, context_instance=RequestContext(request))

    else: #User is not submitting form, but get requesting.  Show the form
        form = LoginForm()
        context = {'form' : form}
        return render_to_response('login.html', context, context_instance=RequestContext(request)) 

def LogoutRequest(request):
    logout(request)
    return HttpResponseRedirect('login.html')

def WindbeltResultPage(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    elif not request.user.is_staff:
        return HttpResponse('Unauthorized', status=401)

    context = {'windbelt_results' : WindbeltResult.objects.all()}
    return render_to_response('windbelt_result.html', context, context_instance=RequestContext(request))

