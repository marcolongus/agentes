#Animación de la epidemia a través de agentes. 

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.patches as patches
from tqdm import tqdm,trange
import os

colores = ['blue','red', 'green', ]
archivo = "data/animacion.txt"

##############################################################################################
#Animacion
##############################################################################################

def trayectoria(tpause = 0.01, animation = False):

	N = 100
	L = 50

	nsteps  = np.loadtxt(archivo, usecols=0).size/N
	fig, ax = plt.subplots()
	loop_range = int(nsteps);
	for i in trange(loop_range,mininterval=10):

		x = np.loadtxt(archivo, usecols=0, skiprows=N*i, max_rows=N)
		y = np.loadtxt(archivo, usecols=1, skiprows=N*i, max_rows=N)

		estado = np.loadtxt(archivo, usecols=3, skiprows=N*i, max_rows=N, dtype=int)

		plt.cla()

		plt.title("Agents system")
		plt.xlabel("x coordinate")
		plt.ylabel("y coordinate")

		plt.axis('square')
		plt.grid()
		plt.xlim(-1,L+1)
		plt.ylim(-1,L+1)

		for j in range(N):
			circ = patches.Circle((x[j],y[j]), 1, alpha=0.7, fc= colores[estado[j]])
			ax.add_patch(circ)

		plt.savefig("video/pic%.4i.png" %(i),dpi=150)
		#plt.pause(tpause)

	if animation:

		path = "C:/Users/Admin/Desktop/GIT/Agentes/video"
		print(os.getcwd())
		os.chdir(path)
		print(os.getcwd())
		os.system('cmd /k "ffmpeg -r 30 -f image2 -s 1920x1080 -i pic%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p test_0.mp4"')



####################################################################
####################################################################


trayectoria()
