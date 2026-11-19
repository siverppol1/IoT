from django.db import models


class Dht11(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    date = models.DateTimeField(auto_now_add=True)
