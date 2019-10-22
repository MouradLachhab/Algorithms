import sys
import numpy as np
import matplotlib.pyplot as plt  # To visualize
import pandas as pd  # To read data
from sklearn.linear_model import LinearRegression

data = pd.read_csv(sys.argv[1])  # load data set
X = data.iloc[1:, 3].values.reshape(-1, 1)  # values converts it into a numpy array
Y = data.iloc[1:, 2].values.reshape(-1, 1)  # -1 means that calculate the dimension of rows, but have 1 column
linear_regressor = LinearRegression()  # create object for the class
linear_regressor.fit(X, Y)  # perform linear regression
Y_pred = linear_regressor.predict(X)  # make predictions

plt.scatter(X, Y)
plt.plot(X, Y_pred, color='red')
plt.savefig('test_constante' + '_' + sys.argv[2])

print(linear_regressor.coef_)
print(linear_regressor.intercept_)
