#!/usr/bin/env python
# coding: utf-8

# Package Imports
import math
import random
import numpy as np
import pandas as pd
import dask.dataframe as dd
import matplotlib.pyplot as plt

from tqdm import tqdm_notebook
from tqdm import tnrange
from sklearn.preprocessing import StandardScaler

# Load all events using Dask
def dask_events(path="None"):
    """Load multiple CSV files into Dask"""
    hits = dd.read_csv(path+'*-hits.csv')
    truth = dd.read_csv(path+'*-truth.csv')
    particles = dd.read_csv(path+'*-particles.csv')
    tubes = dd.read_csv(path+'*-tubes.csv')

    # convert to pandas format, if required.
    # hits = hits.compute()
    # truth = truth.compute()
    # particles = particles.compute()
    # tubes = tubes.compute()
    
    return hits, truth, particles, tubes


# Load all events using Pandas
def pandas_events(path="None"):
    """Load multiple CSV files into Pandas"""
    
    # map(function, iterable) == map(pd.read_csv, files) where variable
    # files = glob.glob(os.path.join(path+"*-hits.csv")) is a list
    
    hits = pd.concat(map(pd.read_csv, glob.glob(os.path.join(path+"*-hits.csv"))))
    truth = pd.concat(map(pd.read_csv, glob.glob(os.path.join(path+"*-truth.csv"))))
    particles = pd.concat(map(pd.read_csv, glob.glob(os.path.join(path+"*-particles.csv"))))
    tubes = pd.concat(map(pd.read_csv, glob.glob(os.path.join(path+"*-tubes.csv"))))
    
    return hits, tubes, truth, particles


def get_geometry(events=None):
    """
    Pass all events, it will extract (x,y,z) values based on unique
    tube_ids. Sort it, dump it as a csv file.
    """
    df = events.drop_duplicates(subset='tube_id', keep="first")
    df = df[['x', 'y', 'z', 'skewed', 'tube_id', 'layer_id', 'sector_id']]
    df = df.sort_values(['tube_id', 'layer_id'])
    df.to_csv('stt.csv', encoding='utf-8', index=False)
    

def load_event(path='', event_prefix=1):
    '''
    Loads data arranged in .csv files, two .csv files
    per event one for detector related information, 
    the other related to MC truth information.
    
    Inputs:
    1. event_prefix.
    2. path where .csv files are located.
    '''
    path_detector = path+"box_detector_"+str(event_prefix)+".csv"
    path_real     = path+"box_real_"    +str(event_prefix)+".csv"
    event_i = pd.read_csv(path_detector, encoding='utf-8')
    event_j = pd.read_csv(path_real, encoding='utf-8')
    event = event_i.merge(event_j, on=['hit_id'],how='left')
    del event_i
    del event_j

    if not event.empty:
        pd.set_option('display.max_columns', 50)
        return event

def load_events(path='', start=0, end=100):
    '''
    Loads number of events.
    Inputs:
    1. strat: starting event prefix.
    2. end: end event prefix.
    '''
    dfs = []
    for index in range(start, end):
        df = load_event(path, index)
        dfs.append(df)
    
    print("events", (end-start))
    return dfs

def pass_event(df=None):
    '''
    Skip short straws, which are above and below
    the beam hole (have y value that is not zero),
    because x-z hit projection is not at y=0.
    '''
    dfCopy = df.copy()
    flag = (np.any(dfCopy.y.values)==0)
    return flag

def preprocess(event_list, nEvents=1000):
    """
    Function to do post processing tasks on the loaded events. For example
    to do selection on data. Selecting a dataset based on only non-skewed 
    layers or selecting only certain layers etc.
    """
    index = 0
    new_list = []
    for l in range(len(event_list)):
        index = index+1
        df = event_list[l]
        if df is not None:
            if (index%100 == 0):
                print('event', index)
            
            # Start: Preprocessing Tasks
            df = df.drop(['z', 'tube_id','depcharge', 'tx', 'ty', 'tz','px','py','pz'], axis=1)
            
            # df = event[['skewed', 'layer_id']]   # select only 2 columns
            # df = df[df['skewed'] == 0]           # all non skewed
            df = df.query('skewed==0')
            # df = df[df['skewed'] == 1]           # all skewed, layers = 8,9,2,...15 are skewed
            # df = df.query('skewed==1')
         
            # df = df[df['layer_id'] < 8]          # layers = 0,1,2,...7 are before skewed
            # df = df[df['layer_id'] > 15]         # layers = 16,17,...24 are after skewed
            # df = df[(df['layer_id'] > 7) & 
            # (df['layer_id'] < 16)]               # layers = 8,9,2,...15 are skewed

            # df.cluster_id.unique()
            # df.layer.unique()
            # df[df["layer"] < 3].head()
            # df[(df["layer"] < 5) & (df["layer"] > 3)].head()
            # df.loc[df["layer"].isin([2, 3])].head()
            # df.loc[~df["layer"].isin([2, 3])].head()
            # df.loc[(df["cluster_id"]==8) & df["layer"].isin([2, 3])].head()
            
            # End
            new_list.append(df)
        if index>nEvents: break
    return new_list


# Function to create HitPairs
def prepare_train_set(events, n_events=1):
    """
    Training data is hit-pairs in all possible layers
    events: list of dataframes (event)
    """
    number_of_events = 0
    X = []                               # input features as list
    y = []                               # output as a list
    for e in tnrange(len(events)):
        number_of_events += 1
        df = events[e]
        if df.empty: continue       
        data = np.array(df)
        
        # PLEAZE check the right indexing of the dataframe (print (df.head(0)))
        # 0=hit_id, 1=x, 2=y, 3=skewed, 4=layer_id, 5=sector_id, 6=isochrone radius, 7=particle_id
        for i in data:
            for j in data:
                if (i[0]==j[0]): continue                   # if hits are same then skip
                # if (abs(i[4]-j[4])!=1): continue            # only adjacent layers

                # distance between hits
                dirV_x = j[1]-i[1]
                dirV_y = j[2]-i[2]
                dist   = math.sqrt(dirV_x**2 + dirV_y**2)
                if i[7]==j[7]:
                    X.append([i[1], i[2], i[3], i[4], i[5], 
                              j[1], j[2], j[3], j[4], j[5], 
                              abs(dist-(i[6]+j[6]))])
                    y.append([1])
                else:
                    X.append([i[1], i[2], i[3], i[4], i[5], 
                              j[1], j[2], j[3], j[4], j[5], 
                              abs(dist-(i[6]+j[6]))])
                    y.append([0])
       
        if number_of_events > n_events: break
    
    # Covert X,y list as ndarray
    X = np.vstack(X)
    y = np.vstack(y)
    combined = list(zip(X, y))  # make list of iterators on X,y as tuple, use tuple(combines) to print the values
    random.shuffle(combined)    # Shuffle list
    X[:], y[:] = zip(*combined) # operator * can be used to unzip a list >> ndarray
    return X,y


def prepare_test_set(df=None):
    """
    In order to reconstruct a tracks for a single event, first we need to prepare 
    the data (X) which can be used in the model for prediction. But we also create
    a new variable (y) which is hit-pair
    """   
    X = []
    y = []
    data   = np.array(df)
    length = len(data)
    
    # PLEAZE check the right indexing of the dataframe
    # 0=hit_id, 1=x, 2=y, 3=skewed, 4=layer_id, 5=sector_id, 6=isochrone, 7=particle_id
    for i in data:
        for j in data:
            if(i[0]==j[0]): continue
            #if(abs(i[4]-j[4])!=1): continue            # only adjacent layers
            #if not abs(j[4]-i[4])<=1: continue        # only adjacent layers
            
            dirV_x = j[1]-i[1]
            dirV_z = j[2]-i[2]
            dist   = math.sqrt(dirV_x**2 + dirV_z**2)
            
            # X = [(x, y, skewed, layer_id, sector_id)_i, (x, y, skewed, layer_id, sector_id)_j, dist(i,j)]
            X.append([i[1],i[2],i[3],i[4],i[5], 
                      j[1],j[2],j[3],j[4],j[5], 
                      abs(dist-(i[6]+j[6]))])
            
            # y = [hit_id_i, hit_id_j]
            y.append([i[0],j[0]])     
    
    # convert list as numpy array by stacking.
    X = np.vstack(X)            
    y = np.vstack(y)
    combined = list(zip(X, y))  # make list of iterators on X,y as tuple, use tuple(combines) to print the values
    random.shuffle(combined)    # Shuffle list
    X[:], y[:] = zip(*combined) # * operator can be used to unzip a list
    return X,y

# Function to create HitTriplets
def prepare_triplets(events, n_events=1):
    """
    Training data is hit-pairs in all possible layers
    events: list of dataframes (event)
    """
    number_of_events = 0
    X = []                               # input features as list
    y = []                               # output as a list
    for e in tnrange(len(events)):
        number_of_events += 1
        df = events[e]
        if df.empty: continue              
        data = np.array(df)
        
        # PLEAZE check the right indexing of the dataframe (print (df.head(0)))
        # 0=hit_id, 1=x, 2=y, 3=skewed, 4=layer_id, 5=sector_id, 6=isochrone radius, 7=particle_id
        
        for i in data:
            for j in data:
                if (i[0]==j[0]): continue                            # if hits are same then skip
                for k in data:
                    if (i[0]==k[0] or j[0]==k[0]): continue          # if hits are same then skip
                        
                    # distance between hits, not sure about triplets
                    # dirV_x = j[1]-i[1]
                    # dirV_y = j[2]-i[2]
                    # dist   = math.sqrt(dirV_x**2 + dirV_y**2)
                    if i[7]==j[7]:
                        X.append([i[1], i[2], i[3], i[4], i[5], 
                                  j[1], j[2], j[3], j[4], j[5],
                                  k[1], k[2], k[3], k[4], k[5]])
                                  # abs(dist-(i[6]+j[6]))])
                        
                        y.append([1])
                    else:
                        X.append([i[1], i[2], i[3], i[4], i[5],
                                  j[1], j[2], j[3], j[4], j[5], 
                                  k[1], k[2], k[3], k[4], k[5]])
                                  # abs(dist-(i[6]+j[6]))])
                        y.append([0])
       
        if number_of_events > n_events: break
    
    # Covert X,y list as ndarray
    X = np.vstack(X)
    y = np.vstack(y)
    combined = list(zip(X, y))  # make list of iterators on X,y as tuple, use tuple(combines) to print the values
    random.shuffle(combined)    # Shuffle list
    X[:], y[:] = zip(*combined) # operator * can be used to unzip a list >> ndarray
    return X,y

# Plot Event
def plot_single_event(event=None, event_id=9999, name="event"):
    p_ids = np.unique(event.particle_id.values)
    det = pd.read_csv("stt.csv")
    # plt.style.use('seaborn')
    fig = plt.gcf()
    plt.figure(figsize=(10,10))
    skw = det.query('skewed==0')
    nkw = det.query('skewed==1')
    plt.scatter(skw.x.values, skw.y.values, s=30, facecolors='none', edgecolors='lightgreen')
    plt.scatter(nkw.x.values, nkw.y.values, s=30, facecolors='none', edgecolors='coral')
    for i in p_ids:
        df_ = event.loc[event.particle_id==i]
        plt.scatter(df_.x.values, df_.y.values, s=30, label='track_id: %d'%i)
    plt.title('Reconstructed Event # %d'%event_id)
    plt.xlabel('x [cm]', fontsize=10)
    plt.ylabel('y [cm]', fontsize=10)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.xlim((-41,41))
    plt.ylim((-41,41))
    #plt.axis('scaled')
    plt.grid(False)
    plt.legend(fontsize=10, loc='best')
    plt.savefig(name+'_%d.png'%event_id)
    # plt.show()
    # return fig

# Plot Reco Event
def reco_single_event(event=None, reco_event=None, event_id=9999, name="reco_event"):
    
    p_ids = np.unique(event.particle_id.values)
    
    det = pd.read_csv("stt.csv")
    # plt.style.use('seaborn')
    fig = plt.gcf()
    plt.figure(figsize=(10,10))
    skw = det.query('skewed==0')
    nkw = det.query('skewed==1')
    plt.scatter(skw.x.values, skw.y.values, s=30, facecolors='none', edgecolors='lightgreen')
    plt.scatter(nkw.x.values, nkw.y.values, s=30, facecolors='none', edgecolors='coral')
    for i in p_ids:
        df_ = event.loc[event.particle_id==i]
        plt.scatter(df_.x.values, df_.y.values, s=30, label='MC Track: %d'%i)
    
    plt.scatter(reco_event.x.values, reco_event.y.values, 
                facecolors='none', edgecolors='red', 
                s=100,
                linewidth=1, 
                label='Recon Track')
    
    plt.title('Reconstructed Event # %d'%event_id)
    plt.xlabel('x [cm]', fontsize=10)
    plt.ylabel('y [cm]', fontsize=10)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.xlim((-41,41))
    plt.ylim((-41,41))
    #plt.axis('scaled')
    plt.grid(False)
    plt.legend(fontsize=10, loc='best')
    plt.savefig(name+'_%d.png'%event_id)
    # plt.show()
    # return fig