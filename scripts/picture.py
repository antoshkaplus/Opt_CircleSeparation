

import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("solution.txt")

max_radius = np.max(data[:, 4])

fig = plt.gcf()
for i in range(data.shape[0]): 
    c = plt.Circle(data[i, 2:4], data[i, 4])
    fig.gca().add_artist(c)

for i in range(data.shape[0]):
    p = zip(data[i, 0:2], data[i, 2:4])
    plt.plot(p[0], p[1], 'y-', lw=1)

    
plt.xlim(data[:, 2].min() - max_radius, data[:, 2].max() + max_radius)
plt.ylim(data[:, 3].min() - max_radius, data[:, 3].max() + max_radius)

plt.show()
