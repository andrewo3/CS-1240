import pandas as pd
import matplotlib.pyplot as plt

# Load saved data
df = pd.read_csv("output.csv")

fig, axes = plt.subplots(5, 1, figsize=(8, 20))

for i in range(5):
    sub = df[df["dimension"] == i]

    axes[i].plot(sub["n"], sub["mst"], marker="o")
    axes[i].set_xscale("log", base=2)
    axes[i].set_title(f"randmst results (dimension = {i})")
    axes[i].set_xlabel("n")
    axes[i].set_ylabel("Average MST Length")

plt.tight_layout()
plt.savefig("output_plot.png")
plt.show()
