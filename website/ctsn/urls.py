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
    url(r'^admin_messages.html$', 'ctsn_web.views.AdminMessageView'),
    url(r'^node_status.html$', 'ctsn_web.views.NodeStatusView'),
    url(r'^node_stats.html$', 'ctsn_web.views.NodeStatsView'),
    url(r'^admin_maintenance.html$', 'ctsn_web.views.AdminMaintenanceView'),
    url(r'^login.html$', 'ctsn_web.views.LoginRequest'),
    url(r'^logout.html$', 'ctsn_web.views.LogoutRequest')
)
