"""Build models for CaloX hgcal."""

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

class Erecon(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv3d(1, 2, 5)
        self.pool = nn.MaxPool3d(3)
        self.conv2 = nn.Conv3d(2, 2, 5)
        #self.conv3 = nn.Conv3d(2, 2, 5)

        self.fc1 = nn.Linear(2*3*3*6, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 1)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        #x = self.pool(F.relu(self.conv3(x)))

        x = torch.flatten(x, 1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

    def fit(self, y, x):
        criterion = nn.MSELoss()
        optimizer = optim.SGD(self.parameters(), lr=0.001, momentum=0.9)

        batch_labels = np.arange(15)
        batches = np.random.choice(batch_labels, size=len(y))

        for epoch in range(20):
            running_loss = 0.

            for bl in batch_labels:
                indx = np.argwhere(batches == bl).flatten()
                yb = y[indx].cuda()
                xb = x[indx].cuda()
                
                optimizer.zero_grad()

                yhat = self(xb)
                loss = criterion(yhat, yb)
                loss.backward()
                optimizer.step()

                running_loss += loss.item()
                print(f"{epoch + 1}, {bl + 1} loss: {running_loss}")
                running_loss = 0.

    def predict(self, x):
        batch_size=700
        result = np.zeros(x.shape[0])
        bt = 0
        while bt < x.shape[0]:
            btpp = bt + batch_size
            result[bt:btpp] = self(x[bt:btpp].cuda()).cpu().detach().numpy().flatten()
            bt = btpp

        return result
