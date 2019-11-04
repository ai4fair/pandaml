import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Draw Train History
def draw_train_history(history, draw_val=True, figsize=(12, 5)):
    """
    Make plots of training and validation losses and accuracies
    """
    #plt.style.use('ggplot')
    fig = plt.figure(figsize=figsize)
    
    # Plot Loss
    plt.subplot(121)
    plt.plot(history.epoch, history.history['loss'], label='Training set')
    if draw_val:
        plt.plot(history.epoch, history.history['val_loss'], label='Validation set')
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.xlim(0)
    plt.ylim(0., 1.0)
    plt.grid(True)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.title('Training Loss')
    plt.legend()

    # Plot Accuracy
    plt.subplot(122)
    plt.plot(history.epoch, history.history['acc'], label='Training set')
    if draw_val:
        plt.plot(history.epoch, history.history['val_acc'], label='Validation set')
    plt.xlabel('Epochs')
    plt.ylabel('Accuracy')
    plt.xlim(0)
    plt.ylim(0., 1.0)
    plt.grid(True)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.title('Training Accuracy')
    plt.legend(loc=0)
    
    plt.tight_layout()
    plt.savefig('train_history.png')
    return fig


# Draw Train History from Pickle (Saved File)
def draw_pickled_history(history, draw_val=True, figsize=(12, 5)):
    """
    Plot history from the pickle file.
    """
    fig = plt.figure(figsize=figsize)
    
    # Plot training & validation accuracy values
    plt.subplot(121)
    plt.plot(history['loss'], label='Training set')
    if draw_val:
        plt.plot(history['val_loss'], label='Validation set')
    plt.title('Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.xlim(0)
    plt.ylim(0., 1.0)
    plt.grid(True)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.legend()
    
    # Plot training & validation loss values
    plt.subplot(122)
    plt.plot(history['acc'], label='Training set')
    if draw_val:
        plt.plot(history['val_acc'], label='Validation set')
    plt.title('Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.xlim(0)
    plt.ylim(0., 1.0)
    plt.grid(False)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.legend()
    
    plt.tight_layout()
    return fig
   
# Draw Event
def draw_single_event(event=None, event_id=9999, name="event"):
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

# Draw Reco Event
def draw_reco_event(event=None, reco_event=None, event_id=9999, name="reco_event"):
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
