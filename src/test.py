import pandas as pd
df=pd.read_csv("../data/A3.csv")
df1=pd.read_csv("../data/S1.csv")
df.sort_values("e",inplace=True)
cols=list(df.columns)

df=df.values
df1=df1.values
import numpy as np

vals,counts=np.unique(df,return_counts=True,axis=0)
print(vals,counts)

vals1,counts1=np.unique(df1,return_counts=True,axis=0)
flag=True
for i in range(len(df)-1):
    if(df[i][cols.index("e")]>df[i+1][cols.index("e")]):
        flag=False
        break

        

print((vals==vals1).all() and (counts==counts1).all() and flag)

