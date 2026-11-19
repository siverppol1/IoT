from django.test import TestCase


class TestViewTests(TestCase):
    def test_test_view(self):
        response = self.client.get('/test/')

        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.content, b'salam je suis kiker / hamdaoui')
