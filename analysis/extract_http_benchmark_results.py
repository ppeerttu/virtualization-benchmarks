from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/case-app/http-benchmark/output")
output_dir = path.join(dirname, "extracted")
output_file = "http_benchmark_results.csv"

results = get_files(results_dir)

df = pd.DataFrame(columns=['platform', 'concurrency', 'duration', 'completed_req', 'failed_req', 'bytes_transferred', 'req_per_sec', 'mean_time_per_req'])

concurrency = 0
duration = 0
completed_req = 0
failed_req = 0
bytes_transferred = 0
req_per_sec = 0
mean_time_per_req = 0

def clear_row():
    global concurrency, duration, completed_req, failed_req, bytes_transferred, req_per_sec, mean_time_per_req
    concurrency = 0
    duration = 0
    completed_req = 0
    failed_req = 0
    bytes_transferred = 0
    req_per_sec = 0
    mean_time_per_req = 0


stats_started = False
selector = None
i = 0

def parse_line(line: str) -> None:
    global selector, stats_started, concurrency, duration, completed_req, failed_req, bytes_transferred, req_per_sec, mean_time_per_req, i, df
    if not stats_started:
        if line.startswith("Server Software:"):
            stats_started = True
    elif line.startswith("Percentage of the requests"):
        df.loc[i] = [selector, concurrency, duration, completed_req, failed_req, bytes_transferred, req_per_sec, mean_time_per_req]
        stats_started = False
        clear_row()
        i += 1
    else:
        m = re.search('{}: +([0-9\.]+)'.format("Concurrency Level"), line)
        if m:
            concurrency = int(m.group(1))
        m = re.search('{}: +([0-9\.]+)'.format("Time taken for tests"), line)
        if m:
            duration = float(m.group(1))
        m = re.search('{}: +([0-9\.]+)'.format("Complete requests"), line)
        if m:
            completed_req = int(m.group(1))
        m = re.search('{}: +([0-9\.]+)'.format("Failed requests"), line)
        if m:
            failed_req = int(m.group(1))
        m = re.search('{}: +([0-9\.]+)'.format("Total transferred"), line)
        if m:
            bytes_transferred = int(m.group(1))
        m = re.search('{}: +([0-9\.]+)'.format("Requests per second"), line)
        if m:
            req_per_sec = float(m.group(1))
        m = re.search('{}: +([0-9\.]+) \[ms\] \(mean\)'.format("Time per request"), line)
        if m:
            mean_time_per_req = float(m.group(1))

for f in results:
    selector = get_platform_selector(f)
    print("Platform: {}".format(selector))
    read_file_per_line(path.join(results_dir, f), parse_line)

df.to_csv(path.join(output_dir, output_file), index=False)
