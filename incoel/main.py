# -*- coding: utf-8 -*-
from lxml import etree
import argparse


parser = argparse.ArgumentParser(description="Count metrics of neural network")
parser.add_argument("-d", type=str, help="dts directory")
parser.add_argument("-g", type=str, help="gts directory")
args = parser.parse_args()


def parseXML(xmlFile):

    with open(xmlFile) as fobj:
        xml = fobj.read()

    root = etree.fromstring(xml)

    boxList = []  # list of all objects in XML

    for appt in root.getchildren():
        for elem in appt.getchildren():
            if elem.tag == "bndbox":
                coords = []  # list of nums of each object
                for box in elem.getchildren():
                    if not box.text:
                        text = "None"
                    else:
                        text = box.text
                    coords.append(text)
                boxList.append(coords)

    return boxList


def countMetrics(firstXML, secondXML):

    tp = 0  # element is found in parseXML(firstXML) and is contained in parseXML(secondXML)]
    fp = 0  # element is found in parseXML(firstXML) but is not contained in parseXML(secondXML)]
    fn = 0  # element is not found in parseXML(firstXML) but is contained in parseXML(secondXML)]
    tn = 0  # element is not found in parseXML(firstXML) and is not contained in parseXML(secondXML)] (always true)

    for i in parseXML(firstXML):
        if i in parseXML(secondXML):
            tp += 1

    for i in parseXML(firstXML):
        if i not in parseXML(secondXML):
            fp += 1

    for i in parseXML(secondXML):
        if i not in parseXML(firstXML):
            fn += 1

    print("=======")
    print("TP: " + str(tp))
    print("TN: " + str(tn))
    print("FP: " + str(fp))
    print("FN: " + str(fn))
    print("=======")
    print("AC: " + str((tp + tn) / (tp + tn + fp + fn)))
    print("TPR: " + str(tp / (tp + fn)))
    print("FPR: " + str(fp / (fp + tn)))


if __name__ == "__main__":
    countMetrics("test_task/dst/" + args.d, "test_task/gts/" + args.g)
