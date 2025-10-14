import numpy as np

import torch
from torch.utils.data import Dataset


class EcalDataset(Dataset):
    def __init__(self, y, x, x_standard=None, y_standard=None):
        self.y = y.copy()
        self.x = x.copy()

        self.x_standard = x_standard
        if x_standard is None:
            self.x_standard = (
                    np.mean(self.x, axis=0),
                    np.std(self.x, axis=0)
                )

        self.y_standard = y_standard
        if y_standard is None:
            self.y_standard = (
                    np.mean(self.y),
                    np.std(self.y)
                )

    def __len__(self):
        return len(self.y)

    def __getitem__(self, idx):
        xt = torch.Tensor(
            (self.x[idx] - self.x_standard[0])/self.x_standard[1]
            )
        yt = torch.Tensor(
            [(self.y[idx] - self.y_standard[0])/self.y_standard[1]]
            )
        return xt, yt

    def scaleY(self):
        return self.y*self.y_standard[1] + self.y_standard[0]

