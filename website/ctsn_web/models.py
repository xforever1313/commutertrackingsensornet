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
    time = models.DateTimeField()
    class Meta:
        managed = False
        db_table = 'error_log'

class ErrorMessages(models.Model):
    id = models.IntegerField(db_column='ID', primary_key=True) # Field name made lowercase.
    message = models.CharField(db_column='MESSAGE', max_length=255) # Field name made lowercase.
    severity = models.ForeignKey('StatusSeverity', db_column='severity')
    class Meta:
        managed = False
        db_table = 'error_messages'

class Node(models.Model):
    desc = models.CharField(db_column = "description", max_length = 255)
    x = models.IntegerField(db_column = 'x')
    y = models.IntegerField(db_column = 'y')
    status = models.ForeignKey('NodeStatus', db_column="status")
    class Meta:
        managed = True
        db_table = 'node'

class NodeStatus(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    severity = models.ForeignKey('StatusSeverity', db_column='status_severity')
    class Meta:
        managed = True
        db_table = 'node_status'

class StatusSeverity(models.Model):
    desc = models.CharField(db_column = 'desc', max_length = 255)
    color = models.CharField(db_column = 'color', max_length = 15)

    class Meta:
        managed = True
        db_table = 'status_severity'

class CtsnUser(models.Model):
    user = models.ForeignKey(User)
    phone = models.CharField(db_column = "PHONE_NUMBER", max_length=255)
    provider = models.IntegerField(db_column = "PROVIDER")

    class Meta:
        managed = True
        db_table = 'ctsn_user'

