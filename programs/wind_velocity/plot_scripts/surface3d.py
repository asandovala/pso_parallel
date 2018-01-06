#!/usr/bin/env python
# -*- coding: utf-8 -*-

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

file_path = "../../../data/2013/parse/3D_data_13.csv"
x_data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
y_data = [3,6,9,12,15,18,21,24]
z_data = []

with open(file_path, "r") as f:
    text = f.read()
    text = text.split("\n")
    for i in range(0, len(text)):
        velocities = text[i].split(",")
        row = []
        for vel in velocities:
            #print vel
            try:
                row.append(float(vel))
            except ValueError:
                e = "Error"
        if len(row) > 0:        
            z_data.append(row)
#print z_data
fig = plt.figure()
ax = fig.gca(projection='3d')
X = y_data
Y = x_data
X, Y = np.meshgrid(X, Y)
Z = z_data
surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
                       linewidth=0, antialiased=True)
ax.set_zlim(0, 8)
ax.set_ylim(1, 12)
ax.set_xlim(3, 24)

ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

fig.colorbar(surf, shrink=0.5, aspect=5)

ax.set_title(r"""$Velocidad\ del\ viento - Valpara{\acute{\imath}}so, 2013$""", fontsize=14)
ax.set_xlabel('Horas')
ax.set_ylabel('Meses')
ax.set_zlabel('Velocidad(m/s)')

plt.show()
