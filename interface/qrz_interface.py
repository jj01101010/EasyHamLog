import requests
from xml.etree import cElementTree as ElementTree
from xml.dom import minidom as dom

URL = "https://xmldata.qrz.com/xml/"


doc = dom.parseString(r.text)

print(doc)

def login(username, password):
    r = requests.post(URL + "")
