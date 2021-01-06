from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path, stat_result
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/database/output")
output_dir = path.join(dirname, "extracted")
output_file = "cpu_results.csv"

results = get_files(results_dir)

df = pd.DataFrame(columns=['platform', 'rqps', 'wqps', 'oqps', 'tqps', 'transactions', 'latency_min', 'latency_max', 'latency_avg', 'time_total'])

rqps = 0
wqps = 0
oqps = 0
tqps = 0
transactions = 0
latency_min = 0
latency_max = 0
latency_avg = 0

def clear_row():
    rqps = 0
    wqps = 0
    oqps = 0
    tqps = 0
    transactions = 0
    latency_min = 0
    latency_max = 0
    latency_avg = 0

stats_started = False
selector = None

def parse_line(line: str) -> None:
    global stats_started
    if not stats_started:
        if line.startswith("SQL statistics:"):
            stats_started = True
    elif line.startswith("sysbench"):
        stats_started = False
        clear_row()
    else:
        m = re.search(' +{}: +([0-9\.]+)'.format("read"), line)
        if m:
            rqps = float(m.group(1))
            print(rqps)

for f in results:
    selector = get_platform_selector(f)

    read_file_per_line(path.join(results_dir, f), parse_line)
    # assert len(arr) == 10, "Expected to collect 10 results but got " + len(arr)

# df = pd.DataFrame(data=data)

# print("Averages:")
# print(df.mean(axis=0))

# df.to_csv(path.join(output_dir, output_file))
