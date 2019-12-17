#!/usr/bin/python
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from pylab import *
import matplotlib
import matplotlib.ticker as ticker
import os

def generate_plot(data, plot_path):
    (stats_naive, stats_efficient) = data
    fig, ax = plt.subplots(figsize=(8,6))
    stats_naive = [x/1000 for x in stats_naive]
    stats_efficient = [x/1000 for x in stats_efficient]
    x_ticks = [1,2,4,6,8,10,12,16,20]
    xlabel = "# Reader Threads"
    ylabel = "Search Throughput (Kops/sec)"
    legend = ["Naive Locking", "Efficient Locking"]
    plt.plot(x_ticks, stats_naive, 'rx-', label = "Naive Locking")
    plt.plot(x_ticks, stats_efficient, 'bo-', label = "Efficient Locking")
    plt.ylabel(ylabel, fontsize=22)
    plt.xlabel(xlabel, fontsize=22)
    plt.xticks([2,4,6,8,10,12,14,16,18,20], fontsize=18)
    plt.yticks(fontsize=18)
    plt.ylim(top=1600, bottom=0)
    plt.legend(legend, fontsize =16, ncol=1)
    plt.tight_layout()
    pdf_page = PdfPages(plot_path)
    pdf_page.savefig()
    pdf_page.close()

def read_stats(data_path):
    with open(data_path) as f:
        content = f.readlines()
    content = content[1:]
    data = [float(x.strip().split(",")[0].strip()) for x in content]
    stats = [np.mean(data), np.percentile(data, 50), np.percentile(data, 99), np.percentile(data, 99.5), np.percentile(data, 99.9)]
    return stats

def read_data():
    return ([852672, 846473, 891589, 848680, 826952, 831918, 819799, 833002, 811540], [826446, 922901, 1085429, 1217461, 1306188, 1308971, 1487749, 1473625, 1511845])

def bar_plot():
    plots_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "plots")
    plot_path = os.path.join(plots_dir, "search_throughput.pdf")
    data = read_data()
    generate_plot(data, plot_path)

def main():
    matplotlib.rcParams['ps.useafm'] = True
    matplotlib.rcParams['pdf.use14corefonts'] = True
    #matplotlib.rcParams['text.usetex'] = True
    matplotlib.rcParams.update({'figure.autolayout':True})
    matplotlib.rcParams.update({'font.size': 11})
    bar_plot()

if __name__ == "__main__":
    main()
