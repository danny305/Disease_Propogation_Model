"""
Danny Diaz
dd32387
12-2-18
Disease Propogation Project:Data Analysis.
"""

import subprocess as sp
import os

from pprint import pprint
from datetime import datetime


import pandas as pd
import matplotlib.pyplot as plt



def create_vaccination_data(pop_size=10000, data_points=35, percent_vacc=0.98, num_interaction=10, inf_rate = 0.05):

    vaccination_range = int(pop_size * percent_vacc)
    step_size = int(vaccination_range//data_points)

    all_df = []
    for num_vacc in range(0,vaccination_range,step_size):
        sp.call(['./pop_cli_no_cout.bin', str(pop_size), '1', str(num_vacc),'7',
                 str(num_interaction), str(inf_rate), 'true'])

        date_time = datetime.today().strftime('%-m-%-d-%Y_%H:%-M')
        file_name = f"dis_prop_{date_time}-{num_vacc}.csv"
        path = f'../csv_data/{file_name}'
        try:
            agg_data = process_raw_data(path)
        except (FileNotFoundError, FileExistsError):
            continue

        rem_healthy = agg_data.tail(1)['Uninfected']
        normalized_rem_healthy = rem_healthy.apply(lambda x: round(x/agg_data['Uninfected'][0],2))
        print(normalized_rem_healthy)
        all_df.append(normalized_rem_healthy)

#I should seperate this into another function but then I have to pass all the arguments of this function to the new one.

    still_healthy_perc = [df.apply(lambda x: x * 100).values for df in all_df]
    days_per_df = [ df.index.values for df in all_df]

    x = [100*index*step_size/pop_size for index in range(len(still_healthy_perc))]
    y1 = still_healthy_perc
    y2 = days_per_df

    fig, ax1 = plt.subplots()
    fig.suptitle('Herd Immunity')
    ax1.set_title(f'Pop Size:{pop_size} Inf Rate:{inf_rate} Num Interactions/day: {num_interaction}')
    ax1.plot(x, y1, 'ro')

    ax1.set_xlabel('Vaccinated (%)')
    ax1.set_ylabel('Not Vaccinated (%) that does not get infected',color='r')

    ax2 = ax1.twinx()
    ax2.plot(x,y2,'bo')
    ax2.set_ylabel("Disease Duration (days)",color='b')

    plt.show()


def process_raw_data(path):
    df = pd.read_csv(path,names=['person_id', 'health','sick_days_left', 'age'], skiprows=[0])
    df.set_index(['age','person_id'],inplace=True)
    groups = df.groupby('age')
    #health_stat_each_day =  [ ( day,group['health'].value_counts() ) for day, group in groups]
    all_groups = [group for age,group in groups]

    agg_data = pd.DataFrame([day['health'].value_counts() for day in all_groups],
                            index=[day for day in range(len(all_groups))])
    agg_data.rename({-1 :"Sick", 0 :"Uninfected", 1 :"Recovered", 2 :"Vaccinated"}, axis='columns', inplace=True)
    agg_data.fillna(0, inplace=True); agg_data.index.name = "Day"

    pprint(agg_data)
    return agg_data


#To Do Figure out how to calculate the mean of three of the same models in order to smooth out anamolies.
def compute_avg_df():
    averages = pd.concat([each.stack() for each in dfs], axis=1) \
        .apply(lambda x: x.mean(), axis=1) \
        .unstack()



def plot_data(agg_data):
    agg_data.plot(y='Uninfected', use_index=True, stacked=True)
    agg_data.plot(y='Sick', stacked=True)
    plt.show()
















def main():
    #data = process_data('../csv_data/dis_prop_12-15-18_17:42.csv')
	#process_data('../csv_data/dis_prop_12-15-2018_18:56.csv')
    create_vaccination_data(inf_rate=0.05)
    create_vaccination_data(inf_rate=0.1)
    create_vaccination_data(inf_rate=0.15)
    create_vaccination_data(inf_rate=0.2)
    create_vaccination_data(inf_rate=0.25)
    create_vaccination_data(inf_rate=0.3)
    create_vaccination_data(inf_rate=0.35)
    create_vaccination_data(inf_rate=0.4)
    create_vaccination_data(inf_rate=0.45)
    create_vaccination_data(inf_rate=0.5)
    create_vaccination_data(inf_rate=0.55)
    create_vaccination_data(inf_rate=0.60)
    create_vaccination_data(inf_rate=0.65)
    create_vaccination_data(inf_rate=0.70)
    create_vaccination_data(inf_rate=0.75)
    create_vaccination_data(inf_rate=0.80)
    create_vaccination_data(inf_rate=0.85)
    create_vaccination_data(inf_rate=0.90)



    #sp.call(['./pop_cli_no_cout.bin', '1000', '1', '50', '7', '10', '0.05', 'true'])



if __name__ == "__main__":
	main()
