import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import sys
import os


def plot_results():

    data_a = []
    tiempo_a = []
    with open('../src/output_a.txt', 'r') as f:
        for line in f:
            vals = line.split()
            n = int(vals[0])
            tiempo = float(vals[1])
            data_a.append([n, tiempo])
            tiempo_a.append(tiempo)

    df_a = pd.DataFrame(data_a, columns=['n', 'tiempo'])

    data_rand = []
    tiempo_rand = []
    with open('../src/output_rand.txt', 'r') as f:
        for line in f:
            vals = line.split()
            n = int(vals[0])
            tiempo = float(vals[1])
            data_rand.append([n, tiempo])
            tiempo_rand.append(tiempo)

    df_rand = pd.DataFrame(data_rand, columns=['n', 'tiempo'])

    print(df_a)
    print(df_rand)

    print(tiempo_a)
    print(tiempo_rand)

plot_results()