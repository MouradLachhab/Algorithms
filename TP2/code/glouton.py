import time

def glouton(n, cuts, prices, showCuts):
    cutsChosen = []
    profitability = [0]  * n
    total = 0
    for i in range(n):
        profitability[i] = prices[i] / float(cuts[i])

    currentLength = n

    while currentLength > 0:
        maxProfit = 0
        maxIndex = 0

        for i in range(currentLength):
            if (profitability[i] > maxProfit):
                maxProfit = profitability[i]
                maxIndex = i
        
        currentLength -= cuts[maxIndex]
        total += prices[maxIndex]
        cutsChosen.append(cuts[maxIndex])

    if showCuts:
        print(' '.join(map(str,cutsChosen)))    

    return total

def execute_glouton(n, cuts, prices, showTotal, showCuts):
    start = time.time()
    total = glouton(n, cuts, prices, showCuts)
    end = time.time()

    if showTotal:
        print(int(total))

    return end-start