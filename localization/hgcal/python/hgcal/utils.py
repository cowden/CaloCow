import numpy as np
import pickle

import torch


class Standardizer:
    def __init__(self, y, X):
        self._y_mu = np.mean(y)
        self._y_std = np.std(y)

        self._x_mu = np.mean(X, axis=0)
        self._x_std = np.std(X, axis=0)
        self._x_std[self._x_std == 0] = 1.

    @property
    def y_mu(self):
        return self._y_mu

    @property
    def y_std(self):
        return self._y_std

    @property
    def x_mu(self):
        return self._x_mu

    @property
    def x_std(self):
        return self._x_std

    def transform(self, y, x):
        yt = (y - self._y_mu)/self._y_std
        Xt = (x - self._x_mu)/self._x_std
        Xt[np.isnan(Xt)] = 0.

        return yt, np.expand_dims(Xt, 1)

def load_standardizer(pth):
    with open(pth,"rb") as f:
        std = pickle.load(f)
    return std

def save_standardizer(std, pth):
    with open(pth, "wb") as f:
        pickle.dump(std, f)

def load_checkpoint(pth):
    return torch.load(pth)

def save_checkpoint(mod, pth):
    torch.save(mod, pth)
