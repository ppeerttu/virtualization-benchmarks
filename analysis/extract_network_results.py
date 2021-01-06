from src.utils import get_files, get_platform_selector, read_json_file
from os import path
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/network/output")
output_dir = path.join(dirname, "extracted")
bandwidth_file = "bandwidth_results.csv"
jitter_file = "jitter_results.csv"

results = [f for f in get_files(results_dir) if 'server' not in f]
b_results = [f for f in results if 'jitter' not in f]
j_results = [f for f in results if 'jitter' in f]

df = pd.DataFrame(columns=['platform', 'sent_bps', 'received_bps', 'cpu_client', 'cpu_server'])

for offset, f in enumerate(b_results):
    selector = get_platform_selector(f)
    iterations = read_json_file(path.join(results_dir, f))
    for i, iteration in enumerate(iterations):
        end = iteration['end']
        sent_bps = end['sum_sent']['bits_per_second']
        received_bps = end['sum_received']['bits_per_second']
        cpu_local = end['cpu_utilization_percent']['host_total']
        cpu_remote = end['cpu_utilization_percent']['remote_total']
        index = offset * 10 + i
        # Store from the platform perspecvive, hence column "sent_bps" means bits sent by the server, not client
        df.loc[index] = [selector, received_bps, sent_bps, cpu_local, cpu_remote]

df.to_csv(path.join(output_dir, bandwidth_file))

df = pd.DataFrame(columns=['platform', 'jitter_ms', 'bps', 'lost_percent', 'cpu_client', 'cpu_server'])

for offset, f in enumerate(j_results):
    selector = get_platform_selector(f)
    iterations = read_json_file(path.join(results_dir, f))
    for i, iteration in enumerate(iterations):
        end = iteration['end']
        jitter_ms = end['sum']['jitter_ms']
        bps = end['sum']['bits_per_second']
        lost_percent = end['sum']['lost_percent']
        cpu_local = end['cpu_utilization_percent']['host_total']
        cpu_remote = end['cpu_utilization_percent']['remote_total']
        index = offset * 10 + i
        # Store from the platform perspecvive, hence column "sent_bps" means bits sent by the server, not client
        df.loc[index] = [selector, jitter_ms, bps, lost_percent, cpu_local, cpu_remote]

df.to_csv(path.join(output_dir, jitter_file))
