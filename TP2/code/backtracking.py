import time
import sys

def backtrack(n, cuts, prices, showCuts):
    pathChosen = []
    subTreeTotal = 0

    for i in range(0,n):
        tempTotal, path = backtrack(n-cuts[i], cuts, prices, showCuts)
        tempTotal += prices[i]
        if tempTotal > subTreeTotal:
            subTreeTotal = tempTotal
            pathChosen = path + [cuts[i]]

    return [subTreeTotal, pathChosen]


def execute_backtrack(n, cuts, prices, showTotal, showCuts):
    sys.setrecursionlimit(10**5)
    start = time.time()
    total, path = backtrack(n, cuts, prices, showCuts)
    end = time.time()

    if showCuts:
        print(path)
    
    if showTotal:
        print(int(total))

    return end-start