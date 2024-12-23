# Written By Abraham Kabon
# Simulating Electric Field Patterns for charges located at location Xn, Yn 

import os
import subprocess
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import time

# CONSTANTS
k = 8987551788 # one over 4 pi e not r-squared
q_p = 1.602 * 10**(-19) # proton charge
q_e = -q_p # electron charge

# INTRODUCTION
print("Welcome! This program enables you to generate the electric field due to n local point charges.")
print("----------------------------------------------------------------------------------------------")
# Works only in 2D for now

# VARIABLES
try:
    # Get user choice
    choice = input("[INPUT 1] to generate data points using SpatialTransformer.exe\n[INPUT 2] to load saved data points\n")
except ValueError:
    print("Invalid input. Please enter 1 or 2.")
    print("----------------------------------------------------------------------------------------------")
    exit()

exe_file = os.path.abspath("SpatialTransformer.exe")
file_checker = "filename.txt"
exit_file = "exit"

if os.path.exists(file_checker):
    os.remove(file_checker)

if os.path.exists(exit_file):
    os.remove(exit_file)

if choice == '1':
    print("----------------------------------------------------------------------------------------------")
    print("Launching 'SpatialTransformer.exe'...")
    print("----------------------------------------------------------------------------------------------")
    time.sleep(3)
    print("Be Sure to Transform Your Points to Cartesian using SpatialTransformer!")
    time.sleep(3)
    subprocess.run(f'start /wait cmd /c "{exe_file}"', shell=True)
    while True:
        if os.path.exists(exit_file):
            with open(file_checker, "r") as file:
                filename = file.read().strip()
            os.remove(file_checker)
            os.remove(exit_file)
            break
        time.sleep(1)

elif choice == '2':
    print("----------------------------------------------------------------------------------------------")
    filename = input("Enter the name of your saved file (with extension): ")

else:
    print("----------------------------------------------------------------------------------------------")
    print("Invalid choice. Please enter 1 or 2.")
    exit()

print("----------------------------------------------------------------------------------------------")
print("File Found! Generating Plot...")
print("----------------------------------------------------------------------------------------------")
time.sleep(3)

# Read the data points from the file
p_vector = []  # List to store point charge coordinates
try:
    with open(filename, "r") as file:
        lines = file.readlines()
        scale = float(lines[-2].strip())  # Second to the last line in the txt file contains the scale
        n = int(lines[-1].strip()) #Last line contains the number of charges
        for line in lines[:-2]:  # All other lines contain point coordinates
            coordinates = list(map(float, line.split()))
            p_vector.append(coordinates[:2])  # Only X, Y coordinates
except FileNotFoundError:
    print(f"File '{filename}' not found. Please ensure the file exists.")
    exit()
except Exception as e:
    print(f"An error occurred while reading the file: {e}")
    exit()

# Generate grid points for the electric field visualization
x = np.linspace(-scale, scale, 100) # Generates 200 points between -x and x 
y = np.linspace(-scale, scale, 100) # Generates 200 points between -y and y

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
    E_x += (k * q_p * dx_vector) / (d_vector_mag**3)  # X-component of the field
    E_y += (k * q_p * dy_vector) / (d_vector_mag**3)  # Y-component of the field

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
plt.scatter(*zip(*p_vector), color = "red", label = "Point Charges", s = 20)

plt.title(f"Electric Field due to {n} local point charges")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid()
time.sleep(5)
choice = input("Do you wish to save this plot as a .png file? (Y/N): ")
if choice == 'Y':
    plot_file = input("Enter the Filename: ")
    plot_file = plot_file + f"_n{n}.png"
    plt.savefig(plot_file, dpi = 300, bbox_inches = 'tight')
plt.show()