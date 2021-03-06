# Python code for 2D random walk.
import numpy as np
import pylab
import random
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from scipy import stats
from scipy.stats import norm

sns.set_style('darkgrid')

#defining function for fitting pf plot
def normal(mean, std, color="black"):
    a = np.linspace(mean-4*std, mean+4*std, 200)
    b = stats.norm.pdf(a, mean, std)
    c = plt.plot(a, b, color, linewidth=2)


# defining the number of steps
n = 1000   # nmber of runs
m = 1000   # number of steps per run
xfinal = np.zeros(n)
yfinal = np.zeros(n)


#creating two array for containing x and y coordinate
#of size equals to the number of size and filled up with 0's

for k in range (1,n):
    x = np.zeros(n)
    y = np.zeros(n)
    
    # filling the coordinates with random variables
        
    for i in range (1, m):

        num = random.random()

        if num >= 0 and num < 0.25:
            x[i] = x[i - 1] + 1
            y[i] = y[i - 1]
        elif num >= 0.25 and num < 0.5:
            x[i] = x[i - 1] - 1
            y[i] = y[i - 1]
        elif num >= 0.5 and num < 0.75:
            x[i] = x[i - 1]
            y[i] = y[i - 1] + 1
        else:
            x[i] = x[i - 1]
            y[i] = y[i - 1] - 1
                
    xfinal[k] = x[i]
    yfinal[k] = y[i]  
            

        
# plotting stuff:


# histogram and fit

#sns.distplot(xfinal, fit=norm, kde=False)
sns.histplot(xfinal, stat = 'density', color = 'red')
normal(xfinal.mean(), xfinal.std())
pylab.show()

sns.histplot(yfinal, stat = 'density', color = 'blue')
normal(yfinal.mean(), yfinal.std())
pylab.show()
