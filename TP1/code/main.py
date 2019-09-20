# coding=utf-8

import random
import math
import sys
import time
import csv
import argparse

from brute_force import execute_brute_force
from DpR import execute_DpR
from utils import GRID_SIZE

def getOptions(args=sys.argv[1:]):
    parser = argparse.ArgumentParser(description="Parses command.")
    parser.add_argument("-a", "--algo")
    parser.add_argument("-e", "--path")
    parser.add_argument("-p", "--smallest", dest='smallest', action='store_true')
    parser.add_argument("-t", "--time", dest='time', action='store_true')
    options = parser.parse_args(args)
    return options


def generate_points(N):
    points = [(random.randint(0, GRID_SIZE), random.randint(0, GRID_SIZE)) for i in range(N)]
    return points


def main():
    options = getOptions()
    FILE = open(options.path, "r")
    nb_points = FILE.readline()
    POINTS = []
    time = 0
    for x in range(int(nb_points)):
        point1, point2 = FILE.readline().split()
        POINTS.append([int(point1), int(point2)])

    sorted_points_x = sorted(POINTS, key=lambda x: x[0])
    sorted_points_y = sorted(POINTS, key=lambda x: x[1])
    
    if options.algo == "brute":
        # Exécuter l'algorithme force brute
        time = execute_brute_force(sorted_points_x, options.smallest)
    
    elif options.algo == "seuil":
        # Exécuter l'algorithme Diviser pour régner
        time = execute_DpR(sorted_points_x, sorted_points_y, 1, options.smallest)

    elif options.algo == "seuil4":
        # Exécuter l'algorithme Diviser pour régner
        time = execute_DpR(sorted_points_x, sorted_points_y, 4, options.smallest)

    if options.time:
        print(time)
main()