import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille']).mean().reset_index()
pf = df['taille']

df['temps'] = df['temps']/(pf*np.log(pf))

g = sns.FacetGrid(df, hue='algo', size=4, aspect=1)
g = g.map(plt.scatter, 'taille', 'temps')
plt.axis([0,15000,0.00000078,0.000000925])
g.add_legend()
plt.savefig('test_rapport' + '_' + sys.argv[2])
