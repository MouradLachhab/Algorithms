import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['taille']).mean().reset_index()
pf = df['taille']

df['temps'] = df['temps']/(pf*pf)

print(df)
g = sns.FacetGrid(df, size=4, aspect=1)
g = g.map(plt.scatter, 'taille', 'temps')
plt.axis([0,40000,0.00000004,0.00000003])

g.add_legend()
plt.savefig('test_rapport' + '_' + sys.argv[2])
