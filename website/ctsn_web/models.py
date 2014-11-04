# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Remove `managed = False` lines if you wish to allow Django to create and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
#
# Also note: You'll have to insert the output of 'django-admin.py sqlcustom [appname]'
# into your database.
from __future__ import unicode_literals

from django.db import models
from django.contrib.auth.models import User

class ErrorLog(models.Model):
    id = models.IntegerField(db_column='ID', primary_key=True) # Field name made lowercase.
    node = models.ForeignKey('Node', db_column='NODE_ID') # Field name made lowercase.
    message = models.ForeignKey('ErrorMessages', db_column='MESSAGE_ID') # Field name made lowercase.
    time = models.DateTimeField(db_column='time')
    class Meta:
        managed = True
        db_table = 'error_log'

class ErrorMessages(models.Model):
    id = models.IntegerField(db_column='ID', primary_key=True) # Field name made lowercase.
    message = models.CharField(db_column='MESSAGE', max_length=255) # Field name made lowercase.
    severity = models.ForeignKey('StatusSeverity', db_column='severity')

    def __unicode__(self):
        return self.message

    class Meta:
        managed = True
        db_table = 'error_messages'

class TrailResultType(models.Model):
    desc = models.CharField(db_column = "description", max_length=255)
    class Meta:
        managed = True
        db_table = "trail_result_table"

    def __unicode__(self):
        return self.desc

class TrailResult(models.Model):
    time = models.DateTimeField(db_column='time')
    type = models.ForeignKey('TrailResultType', db_column='type')
    node = models.ForeignKey('Node', db_column = 'node')
    class Meta:
        managed  = True
        db_table = "trail_result"

class Node(models.Model):
    desc = models.CharField(db_column = "description", max_length = 255)
    lat = models.FloatField(db_column = 'lat')
    lon = models.FloatField(db_column = 'lon')
    status = models.ForeignKey('NodeStatus', db_column="status", default = lambda: NodeStatus.objects.get(desc='unknown'))
    address = models.CharField(db_column = "address", max_length = 16, default = "000000000000ffff")

    class Meta:
        managed = True
        db_table = 'node'

    def __unicode__(self):
        return self.desc

class NodeStatus(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    severity = models.ForeignKey('StatusSeverity', db_column='status_severity')
    class Meta:
        managed = True
        db_table = 'node_status'

    def __unicode__(self):
        return self.desc

class StatusSeverity(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    color = models.CharField(db_column = 'color', max_length = 15)

    class Meta:
        managed = True
        db_table = 'status_severity'

    def __unicode__(self):
        return self.desc

providerChoices = ((1, "ATT"),
                   (2, "Verizon"),
                   (3, "T-Mobile"),
                   (4, "Sprint"),
                   (5, "Virgin Mobile"),
                   (6, "US Cellular"),
                   (7, "Nextel"),
                   (8, "Boost Mobile"),
                   (9, "Alltel"))

class CtsnUser(models.Model):
    user = models.ForeignKey(User)
    phone = models.CharField(db_column = "PHONE_NUMBER", max_length=255)
    provider = models.IntegerField(db_column = "PROVIDER", choices=providerChoices)

    class Meta:
        managed = True
        db_table = 'ctsn_user'

    def __unicode__(self):
        firstName = self.user.first_name
        lastName = self.user.last_name
        if ((firstName == "") or (lastName == "")):
            return self.user.username
        else:
            return firstName + " " + lastName

class WindbeltResult(models.Model):
    voltage = models.IntegerField('voltage', db_column='voltage')
    time = models.DateTimeField(db_column='time')
    class Meta:
        managed = True
        db_table = 'windbelt_result'

    def __unicode__(self):
        return self.voltage

class Website(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    status = models.ForeignKey('WebsiteStatus', db_column='status')
    class Meta:
        managed = True
        db_table = 'website'

    def __unicode__(self):
        return self.desc

class WebsiteStatus(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    severity = models.ForeignKey('StatusSeverity', db_column='status_severity')
    class Meta:
        managed = True
        db_table = 'website_status'

    def __unicode__(self):
        return self.desc

