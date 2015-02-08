

import numpy as np
import matplotlib.pyplot as plt

max_radius = 4

data = np.genfromtxt("output.txt")
fig = plt.gcf()
for i in range(data.shape[0]): 
    c = plt.Circle(data[i, :], max_radius)
    fig.gca().add_artist(c)
    
plt.xlim(data[:, 0].min() - max_radius, data[:, 0].max() + max_radius)
plt.ylim(data[:, 1].min() - max_radius, data[:, 1].max() + max_radius)

plt.show()
