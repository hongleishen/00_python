import numpy as np
import matplotlib.pyplot as plt
%matplotlib inline
# Fixing random state for reproducibility
np.random.seed(19680801)

n =1
X = np.random.rand(100, 200)
xs = np.mean(X, axis=1)
ys = np.std(X, axis=1)
print(xs.shape)

fig, (ax, ax2) = plt.subplots(2, 1)
ax.set_title('click on point to plot time series')
line, = ax.plot(xs, ys, 'o', picker=1)  # 5 points tolerance


def press_call_back(event):
    if event.key == 'n':
        inc = 1
    else:
        inc = -1
    
    n += inc
    print(n)
    
    ax2.cla()
    ax2.plot(X[n])





fig.canvas.mpl_connect('key_press_event', press_call_back)