
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("build/simulation_paths.csv")
for col in df.columns[1:]:
    plt.plot(df["Step"], df[col], alpha=0.6)
plt.title("Trajectoires Monte-Carlo - Moteur Quantitatif")
plt.xlabel("Pas de temps")
plt.ylabel("Spot")
plt.show()