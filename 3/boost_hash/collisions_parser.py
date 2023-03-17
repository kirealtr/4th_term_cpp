import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

plt.figure(figsize=(12, 8))
collision_test_size = 0
for i in range(9):
    df = pd.read_csv(".\\boost_hash\\collision_test_data\\" + str(i) + ".csv", header=None)
    data = np.array(df[1:])
    collision_test_size = int(data[..., 0].max()) + 1
    plt.plot(data[..., 0], data[..., 1], label = df[0][0])

plt.xlabel("Number of objects")
plt.ylabel("Number of collisions")
plt.xticks(np.arange(-1, collision_test_size, step=collision_test_size/10))
plt.legend()
plt.title("График зависимости числа колизий от количества экземпляров для разных хэш-функций")
plt.savefig(".\\boost_hash\\collision_graph.png")
plt.show()