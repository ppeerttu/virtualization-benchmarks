from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/cpu/output")
output_dir = path.join(dirname, "extracted")
output_file = "cpu_results.csv"

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

    def parse_line(line: str) -> None:
        m = re.search(' +events per second: +([0-9\.]+)', line)
        if m:
            events = float(m.group(1))
            arr.append(events)

    read_file_per_line(path.join(results_dir, f), parse_line)
    assert len(arr) == 10, "Expected to collect 10 results but got " + len(arr)

df = pd.DataFrame(data=data)

print("Averages:")
print(df.mean(axis=0))

df.to_csv(path.join(output_dir, output_file), index=False)
