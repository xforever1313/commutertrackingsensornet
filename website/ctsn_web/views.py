from django.shortcuts import render
from django.shortcuts import render_to_response
from django.template import RequestContext, loader, Context
from django.contrib.auth import authenticate, login, logout
from django.http import HttpResponseRedirect, HttpResponse
from ctsn_web.models import *
from ctsn_web.forms import *

NavBarURLs = [('Home', 'index.html'), ('Node Status', 'node_status.html'),
              ('Statistics', 'node_stats.html'),
              ('Admin', 'admin_messages.html')]

AdminBarURLs = [('Messages', 'admin_messages.html'),
              ('Maintenance', 'admin_maintenance.html')]

# Used to get the status of the nodes.
class CtsnStatus():
    def __init__(self):
        self.nodes = Node.objects.all()
        self.gatewayNode = self.nodes.get(id=1)
        self.trailNodes = self.nodes.exclude(id=1)

    def getGatewayNodeStatus(self):
        return self.gatewayNode.status

    def getTrailNodeStatus(self):
        returnStatus = StatusSeverity.objects.get(id=1)

        for node in self.trailNodes:
            if (node.status.severity.id > returnStatus.id and \
                node.status.severity.id < 6):

                returnStatus = node.status.severity

        return returnStatus

#Checks to see if the website is in maintenance mode
def isMaintenance(user = None):
    if (user == None):
        pass
    elif (user.is_staff):
        return False

    website = Website.objects.get(id=1)
    return website.status.id == 2

#Returns a response that displays the "We are in maintance" page
def getMaintenanceResponse():
    t = loader.get_template('maintenance.html')
    c = Context({})
    return HttpResponse(t.render(c), content_type="text/html", status=503)

def HomeView(request):
    #If in maintenance mode, return 503.
    if isMaintenance(request.user):
        return getMaintenanceResponse()
    #If not authenticated, return to login
    elif not request.user.is_authenticated():
        return HttpResponseRedirect('login.html')

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1]

    context = { 'NavBarURLs' : urls, 'pageID' : NavBarURLs[0][1], 
                'title' : 'CTSN Home', 'status' : CtsnStatus(),
                'isMaintenance' : isMaintenance()}
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
                'errorMessages' : errorMessages, 'status' : CtsnStatus() ,
                'isMaintenance' : isMaintenance()}

    return render_to_response(NavBarURLs[3][1], context, context_instance=RequestContext(request))

def NodeStatusView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    #If in maintenance mode, return 503.
    elif isMaintenance(request.user):
        return getMaintenanceResponse()

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1] #Do not show admin link.

    context = {'NavBarURLs' : urls, 'pageID' : NavBarURLs[1][1], 
               'title' : 'CTSN Node Status',
               'nodes' : Node.objects.all(), 'status' : CtsnStatus(),
               'isMaintenance' : isMaintenance()}

    return render_to_response(NavBarURLs[1][1], context, context_instance=RequestContext(request))

class Result:
    def __init__(self, node):
        self.node = node
        self.results = {}
        self.nodeTotal = 0

    def addResult(self, result_type):
        self.results[result_type] = TrailResult.objects.filter(node=self.node, type=result_type).count()
        self.nodeTotal += self.results[result_type]

def NodeStatsView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    #If in maintenance mode, return 503.
    elif isMaintenance(request.user):
        return getMaintenanceResponse()

    if request.user.is_staff:
        urls = NavBarURLs
    else:
        urls = NavBarURLs[:-1] #Do not show admin link.

    results = {}
    resultTypeTotals = {}
    grandTotal = 0
    for node in Node.objects.all():
        results[node] = Result(node)
        for result_type in TrailResultType.objects.all():
            results[node].addResult(result_type)
            try:
                resultTypeTotals[result_type] += results[node].results[result_type]
            except KeyError:
                resultTypeTotals[result_type] = results[node].results[result_type]
            grandTotal += results[node].results[result_type]

    context = {'NavBarURLs' : urls, 'pageID' : NavBarURLs[2][1], 
               'title' : 'CTSN Statistics',
               'nodes' : Node.objects.all(), 
               'status' : CtsnStatus(),
               'isMaintenance' : isMaintenance(),
               'result_type_objects' : TrailResultType.objects.all(),
               'results' : results,
               'result_type_totals' : resultTypeTotals,
               'grand_total' : grandTotal}

    return render_to_response(NavBarURLs[2][1], context, context_instance=RequestContext(request))

def AdminMaintenanceView(request):
    #If not authenticated, return to login
    if not request.user.is_authenticated():
       return HttpResponseRedirect('login.html')
    elif not request.user.is_staff:
        return HttpResponse('Unauthorized', status=401)

    if (request.method == 'POST'):
        if ('disable_maintenance' in request.POST):
            website = Website.objects.get(id=1)
            website.status = WebsiteStatus.objects.get(id=1)
            website.save()

            errorMessage = ErrorMessages.objects.get(id=4)
            query = ErrorLog(node=Node.objects.get(id=1), message=errorMessage)
            query.save()
        elif ('enable_maintenance' in request.POST):
            website = Website.objects.get(id=1)
            website.status = WebsiteStatus.objects.get(id=2)
            website.save()

            errorMessage = ErrorMessages.objects.get(id=3)
            query = ErrorLog(node=Node.objects.get(id=1), message=errorMessage)
            query.save()

    context = {'NavBarURLs' : NavBarURLs, 'pageID' : AdminBarURLs[1][1], 
               'title' : 'CTSN Node Maintenance', 'AdminBarURLs' : AdminBarURLs,
               'nodes' : Node.objects.all(), 'status' : CtsnStatus(),
               'isMaintenance' : isMaintenance() }

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

def MaintenanceView(request):
    t = loader.get_template('maintenance.html')
    c = Context({})
    return HttpResponse(t.render(c), content_type="text/html", status=503)

