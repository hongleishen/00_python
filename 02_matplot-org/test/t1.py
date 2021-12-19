
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np



x = np.linspace(0,49,50)
y = np.sin(x)
#ax.plot(x,y,alpha = 0.2)


#fig, (ax0, ax1, ax2,ax3,ax4,ax5) = plt.subplots(nrows=2,nclos=3)


#唐老师方法
ax1 = plt.subplot(211)
plt.plot(x,y)

ax2 = plt.subplot(212)
plt.plot(x,y)

plt.show()


