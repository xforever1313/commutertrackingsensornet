"""
Django settings for ctsn project.

For more information on this file, see
https://docs.djangoproject.com/en/1.6/topics/settings/

For the full list of settings and their values, see
https://docs.djangoproject.com/en/1.6/ref/settings/
"""

# Build paths inside the project like this: os.path.join(BASE_DIR, ...)
import os
import sys
BASE_DIR = os.path.dirname(os.path.dirname(__file__))

from Secrets import *

# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/1.6/howto/deployment/checklist/

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = django_key

# SECURITY WARNING: don't run with debug turned on in production!
DEBUG = False

TEMPLATE_DEBUG = False

ALLOWED_HOSTS = ['localhost', django_host]


CSRF_COOKIE_HTTPONLY = True
CSRF_COOKIE_SECURE = True

SESSION_COOKIE_HTTPONLY = True
SESSION_COOKIE_SECURE = True


# Robots.txt settings
ROBOTS_USE_SITEMAP=False

SITE_ID=1

# Application definition

TEMPLATE_DIRS = (
    "/home/django/www/templates",
)

INSTALLED_APPS = (
    'BruteBuster',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sites',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'ctsn_web',
    'captcha',
    'robots',
)

MIDDLEWARE_CLASSES = (
    'BruteBuster.middleware.RequestMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
)

ROOT_URLCONF = 'ctsn.urls'

WSGI_APPLICATION = 'ctsn.wsgi.application'


# Database
# https://docs.djangoproject.com/en/1.6/ref/settings/#databases

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.mysql',
        'NAME' : mariadb_name,
        'USER' : mariadb_user,
        'PASSWORD' : mariadb_pass,
        'HOST' : mariadb_ip,
        'PORT' : mariadb_port
    }
}

# Internationalization
# https://docs.djangoproject.com/en/1.6/topics/i18n/

LANGUAGE_CODE = 'en-us'

TIME_ZONE = 'America/New_York'

USE_I18N = True

USE_L10N = True

USE_TZ = False


# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/1.6/howto/static-files/

STATIC_URL = '/static/'

STATIC_ROOT = '/home/django/static'

STATICFILES_DIRS = ('/home/django/www/ctsn/static',)
