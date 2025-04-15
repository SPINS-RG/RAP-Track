import matplotlib.pyplot as plt
import pandas as pd

# Data for each table
data_binary_size = {
    "Method": ["Naive MTB", "TRACES", "RAP-Track"],
    "Ultrasonic": [220, 284, 244],
    "Geiger": [556, 676, 774],
    "Syringe": [376, 496, 630],
    "Temperature": [456, 564, 510],
    "GPS": [3340, 4206, 5952],
    "prime": [332, 414, 502],
    "crc32": [152, 208, 184],
    "search": [208, 324, 434]
}

data_runtime = {
    "Method": ["Naive MTB", "TRACES", "RAP-Track"],
    "Ultrasonic": [26258, 28284, 26795],
    "Geiger": [3421, 5490, 4309],
    "Syringe": [41915, 84582, 49687],
    "Temperature": [7698, 44619, 9971],
    "GPS": [7012, 76296, 10456],
    "prime": [21426, 177570, 29241],
    "crc32": [50277, 296501, 50461],
    "search": [29732, 418939, 48211]
}

data_cflog_size = {
    "Method": ["Naive MTB", "TRACES", "RAP-Track"],
    "Ultrasonic": [12144, 56, 24],
    "Geiger": [2880, 186, 464],
    "Syringe": [49360, 1400, 992],
    "Temperature": [11512, 1212, 1360],
    "GPS": [8400, 2596, 2920],
    "prime": [10392, 5218, 6928],
    "crc32": [20464, 8208, 16],
    "search": [24224, 12904, 2224]
}

# Converting dictionaries to DataFrames
df_binary_size = pd.DataFrame(data_binary_size)
df_runtime = pd.DataFrame(data_runtime)
df_cflog_size = pd.DataFrame(data_cflog_size)

# Font size configuration
label_font_size = 30
legend_font_size = 25
tick_font_size = 30

# Colors for each method
colors = ["black", "gray", "white"]

# Function to generate the two-panel plot for a given dataset
def generate_two_panel_plot(df, y_label, no_legend=False):
    # Split data into two groups and divide y-values by 1000
    df_group1 = df[["Method", "Ultrasonic", "Geiger", "Temperature"]].set_index("Method").T / 1000
    df_group2 = df[["Method", "Syringe", "GPS", "prime", "crc32", "search"]].set_index("Method").T / 1000

    # Generate plots
    plt.figure(figsize=(14, 6))

    # Left plot
    plt.subplot(1, 2, 1)
    df_group1.plot(kind="bar", color=colors, edgecolor="black", ax=plt.gca())
    plt.yscale('linear')
    plt.ylabel(y_label, fontsize=label_font_size)
    
    plt.xticks(rotation=25, fontsize=tick_font_size)
    plt.yticks(fontsize=tick_font_size)
    plt.legend(fontsize=legend_font_size, title_fontsize=legend_font_size)

    # if no_legend:
    plt.legend().set_visible(False)

    # Right plot
    plt.subplot(1, 2, 2)
    df_group2.plot(kind="bar", color=colors, edgecolor="black", ax=plt.gca())
    plt.yscale('linear')
    plt.xticks(rotation=25, fontsize=tick_font_size)
    plt.yticks(fontsize=tick_font_size)
    plt.legend( fontsize=legend_font_size, title_fontsize=legend_font_size)

    # if no_legend:
    #     plt.legend().set_visible(False)
    

    plt.tight_layout()
    plt.show()

# Generating each plot
generate_two_panel_plot(df_binary_size, "Size (kBytes)",no_legend=True)
generate_two_panel_plot(df_runtime, "Runtime (kCycles)",no_legend=True)
generate_two_panel_plot(df_cflog_size, "Size (kBytes)")
