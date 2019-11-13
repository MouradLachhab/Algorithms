import time

def prog_dyn(n, cuts, prices, showCuts):
    r = [0] * n

    for j in range(1,n):
        for i in range(j + 1):
            currentValue = prices[i] + r[j - i]

            if currentValue > r[j]:
                r[j] = currentValue
    
    return r[n - 1]

def execute_prog_dyn(n, cuts, prices, showTotal, showCuts):
    start = time.time()
    total = prog_dyn(n, cuts, prices, showCuts)
    end = time.time()

    if showTotal:
        print(int(total))

    return end-start