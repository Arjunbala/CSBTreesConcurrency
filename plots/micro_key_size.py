#!/usr/bin/python
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from pylab import *
import matplotlib
import matplotlib.ticker as ticker
import os

def generate_plot(data, plot_path):
    (stats_efficient) = data
    fig, ax = plt.subplots(figsize=(8,6))
    stats_efficient = [x/1000 for x in stats_efficient]
    x_ticks = [2,4,8,16,64]
    xlabel = "Key Size (bytes)"
    ylabel = "Insert Throughput (Kops/sec)"
    legend = ["Efficient Locking"]
    plt.plot(x_ticks, stats_efficient, 'bo-', label = "Efficient Locking")
    plt.ylabel(ylabel, fontsize=22)
    plt.xlabel(xlabel, fontsize=22)
    plt.xticks([2, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64], fontsize=18)
    plt.yticks(fontsize=18)
    plt.ylim(top=250, bottom=0)
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
    return ([223146, 219304, 218868, 209041, 208545])

def bar_plot():
    plots_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "plots")
    plot_path = os.path.join(plots_dir, "insert_key_size.pdf")
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
