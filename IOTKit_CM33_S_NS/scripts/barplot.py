import matplotlib.pyplot as plt
import numpy as np

# Data for the template
algorithms = ['Ultrasonic', 'Geiger', 'Syringe', 'Temperature','GPS','prime','crc32','sglib-binsearch']
methods = ['Method A', 'Method B']
values = np.array([
    [56, 12144],  # Algorithm 1 scores for Method A and Method B
    [186, 2880],  # Algorithm 2 scores for Method A and Method B
    [1400, 49360],
    [1212, 11512],
    [2596, 8400],
    [5216, 10392],
    [8204, 20464],
    [12900, 24224]   # Algorithm 3 scores for Method A and Method B
])

# Splitting data into two halves for side-by-side plots
first_half_algorithms = algorithms[:4]
second_half_algorithms = algorithms[4:]
first_half_values = values[:4]
second_half_values = values[4:]

# Setting up bar positions and width for each plot
x1 = np.arange(len(first_half_algorithms))
x2 = np.arange(len(second_half_algorithms))
width = 0.35

# Creating side-by-side plots with larger font size and without titles
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# Plot for the first half of algorithms with independent y-axis
bars1_1 = ax1.bar(x1 - width/2, first_half_values[:, 0], width, color='black', label='Instrumentation Based CFA')
bars1_2 = ax1.bar(x1 + width/2, first_half_values[:, 1], width, color='white', edgecolor='black', hatch='//', label='Naive MTB Based CFA')
ax1.set_ylabel('CFLog Size (Bytes)', fontsize=22)
ax1.set_xticks(x1)
ax1.set_xticklabels(first_half_algorithms, fontsize=30)
ax1.legend(fontsize=20)  # Further increased legend font size
ax1.tick_params(axis='x', rotation=25, labelsize=30)

# Plot for the second half of algorithms with independent y-axis
bars2_1 = ax2.bar(x2 - width/2, second_half_values[:, 0], width, color='black')
bars2_2 = ax2.bar(x2 + width/2, second_half_values[:, 1], width, color='white', edgecolor='black', hatch='//')
ax2.set_xticks(x2)
ax2.set_xticklabels(second_half_algorithms, fontsize=30)
ax2.tick_params(axis='x', rotation=25, labelsize=30)

# Set independent y-axis limits for each plot
ax1.set_ylim(0, max(first_half_values.max() * 1.1, 5000))  # Scale up by 10% for better display
ax2.set_ylim(0, max(second_half_values.max() * 1.1, 25000)) # Scale up by 10% for better display

# Displaying the plots side by side with larger axis and legend fonts, no titles
plt.tight_layout()
plt.show()
