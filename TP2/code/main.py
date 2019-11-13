# coding=utf-8

import random
import math
import sys
import time
import csv
import argparse
from glouton import execute_glouton
from progdyn import execute_prog_dyn

def getOptions(args=sys.argv[1:]):
    parser = argparse.ArgumentParser(description="Parses command.")
    parser.add_argument("-a", "--algo")
    parser.add_argument("-e", "--path")
    parser.add_argument("-p", "--price", dest='price', action='store_true')
    parser.add_argument("-t", "--time", dest='time', action='store_true')
    parser.add_argument("-c", "--showCuts", dest='showCuts', action='store_true')

    options = parser.parse_args(args)
    return options


def main():
    options = getOptions()
    FILE = open(options.path, "r")
    n = int(FILE.readline())
    cuts = [int(i) for i in FILE.readline().split()]
    prices = [float(i) for i in FILE.readline().split()]

    if options.algo == "glouton":
        # Exécuter l'algorithme force glouton
        time = execute_glouton(n, cuts, prices, options.price, options.showCuts)


    elif options.algo == "progdyn":
        # Exécuter l'algorithme Programmation Dynamique
        cuts = [0]+ cuts
        prices = [0] + prices
        n += 1
        time = execute_prog_dyn(n, cuts, prices, options.price, options.showCuts)



    # elif options.algo == "backtrack":
        # Exécuter l'algorithme avec retour en arriere


    if options.time:
        print(time)
main()
