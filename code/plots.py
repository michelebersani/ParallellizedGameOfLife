import numpy as np
import pandas as pd
import itertools

data = []
sides = [i for i in [1000,10000]]
nws = [i for i in range(1,9)]
nws = [i for i in [1,5,10,15,20,25,30,40,50,60,80,100,120,150,200,250,300,350,400,450,500]]
seq_times = dict()
machine = "mac"
machine = "xeon"
for s in sides:
    with open(f"/Users/michelebersani/Desktop/Scuola/IVanno/Parallel/project/results_{machine}/seq/{s}.txt") as ff:
        times = ff.readlines()
        times = np.array([int(t) for t in times])
        times = times*1e-6
        seq_times[s] = round(times.mean(), 4)
names = ["parfor_ff", "farm_ff", "thread","dyn_thread"]
for s in itertools.product(sides,nws):
    row = [s[0],s[1]]
    seq_time = seq_times[s[0]]
    for name in names:
        suffix = f"{name}/{s[0]}_{s[1]}.txt"
        with open(f"/Users/michelebersani/Desktop/Scuola/IVanno/Parallel/project/results_{machine}/{suffix}") as ff:
            times = ff.readlines()
            times = np.array([int(t) for t in times])
            times = times*1e-6
            row.append(round(times.mean(), 4))
            row.append(round(times.std(), 4))
            speedups = np.array([seq_time/time for time in times])
            row.append(speedups.mean())
            row.append(speedups.std())

    data.append(row)
df = pd.DataFrame(data, columns=['side','workers','ff_mean','ff_std','ff_speedup_mean','ff_speedup_std','ff_pool_mean','ff_pool_std','ff_pool_speedup_mean','ff_pool_speedup_std','thread_mean','thread_std', 'thread_speedup_mean','thread_speedup_std','new_threadpool_mean','new_threadpool_std', 'new_threadpool_speedup_mean','new_threadpool_speedup_std'])
print(df.shape)

import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 15})
    
s = 1000

fig, axs = plt.subplots(1,2, figsize=(20,10))

small_df = df.loc[df['side'] == s]
axs[0].plot(small_df['workers'], small_df['ff_speedup_mean'], 'b-', label='ff Parfor')
axs[0].fill_between(small_df['workers'], small_df['ff_speedup_mean'] + small_df['ff_speedup_std'], small_df['ff_speedup_mean'] - small_df['ff_speedup_std'], color='b', alpha=0.2)
axs[0].plot(small_df['workers']+1, small_df['ff_pool_speedup_mean'], 'c-', label='ff Farm')
axs[0].fill_between(small_df['workers']+1, small_df['ff_pool_speedup_mean']+ small_df['ff_pool_speedup_std'], small_df['ff_pool_speedup_mean'] - small_df['ff_pool_speedup_std'], color='c', alpha=0.2)
axs[0].plot(small_df['workers'], small_df['thread_speedup_mean'], 'r-', label='pthread')
axs[0].fill_between(small_df['workers'], small_df['thread_speedup_mean'] + small_df['thread_speedup_std'], small_df['thread_speedup_mean'] - small_df['thread_speedup_std'], color='r', alpha=0.2)
axs[0].plot(small_df['workers'], small_df['new_threadpool_speedup_mean'], 'k-', label='pthread dynamic')
axs[0].fill_between(small_df['workers'], small_df['new_threadpool_speedup_mean'] + small_df['new_threadpool_speedup_std'], small_df['new_threadpool_speedup_mean'] - small_df['new_threadpool_speedup_std'], color='k', alpha=0.2)
axs[0].plot(small_df['workers'], small_df['workers'], 'g:', label='ideal')
axs[0].set_ylabel('Speedup')
axs[0].set_xlabel('Workers')
axs[0].legend(title=f'{s} X {s} grid')

axs[1].plot(small_df['workers'], small_df['ff_speedup_mean']/small_df['workers'], 'b-', label='ff Parfor')
axs[1].plot(small_df['workers']+1, small_df['ff_pool_speedup_mean']/(small_df['workers']+1), 'c-', label='ff Farm')
axs[1].plot(small_df['workers'], small_df['thread_speedup_mean']/small_df['workers'], 'r-', label='pthread')
axs[1].plot(small_df['workers'], small_df['new_threadpool_speedup_mean']/small_df['workers'], 'k-', label='pthread dynamic')
axs[1].set_ylabel('Efficiency')
axs[1].set_xlabel('Workers')
axs[1].legend(title=f'{s} X {s} grid')

axs[0].set_ylim(-1, 30)

plt.show()