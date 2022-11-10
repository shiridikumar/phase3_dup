import pandas as pd
df=pd.read_csv("../data/A3.csv")
df1=pd.read_csv("../data/S.csv")
df.sort_values("e",inplace=True)
df=df.values
df1=df1.values
for i in range(len(df)):
    print(df[i]==df1[i])
