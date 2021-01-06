from typing import Dict, List
from src.utils import get_files, get_platform_selector, read_file_per_line
from os import path
import re
import pandas as pd

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/fileio/output")
output_dir = path.join(dirname, "extracted")
read_output_file = "fileio_read_results.csv"
write_output_file = "fileio_write_results.csv"

results = get_files(results_dir)

data = {
    'metal': [],
    'kvm': [],
    'firecracker': [],
    'docker': [],
    'gvisor': []
}

def parse_results(files: List[str], type: str) -> Dict[str, List[float]]:
    data = {
        'metal': [],
        'kvm': [],
        'firecracker': [],
        'docker': [],
        'gvisor': []
    }

    for f in files:
        selector = get_platform_selector(f)
        arr = data[selector]

        def parse_line(line: str) -> None:
            m = re.search(' +{}, MiB/s: +([0-9\.]+)'.format(type), line)
            if m:
                mbytes = float(m.group(1))
                if mbytes > 0:
                    arr.append(mbytes)

        read_file_per_line(path.join(results_dir, f), parse_line)
        assert len(arr) == 10, "Expected to collect 10 results from {} but got {}".format(f, len(arr))
    return data

d_read = parse_results(results, "read")
df = pd.DataFrame(data=d_read)

print(df.mean())

df.to_csv(path.join(output_dir, read_output_file))

d_write = parse_results(results, "written")
df = pd.DataFrame(data=d_write)

print(df.mean())

df.to_csv(path.join(output_dir, write_output_file))

# df = pd.DataFrame(data=data)

# print("Averages:")
# print(df.mean(axis=0))

# df.to_csv(path.join(output_dir, output_file))
