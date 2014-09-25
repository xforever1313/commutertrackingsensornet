from django.shortcuts import render
from django.shortcuts import render_to_response
from django.template import RequestContext

def HomeView(request):
    context = {}
    return render_to_response('index.html', context, context_instance=RequestContext(request))

