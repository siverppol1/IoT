from django.shortcuts import render
from .models import Dht11


def dashboard(request):
    """Page d'accueil : Dashboard de monitoring de la chaîne du froid."""
    latest = Dht11.objects.order_by('-date').first()
    return render(request, 'dashboard.html', {'latest': latest})


def test(request):
    from django.http import HttpResponse
    return HttpResponse('salam je suis kiker / hamdaoui')
