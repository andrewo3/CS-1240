import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Load existing data
df = pd.read_csv("output.csv")

# Only dimension 1–4
df = df[df["dimension"].between(1, 4)]

fig, axes = plt.subplots(2, 2, figsize=(8, 8))

for idx, dimension in enumerate(range(1, 5)):
    sub = df[df["dimension"] == dimension]

    n = sub["n"].values
    y = sub["mst"].values

    mask = y > 0
    n = n[mask]
    y = y[mask]

    log_n = np.log(n)
    log_y = np.log(y)

    # Linear regression in log space
    b, log_a = np.polyfit(log_n, log_y, 1)
    a = np.exp(log_a)

    print(f"dimension = {dimension}")
    print(f"  Fit: y = {a:.6f} * n^{b:.6f}\n")

    # Plot
    axes[idx%2,idx//2].scatter(n, y)
    axes[idx%2,idx//2].plot(n, a * n**b)
    axes[idx%2,idx//2].set_xscale("log")
    axes[idx%2,idx//2].set_yscale("log")
    axes[idx%2,idx//2].set_title(f"Log-Log Fit (dimension = {dimension})")
    axes[idx%2,idx//2].set_xlabel("n")
    axes[idx%2,idx//2].set_ylabel("output")

plt.tight_layout()
plt.savefig("loglog_fit.png")
plt.show()
