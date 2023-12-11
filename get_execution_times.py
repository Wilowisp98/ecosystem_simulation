import os
import pandas as pd

df = {
    'N': [],
    'Threads': [],
    'Exec Time': []
}
for file in os.listdir("outputs"):
    df['N'].append(int(file.split("x")[1].split("_")[0]))
    df['Threads'].append(int(file.split("threads")[-1].split(".")[0]))
    with open('outputs/'+file, 'r') as f:
        df['Exec Time'].append(f.readlines()[-1])
    df['Exec Time'][-1] = float(df['Exec Time'][-1].split(': ')[-1].split(" ")[0])

df = pd.DataFrame(df).sort_values(by=['N', 'Threads'], ascending=True)
df = df.pivot(index=['N'], columns=['Threads'])
# Calculate speedup
for c, n in df.columns:
    df[('SpeedUp', n)] = df[(c, 1)] / df[(c, n)]
df.to_latex()