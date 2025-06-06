"""Train a single epoch on simulated data."""
import os
import sys
import argparse

import torch

import hgcal
import hgcalutils as hgu

parser = argparse.ArgumentParser(
            prog="train-epoch.py",
            description="Train a single epoch on a simulation sample.",
            )
parser.add_argument("check_point_path")
parser.add_argument("data_path")
parser.add_argument("-b", "--batch-size")
parser.add_argument("-s", "--standardize", action="store_true")
parser.add_argument("-l", "--load-standardizer")


if __name__ == "__main__":

    args = parser.parse_args()

    # load model
    mod = hgcal.load_checkpoint(args.check_point_path)

    # load data
    y, X = hgu.h5handler.collect_and_merge(
            hgu.h5handler.list_data_files(
                args.data_path
                )
            )
    # data treatment (standardize)
    if not args.standardize:
        std = hgcal.load_standardizer(args.load_standardizer)
    else:
        std = hgcal.Standardizer(y, X)
        hgcal.save_standardizer(std, args.load_standardizer)

    ys, Xs = std.transform(y, X)
    yt = torch.Tensor(ys)
    Xt = torch.Tensor(Xs)

    # train
    mod.to("cuda")
    mod.fit(yt, Xt)

    # save model
    hgcal.save_checkpoint(mod, args.check_point_path)

