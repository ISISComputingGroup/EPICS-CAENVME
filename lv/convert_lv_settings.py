import xml.etree.ElementTree as ET
ns = "{http://www.ni.com/LVData}"
inst = "EMU"
tree = ET.parse(r'e:\default_settings.xml')
root = tree.getroot()
card = "-1"
channel = "-1"
for elem in root.iter():
    if elem.tag == ns + "Name" and elem.text == "Card Number":
        mode = 1
    if elem.tag == ns + "Name" and elem.text == "Threshold (mV)":
        mode = 2
    if elem.tag == ns + "Name" and elem.text == "Enabled":
        mode = 3
    if elem.tag == ns + "Name" and elem.text == "Output Width (0-7)":
        mode = 4
    if elem.tag == ns + "Name" and elem.text == "Output Width (8-15)":
        mode = 5
    if elem.tag == ns + "Name" and elem.text.startswith("Channel"):
        channel = str(int(elem.text[7:]))
    prefix = "caput IN:"+inst+":CAEMV895_01:CR0:C"+card+":"
    chprefix = prefix + "CH" + channel + ":"
    if elem.tag == ns + "Val":
        if mode == 1:
            card = elem.text
        if mode == 2:
            print chprefix+"THOLD:SP", elem.text
        if mode == 3:
            print chprefix+"ENABLE:SP", elem.text
        if mode == 4:
            print prefix+"OUT:WIDTH:0_TO_7:SP", elem.text
        if mode == 5:
            print prefix+"OUT:WIDTH:8_TO_15:SP", elem.text
