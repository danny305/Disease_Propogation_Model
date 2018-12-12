"""
Danny Diaz
dd32387
12-2-18
Disease Propogation Project:Data Analysis.
"""

import subprocess as sp
from pprint import pprint

import pandas as pd
import matplotlib.pyplot as plt

def create_data(runs=10):
    pass


def process_data(path):
    df = pd.read_csv(path,names=['person_id', 'health','sick_days_left', 'age'], skiprows=[0])
    df.set_index(['age','person_id'],inplace=True)
    groups = df.groupby('age')
    health_stat_each_day =  [ ( day,group['health'].value_counts() ) for day, group in groups if group]
    all_groups = [group for age,group in groups]
    #pprint(all_groups[1]['health'].value_counts())
    #num_sick_each_day = [day['health'].value_counts() for day in all_groups]
    pprint(health_stat_each_day)

    #return num_sick_each_day

    #print(df.groupby(['age','person_id']).count())















def main():
    #process_data('../csv_data/dis_prop_12-1-2018_14:1.csv')
	process_data('../csv_data/dis_prop_12-1-2018_16:37.csv')




if __name__ == "__main__":
	main()
