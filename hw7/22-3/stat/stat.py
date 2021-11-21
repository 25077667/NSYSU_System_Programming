#!/usr/bin/python3
from scipy import stats
import statistics


def read_data(filename: str) -> list:
    l = list()
    with open(filename) as f:
        for line in f:
            l.append(float(line.lstrip()))
    return l


def main() -> None:
    sigsuspend = read_data("../sigsuspend")
    sigwaitinfo = read_data("../sigwaitinfo")
    mean = (statistics.mean(sigsuspend), statistics.mean(sigwaitinfo))
    print("sigsuspend mean:{}\nsigwaitinfo mean:{}".format(mean[0], mean[1]))
    print(stats.ttest_ind(sigsuspend, sigwaitinfo))


if __name__ == "__main__":
    main()
