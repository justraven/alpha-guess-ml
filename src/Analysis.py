#!/usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt

# Read in the data
df = pd.read_csv('../data/dataset/01_Merged_alpha.csv', delimiter=',')
print('--HEAD--\n', df.head())
print('\n--DESCRIBE--\n', df.describe())

# check target class percentage
data = df['Alpha'].value_counts()
data.plot(kind='pie', autopct='%1.1f%%', colors=['#006699', '#ff0000'])
plt.show()

# check histogram of target class
df.hist(figsize=(10, 10), bins=20, color='#006699')
plt.show()

# check correlation between features
print('\n--CORRELATION--\n', df.corr())