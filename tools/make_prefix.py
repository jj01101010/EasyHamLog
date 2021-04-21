import xml.etree.ElementTree as ET
from bs4 import BeautifulSoup
import bs4
import requests
from xml.dom import minidom

page = requests.get("https://en.wikipedia.org/wiki/ITU_prefix")

if page.status_code == 200:
    content = page.content

soup = BeautifulSoup(content, 'html.parser')
table = soup.find_all("table", 'wikitable')[-1].tbody

prefixes = []

for tr in table:
	if type(tr) == bs4.element.NavigableString:
		continue
	tds = tr.find_all("td")
	for i in range(0, len(tds), 2):
		# 8211 is the bigger - in utf-8, which wikipedia uses instead of -
		prefix = tds[i].text.strip().replace(chr(8211), "-")
		country = tds[i + 1].a.text.strip().replace(chr(8211), "-")
		prefixes.append([country, prefix])

print(prefixes)

data = ET.Element("PREFIX_LIST")

for prefix in prefixes:
	child = ET.SubElement(data, "PREFIX")
	child.set('name', prefix[0])
	child.set('prefix', prefix[1])


with open('prefix_lookup.xml', "w", encoding="utf-8") as f:
	f.write(minidom.parseString(ET.tostring(data)).toprettyxml(indent="    "))
