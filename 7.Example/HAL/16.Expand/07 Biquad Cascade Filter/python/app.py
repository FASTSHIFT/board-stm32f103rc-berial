import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("output.txt", sep='\t', header=None)

src = np.array(data[0])  # ԭ����
dst = np.array(data[1])  # �˲����

plt.plot(src)
plt.plot(dst)
