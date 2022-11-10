import subprocess
import threading
def execute():
    f=open("cmds.txt","r")
    update_flag="READ"
    for i in f.readlines():
        i=i.split()
        if(i[0]=="UPDATE"):
            update_flag="WRITE"
            break
    # update_flag="READ"
    subprocess.Popen('./server {} <"cmds.txt"'.format(update_flag), shell=True)

l=[]
# calling 5 transaction using 5 threads
for i in range(5):
    t = threading.Thread(target=execute)
    t.start()
    l.append(t)

for i in range(len(l)):
    l[i].join()