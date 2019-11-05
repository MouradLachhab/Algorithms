# coding=utf-8

import random
import math
import sys
import time
import csv
import argparse

def getOptions(args=sys.argv[1:]):
    parser = argparse.ArgumentParser(description="Parses command.")
    parser.add_argument("-a", "--algo")
    parser.add_argument("-e", "--path")
    parser.add_argument("-p", "--price", dest='price', action='store_true')
    parser.add_argument("-t", "--time", dest='time', action='store_true')
    options = parser.parse_args(args)
    return options


def main():
    options = getOptions()
    FILE = open(options.path, "r")
    
    if options.algo == "glouton":
        # Exécuter l'algorithme force glouton
	execute_glouton()
    
    elif options.algo == "progdyn":
        # Exécuter l'algorithme Programmation Dynamique


    elif options.algo == "backtrack":
        # Exécuter l'algorithme avec retour en arriere


    if options.time:
        print(time)
main()
