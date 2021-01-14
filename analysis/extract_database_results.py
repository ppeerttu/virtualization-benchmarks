from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/database/output")
output_dir = path.join(dirname, "extracted")
output_file = "database_results.csv"

results = get_files(results_dir)

df = pd.DataFrame(columns=['platform', 'concurrency', 'read_queries', 'write_queries', 'other_queries', 'total_queries', 'transactions', 'latency_min', 'latency_max', 'latency_avg', 'time_total'])

read_queries = 0
write_queries = 0
other_queries = 0
total_queries = 0
transactions = 0
latency_min = 0
latency_max = 0
latency_avg = 0
time_total = 0
concurrency = 0

def clear_row():
    global stats_started, read_queries, write_queries, other_queries, total_queries, transactions, latency_min, latency_max, latency_avg, time_total, concurrency
    read_queries = 0
    write_queries = 0
    other_queries = 0
    total_queries = 0
    transactions = 0
    latency_min = 0
    latency_max = 0
    latency_avg = 0
    time_total = 0
    concurrency = 0

stats_started = False
selector = None
i = 0

def parse_line(line: str) -> None:
    global selector, stats_started, read_queries, write_queries, other_queries, total_queries, transactions, latency_min, latency_max, latency_avg, time_total, i, df, concurrency
    if not stats_started:
        if line.startswith("Running the test with following options:"):
            stats_started = True
    elif line.startswith("Threads fairness:"):
        df.loc[i] = [selector, concurrency, read_queries, write_queries, other_queries, total_queries, transactions, latency_min, latency_max, latency_avg, time_total]
        stats_started = False
        clear_row()
        i += 1
    else:
        m = re.search('{}: +([0-9\.]+)'.format("Number of threads"), line)
        if m:
            concurrency = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("read"), line)
        if m:
            read_queries = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("write"), line)
        if m:
            write_queries = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("other"), line)
        if m:
            other_queries = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("total"), line)
        if m:
            total_queries = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("transactions"), line)
        if m:
            transactions = int(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("total time"), line)
        if m:
            time_total = float(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("min"), line)
        if m:
            latency_min = float(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("max"), line)
        if m:
            latency_max = float(m.group(1))
        m = re.search(' +{}: +([0-9\.]+)'.format("avg"), line)
        if m:
            latency_avg = float(m.group(1))


for f in results:
    selector = get_platform_selector(f)
    print("Platform: {}".format(selector))
    read_file_per_line(path.join(results_dir, f), parse_line)

df.to_csv(path.join(output_dir, output_file), index=False)
