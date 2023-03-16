import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

conts = ['set', 'vector', 'array']

plt.figure(figsize=(10, 6))
for c in conts:
    data = np.array(pd.read_csv(c + "_data.csv"))
    plt.plot(data[1:, 0], data[1:, 1], label = c + ' test')

plt.xlabel("Container size")
plt.ylabel("Execution time")
plt.legend()
plt.savefig("graph.png")
plt.show()