---
title: 'DS Phase-3 Report'
author: 
- P Shiridi Kumar (2021121005)
- Shikhar Saxena (2021121010)
header-includes:
- |
  ```{=latex}
  \usepackage{graphicx}
  ```
---

# Theoretical Analysis of UPDATE command

## a. Two Simultaneous Reads on a table

Two simultaneous read operations on the table will not be conflicting as the data in the table remains unchanged.


## b. One Read and One Write happening simultaneously on a table

One Read and One Write operations on the table will be conflicting. The Conflicts that can arise in this situation are as follows:

- **The Dirty Read Problem**: Let's say the write operation (transaction $T_1$) happens before the read operation (transaction $T_2$). So, the read operation has read the updated data value (because of the write operation). Now if $T_1$ fails for some reason, then in $T_2$ we still keep working on the temporarily updated value (instead of using the original data value) which results in a conflicting scenario.

## c. Two Simultaneous Writes on a table

Two Simultaneous Write operations on the table will be conflictiong. The Conflicts that can arise in this situationa are as follows:

- **The Lost Update Problem**: Let's say the write operation (transaction $T_1$) happens before the write operation (transaction $T_2$). Let's say $T_1$ and $T_2$ update the data value $X$ to $X_1$ and $X_2$ respectively. Also assume that both read the value of $X$ simultaneously. So, value of $X$ will be changed to $X_1$ which then will be overwritten by the value $X_2$.  Thus, we lost the update for transaction $T_1$ in this case which creates the conflicting scenario. To maintain consistency, transaction $T_2$ should've read the value of $X$ after $T_1$'s update. 


# Our Learnings for this Problem

In this phase we learnt how the nuances of transaction management.

We also learnt how `SimpleRA` works differently than the popular database systems, in regards to transaction management. Essentially, each `SimpleRA` server instance, launches with their own dedicated `BufferManager`, `TableCatalogue` and so on. Whereas, in a popular database systems, the `server` and the `client` features are separate. Essentially, only one `server` instance is launched or if more are launched then they share the `BufferManager`, `TableCatalogue`, etc. Next, we can launch multiple `client` instances which will parse the queries and connect with the server to get the requisite output.

# Our Implementation / Approach
We have implemented a file locking system for handling concurrent transactions and to maintain ACID properties . We have used flock from c standard (posix standard) for file locking . We lock the page files and also the output files using Shared_locks and Exclusive_locks . Shared_locks are used when the transaction is a read and exclusive lock is used when 
a transaction is write or update . 

Firstly we have implemented a update command. In order to do multiple transactions simultaneously we created a python script which runs multiple threads and each thread will execute a commands present in cmds.txt ,i.e,  each thread will be performing a transaction. For this project we find the type of transactions (read transaction / write transaction ) by parsing cmds.txt in processes.py .since we we need load a table in order to update we consider both load and update operation present in cmds.txt as on transaction.

Steps to run simultaneous transactions :
> python3 proccesses.py

cmds.txt contains the transaction that would be runned by several threads
In order to indicate a transaction is a write we pass a flag as argument to ./server in processes.py




# Video Documentation

The video documentation for Part-2 can be viewed here: [https://iiitaphyd-my.sharepoint.com/:v:/g/personal/shikhar_saxena_research_iiit_ac_in/Ebvs18Nk2BtChHqQDI9ySaMB_mnD-xjOBJDAIYT10GCtGw?e=GvcQNC](https://iiitaphyd-my.sharepoint.com/:v:/g/personal/shikhar_saxena_research_iiit_ac_in/Ebvs18Nk2BtChHqQDI9ySaMB_mnD-xjOBJDAIYT10GCtGw?e=GvcQNC)