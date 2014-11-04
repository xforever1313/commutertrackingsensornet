from django.contrib import admin
from ctsn_web.models import *

class CtsnUserAdmin(admin.ModelAdmin):
    list_display = ("getUsername", "getEmail", "phone", "provider", "isStaff", "isAdmin")

    def getEmail(self, obj):
        return obj.user.email

    getEmail.admin_order_field = "user__email"
    getEmail.short_description = "Email"

    def getFirstName(self, obj):
        return obj.user.first_name

    getFirstName.admin_order_field = "user__first_name"
    getFirstName.short_description = "First Name"

    def getLastName(self, obj):
        return obj.user.last_name

    getLastName.admin_order_field = "user__last_name"
    getLastName.short_description = "Last Name"

    def isStaff(self, obj):
        return obj.user.is_staff

    isStaff.admin_order_field = "user__is_staff"
    isStaff.short_description = "Is Staff"

    def isAdmin(self, obj):
        return obj.user.is_superuser

    isAdmin.admin_order_field = "user__is_superuser"
    isAdmin.short_description = "Is Admin"

    def getUsername(self, obj):
        return obj

    getUsername.admin_order_field = "user"
    getUsername.short_description = "User"

class NodeAdmin(admin.ModelAdmin):
    list_display = ("id", "getDescription", "getStatus", "lat", "lon", "address")
    readonly_fields = ("status",)

    def getDescription(self, obj):
        return obj.desc

    getDescription.admin_order_field = "desc"
    getDescription.short_description = "Description"

    def getStatus(self, obj):
        return obj.status.desc

    getStatus.admin_order_field = "status__desc"
    getStatus.short_description = "Status"

class ResultType(admin.ModelAdmin):
    list_display = ("id", "getDescription")

    def getDescription(self, obj):
        return obj.desc

    getDescription.admin_order_field="desc"
    getDescription.short_description = "Description"

# Register your models here.
admin.site.register(CtsnUser, CtsnUserAdmin)
admin.site.register(Node, NodeAdmin)
admin.site.register(TrailResultType, ResultType)

