from django.urls import path
from .api import Dlist, Dhtviews
from . import views

urlpatterns = [
    path("", views.dashboard, name="dashboard"),
    path("list/", Dlist, name="dht-list"),
    path("post/", Dhtviews.as_view(), name="dht-create"),
]
