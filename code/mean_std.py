import numpy as np
import pandas as pd
import itertools

data = []

def main():
    sides = [i for i in range(100,501,200)]
    nws = [i for i in range(1,252,10)]
    for s in itertools.product(sides,nws):
        row = [s[0],s[1]]
        suffix = f"_{s[0]}_{s[1]}.txt"
        with open(f"results/ff{suffix}") as ff:
            times = ff.readlines()
            times = np.array([int(t) for t in times])
            times = times*1e-6
            data.append(round(times.mean(), 4))
            data.append(round(times.std(), 4))
        with open(f"results/thread{suffix}") as ff:
            times = ff.readlines()
            times = np.array([int(t) for t in times])
            times = times*1e-6
            data.append(round(times.mean(), 4))
            data.append(round(times.std(), 4))
        data.append(row)

df = pd.DataFrame(data, columns=['side','workers','ff_mean','ff_std','thread_mean','thread_std'])
print(df.shape)