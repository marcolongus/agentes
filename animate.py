#Animation constructor with ffmepg.

import os

path = "C:/Users/Admin/Desktop/GIT/Agentes/video"

print(os.getcwd())
os.chdir(path)
print(os.getcwd())

os.system('cmd /k "ffmpeg -r 30 -f image2 -s 1920x1080 -i pic%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4"')
