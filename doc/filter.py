#!/bin/env python
# -*- coding: iso-8859-1 -*-


import sys

def print_to_stdout(file_name):
    fd = open(file_name, "r")
    data = fd.read()
    fd.close()

    data = data.replace("�", "&agrave;")
    data = data.replace("�", "&egrave;")
    data = data.replace("�", "&igrave;")
    data = data.replace("�", "&egrave;")
    data = data.replace("�", "&ograve;")
    data = data.replace("�", "&ugrave;")


    data = data.replace("�", "&eacute;")

    data = data.replace("�", "&Egrave;")

    print data




if __name__ == "__main__":
    print_to_stdout(sys.argv[1])
