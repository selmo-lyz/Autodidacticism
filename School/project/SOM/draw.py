import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pylab import savefig

origin_data_name = "output_test_result.csv"
origin_df = pd.read_csv(origin_data_name, header=None, names=["x", "y", "class"])
print(origin_df)

g = sns.scatterplot(x="x", y="y", hue="class", data=origin_df)
fig = g.get_figure()
fig.savefig("output.png", dpi=400)