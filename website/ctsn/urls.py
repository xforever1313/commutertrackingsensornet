from django.conf.urls import patterns, include, url
from django.contrib import admin

admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'ctsn.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^$', 'ctsn_web.views.HomeView'),
    url(r'^index.html$', 'ctsn_web.views.HomeView'),
    url(r'admin_messages.html', 'ctsn_web.views.AdminMessageView'),
    url(r'node_status.html', 'ctsn_web.views.NodeStatusView')
)
