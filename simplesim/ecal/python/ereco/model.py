"""Build a CNN model to reconstruct Ecal energy."""

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader

from . import utils

class Erecon(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 16, 2)
        self.batch = nn.BatchNorm1d(256)
        self.fc0 = nn.Linear(25, 256)
        self.fc1 = nn.Linear(256, 144)
        self.fc2 = nn.Linear(144, 84)
        self.fc3 = nn.Linear(84, 1)

        self._x_scale = None
        self._y_scale = None

        self._train_loss = []
        self._eval_loss = []

        # checked and updated in fit and predict
        self._to_cuda = False

        # flag to prep for training
        self._flag_train_setup = True

    def forward(self, x):
        #x = F.relu(self.conv1(x))
        x = torch.flatten(x,1)
        x = F.leaky_relu(self.fc0(x))
        #x = self.batch(x)
        x = F.leaky_relu(self.fc1(x))
        x = F.leaky_relu(self.fc2(x))
        x = self.fc3(x)
        return x

    def scalers(self, y, x):
        self._x_scale = (x.mean(axis=0), x.std(axis=0))
        self._y_scale = (y.mean(), y.std())

    def pre_fit(self):
        """instantiate loss and optimization classes."""
        self._flag_train_setup = False
        self._criterion = nn.MSELoss()
        self._eval_criterion = nn.MSELoss(reduction="sum")
        #optimizer = optim.SGD(self.parameters(), lr=0.001, momentum=0.9)
        self._optimizer = optim.Adam(self.parameters(), lr=0.001)

    def post_fit(self):
        """remove loss and optimizer after fitting."""
        self._flag_train_setup = True
        self._criterion = None
        self._eval_criterion = None
        self._optimizer = None

    def fit(self, y, x, eval_set=None, batch_size=50_000, epochs=20):

        if self._flag_train_setup:
            self.pre_fit()

        data = utils.EcalDataset(y, x, self._x_scale, self._y_scale)
        dataloader = DataLoader(data, batch_size=batch_size, shuffle=True)

        if next(self.parameters()).is_cuda:
            self._to_cuda = True

        if eval_set:
            eval_set = DataLoader(
                        utils.EcalDataset(
                            eval_set[0],
                            eval_set[1],
                            self._x_scale,
                            self._y_scale
                        ),
                        batch_size=batch_size,
                        shuffle=True
                    )

        for ep in range(epochs):
            self._train_epoch(dataloader, self._criterion, self._optimizer)
            self._train_loss.append(self._eval_epoch(dataloader, self._eval_criterion))
            if eval_set:
                self._eval_loss.append(self._eval_epoch(eval_set, self._eval_criterion))

    def _train_epoch(self, dataloader, lf, optimizer):
        size = len(dataloader.dataset)
        self.train()
        for batch, (x, y) in enumerate(dataloader):
            if self._to_cuda:
                x = x.cuda()
                y = y.cuda()
            pred = self(x)
            loss = lf(pred, y)

            loss.backward()
            optimizer.step()
            optimizer.zero_grad()

    def _eval_epoch(self, dataloader, lf):
        self.eval()
        loss = 0.
        with torch.no_grad():
            for x, y in dataloader:
                if self._to_cuda:
                    x = x.cuda()
                    y = y.cuda()
                pred = self(x)
                loss += lf(pred, y).item()

        return loss/len(dataloader.dataset)

    def predict(self, x, batch_size=50_000):
        dataloader = DataLoader(
            utils.EcalDataset(
                np.zeros(x.shape[0]),
                x,
                self._x_scale,
                self._y_scale
            ),
            batch_size=batch_size,
            shuffle=False
        )

        if next(self.parameters()).is_cuda:
            self._to_cuda = True

        self.eval()
        with torch.no_grad():
            for batch, (x, y) in enumerate(dataloader):
                start = batch*batch_size
                end = (batch+1)*batch_size
                if self._to_cuda:
                    x = x.cuda()
                    y = y.cuda()
                dataloader.dataset.y[start:end] = self(x).cpu().flatten()

        return dataloader.dataset.scaleY()
