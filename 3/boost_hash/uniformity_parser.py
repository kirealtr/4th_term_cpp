import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

plt.figure(figsize=(15, 10))

df = pd.read_csv(".\\boost_hash\\data.csv", index_col=0, header=None).T
#df.columns = df.iloc[0]
sns.histplot(df.melt(), x='value', hue=0, multiple='dodge', shrink=.75, bins=30);

# plt.xlabel("Container size")
# plt.ylabel("Execution time")
plt.title("Гистограммы распределения хэшей случайных строк для разных хэш-функций")
plt.savefig(".\\boost_hash\\uniformity_graph.png")
# plt.xlim((0, 2e8))
plt.show()