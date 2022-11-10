import pandas as pd
import numpy as np
n=16000;m=400
data=list(range(-10,10))
a = np.random.choice(data,n)
b = [0 for i in range(n)]
e=np.random.choice(data,n);
c=np.random.choice(data,m)
d=[1 for i in range(m)]

df=pd.DataFrame()
df["a"]=a;df["b"]=b;df["e"]=e
df.set_index("a",inplace=True)

df.to_csv("../data/A4.csv")
df=pd.DataFrame();df["c"]=c;df["d"]=d;df.set_index("c",inplace=True)
df.to_csv("../data/C4.csv")