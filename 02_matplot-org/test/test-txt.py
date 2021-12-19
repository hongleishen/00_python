import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

fig = plt.figure()
def call_back(event):
    axtemp=event.inaxes
    x_min, x_max = axtemp.get_xlim()
    fanwei = (x_max - x_min) / 10
    if event.button == 'up':
        axtemp.set(xlim=(x_min + fanwei, x_max - fanwei))
        print('up')
    elif event.button == 'down':
        axtemp.set(xlim=(x_min - fanwei, x_max + fanwei))
        print('down')
    fig.canvas.draw_idle()  # 绘图动作实时反映在图像上
fig.canvas.mpl_connect('scroll_event', call_back)
fig.canvas.mpl_connect('button_press_event', call_back)

# -------------------------


# ax.text(left, bottom, 'left top',
        # horizontalalignment='left',
        # verticalalignment='top',
        # transform=ax.transAxes)
#ax = fig.add_axes([0, 0, 1, 1])
#ax = plt.subplots(111)       
# ax.text(0.1,0.2,'in 1.1,0.2', horizontalalignment='left',
        # verticalalignment='top',
        # transform=ax.transAxes)
        
        
        


#fig,ax = plt.subplots(111) 
fig, ax = plt.subplots(nrows=1)

x = np.linspace(0,49,50)
y = np.sin(x)
ax.plot(x,y,alpha = 0.2)

plt.show()