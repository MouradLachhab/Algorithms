
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['taille']).mean().reset_index()

g = sns.FacetGrid(df, size=4, aspect=1)
g = g.map(plt.plot, 'taille', 'temps')
g.set(xscale='log')
g.set(yscale='log')
g.add_legend()



plt.savefig('test_puissance' + '_' + sys.argv[2])

