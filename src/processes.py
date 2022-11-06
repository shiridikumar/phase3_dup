import subprocess
import threading
def execute():
    subprocess.Popen('./server <"cmds.txt" ', shell=True)
l=[]
for i in range(100):
    t = threading.Thread(target=execute)
    t.start()
    l.append(t)
for i in range(len(l)):
    l[i].join()
# subprocess.Popen('./server <"cmds.txt" ', shell=True)