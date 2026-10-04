
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("build/paths.csv")

plt.figure(figsize=(10, 5))
for col in df.columns[1:]:
    plt.plot(df['Step'], df[col], alpha=0.5, linewidth=0.8)

plt.title("Basic_Quant_engine - Monte Carlo Simulation Paths")
plt.xlabel("Time Steps")
plt.ylabel("Asset Price")
plt.grid(True, linestyle="--", alpha=0.5)

plt.savefig("monte_carlo_paths.png", dpi=300, bbox_inches='tight')
plt.show()
