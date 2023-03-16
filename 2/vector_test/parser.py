import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data = np.array(pd.read_csv("data1.csv"))
grid = np.arange(0, data.shape[0], 1)

plt.figure(figsize=(10, 6))
plt.plot(grid, data[..., 0], label = "Vector size")
plt.plot(grid, data[..., 1], label = "Vector capacity")
plt.legend()
plt.savefig("graph_reserved.png")
plt.show()