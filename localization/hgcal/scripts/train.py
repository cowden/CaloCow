

import numpy as np
import torch

from hgcal import Erecon, Standardizer
import hgcalutils as hgu

data_dir = "/data/me/calox/dkr/tmp/"
y, X = hgu.h5handler.collect_and_merge(hgu.h5handler.list_data_files(data_dir))

esum = np.sum(X, axis=(1,2,3))

std = Standardizer(y, X)
ys, Xs = std.transform(y, X)

Xmean = X.mean(axis=0)
Xstd = X.std(axis=0)
X = (X - Xmean)/Xstd
X[np.isnan(X)] = 0.

mu = np.mean(y)
sigma = np.std(y)

#yt = torch.Tensor((y - mu)/sigma)
#Xt = torch.Tensor(np.expand_dims(X,1))
yt = torch.Tensor(ys)
Xt = torch.Tensor(Xs)

mod = Erecon()
mod.to("cuda")
mod.fit(yt, Xt)

#enn = mod.predict(Xt)*sigma + mu
enn = mod.predict(Xt)*std.y_std + std.y_mu

