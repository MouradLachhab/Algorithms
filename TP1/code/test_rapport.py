import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille']).mean().reset_index()

print(df)
g = sns.FacetGrid(df, hue='algo', size=4, aspect=1)
g = g.map(plt.plot, 'taille', 'test')
g.add_legend()
plt.savefig('test_rapport')