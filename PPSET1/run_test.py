import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import time

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
            start = time.perf_counter()
            output = subprocess.check_output(cmd, text=True).strip()
            runtime = time.perf_counter() - start
            value = float(output)
        except Exception as e:
            print(f"Error running {cmd}: {e}")
            value = np.nan
            runtime = np.nan
        data.append((second_arg, value, runtime))
        all_rows.append((dimension, second_arg, value, runtime))
    df = pd.DataFrame(data, columns=["n", "output", "runtime"])
    results[dimension] = df

all_df = pd.DataFrame(all_rows, columns=["dimension", "n", "mst", "runtime"])
all_df.to_csv("output.csv", index=False)

fig, axes = plt.subplots(5, 2, figsize=(16, 20))

for i in range(5):
    df = results[i]

    axes[i, 0].plot(df["n"], df["output"], marker="o")
    axes[i, 0].set_xscale("log", base=2)
    axes[i, 0].set_title(f"randmst MST weight (dimension = {i})")
    axes[i, 0].set_xlabel("n")
    axes[i, 0].set_ylabel("MST Weight")

    axes[i, 1].plot(df["n"], df["runtime"], marker="o", color="orange")
    axes[i, 1].set_xscale("log", base=2)
    axes[i, 1].set_title(f"randmst Runtime (dimension = {i})")
    axes[i, 1].set_xlabel("n")
    axes[i, 1].set_ylabel("Runtime (seconds)")

plt.tight_layout()
plt.savefig("output_plot.png")
plt.show()