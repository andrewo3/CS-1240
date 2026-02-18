import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

results = {}
all_rows = []

for dimension in range(5):
    print(f"Dimension {dimension}")
    if dimension == 1:
        max_power = 18
    else:
        max_power = 15

    powers = [2**i for i in range(7, max_power + 1)]

    data = []

    for second_arg in powers:
        print(f"n = {second_arg}")
        cmd = ["./randmst", "0", str(second_arg), "10", str(dimension)]

        try:
            output = subprocess.check_output(cmd, text=True).strip()
            value = float(output)
        except Exception as e:
            print(f"Error running {cmd}: {e}")
            value = np.nan

        data.append((second_arg, value))
        all_rows.append((dimension, second_arg, value))

    df = pd.DataFrame(data, columns=["n", "output"])
    results[dimension] = df


all_df = pd.DataFrame(all_rows, columns=["dimension", "n", "mst"])
all_df.to_csv("output.csv", index=False)

fig, axes = plt.subplots(5, 1, figsize=(8, 20))

for i in range(5):
    df = results[i]
    axes[i].plot(df["n"], df["output"], marker="o")
    axes[i].set_xscale("log", base=2)
    axes[i].set_title(f"randmst results (dimension = {i})")
    axes[i].set_xlabel("Second Argument (n)")
    axes[i].set_ylabel("Output")

plt.tight_layout()
plt.savefig("output_plot.png")
plt.show()
