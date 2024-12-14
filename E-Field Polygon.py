# Written By Abraham Kabon
# Simulating Electric Field Patterns for Increasing Regular Polygonal Charge Configurations

import math
import pandas as pd
import numpy as np
import scipy as sp
import matplotlib as mp 
import matplotlib.pyplot as plt

# CONSTANTS
k = 8987551788 # one over 4 pi e not r-squared
q = 1.602 * 10**(-19) # proton charge

# VARIABLES
# User input defining radius of polygon and number of sides/vertices
scale = int(input("Enter scale of the visual: "))
r_poly = float(input("Enter the radius of your polygon(0 < r <= 1): "))
n = int(input("Enter the number of vertices (n > 2): "))

# Input validation of n and r
if n <= 2:
    raise ValueError("The number of sides (n) must be greater than 2.")
if r_poly > 1 or r_poly <= 0:
    raise ValueError("The radius of polygon is out of range.")
    
r_poly = r_poly*10**(scale) 

centre_angle = (2*math.pi)/n #Calculating each central angle from knowledge of polygon symmetry

ang_list = [] #List to hold the angle coordinate of all vertice locations

# Calculate angles and Cartesian coordinates
ang_list = [i * centre_angle for i in range(n)]  # Angles in radians
p_vector = [[round(float(r_poly * np.cos(angle)), 10), round(float(r_poly * np.sin(angle)), 10)] for angle in ang_list]  
# Cartesian coordinates of each vertice (n) 

x = np.linspace(-1*10**(scale),1*10**(scale), 100) # Generates 100 points between -x and x 
y = np.linspace(-1*10**(scale),1*10**(scale), 100) # Generates 100 points between -y and y

X, Y = np.meshgrid(x, y)  # Creates 2D grid of points

# Initialize field components
E_x = np.zeros(X.shape)
E_y = np.zeros(Y.shape)

# Compute electric field contribution from each charge
for px_vector, py_vector in p_vector:
    dx_vector = X - px_vector  # Displacement in X
    dy_vector = Y - py_vector  # Displacement in Y
    d_vector_mag = np.sqrt(dx_vector**2 + dy_vector**2)  # Magnitude of the displacement vector
    d_vector_mag[d_vector_mag == 0] = np.inf  # Avoid division by zero at source points
    E_x += (k * q * dx_vector) / (d_vector_mag**3)  # X-component of the field
    E_y += (k * q * dy_vector) / (d_vector_mag**3)  # Y-component of the field

# Normalizing the electric field vectors for better visualization
E_magnitude = np.sqrt(E_x**2 + E_y**2)
E_x_normalized = E_x / E_magnitude
E_y_normalized = E_y / E_magnitude

# Applying inverse logarithmic scaling to emphasize variations of the E-field in regions further from the point charges
E_transformed = np.log10(1 + E_magnitude) / np.log10(1 + np.max(E_magnitude))

# Plotting the electric field with an adjusted color gradient
plt.figure(figsize = (8, 8))

# Use the transformed electric field magnitude for the colormap
plt.contourf(X, Y, E_transformed, levels = 50, cmap ='viridis', alpha = 0.8)
plt.colorbar(label = "Logarithmically Scaled Electric Field Magnitude")

# Add quiver plot for direction (use untransformed E_x and E_y)
plt.quiver(X, Y, E_x_normalized, E_y_normalized, scale = 40, pivot = "middle", color = "white", alpha = 0.8)

# Scatter plot the vertices
marker_size = max(1, 5/scale)
plt.scatter(*zip(*p_vector), color = "red", label = "Point Charges", s = marker_size)

plt.title(f"Electric Field of a Polygonal Charge Configuration (n = {n})")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid()
filename = f"electric_field_visualization_n{n}_scale{scale}.png"
plt.savefig(filename, dpi = 300, bbox_inches = 'tight')
plt.show()
