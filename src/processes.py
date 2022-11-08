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
    subprocess.Popen('./server {} <"cmds.txt"'.format(update_flag), shell=True)

l=[]
for i in range(10):
    t = threading.Thread(target=execute)
    t.start()
    l.append(t)

for i in range(len(l)):
    l[i].join()