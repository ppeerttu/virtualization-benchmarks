from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/boot/output")
output_dir = path.join(dirname, "extracted")
output_file = "boot_times.csv"

results = get_files(results_dir)

data = {
    'metal': [],
    'kvm': [],
    'firecracker': [],
    'docker': [],
    'gvisor': []
}

for f in results:
    selector = get_platform_selector(f)
    arr = data[selector]
    result_file = open(path.join(results_dir, f), 'r')

    def parse_line(line: str) -> None:
        m = re.search('real\t(\d+)m([0-9\.]+)s', line)
        if m:
            minutes = int(m.group(1))
            seconds = float(m.group(2))
            arr.append(seconds + (minutes * 60))
    read_file_per_line(path.join(results_dir, f), parse_line)
    assert len(arr) == 10, "Expected to collect 10 results but got " + len(arr)

def avg_time(values) -> float:
    count = len(values)
    return sum(values) / count

print("Average for Docker: {:.3f} seconds".format(avg_time(data['docker'])))
print("Average for gVisor: {:.3f} seconds".format(avg_time(data['gvisor'])))
print("Average for KVM: {:.3f} seconds".format(avg_time(data['kvm'])))
print("Average for metal: {:.3f} seconds".format(avg_time(data['metal'])))
print("Average for Firecracker: {:.3f} seconds".format(avg_time(data['firecracker'])))

df = pd.DataFrame(data=data)

df.to_csv(path.join(output_dir, output_file))
