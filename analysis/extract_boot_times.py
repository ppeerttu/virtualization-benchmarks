from os import listdir, path
import re

dirname = path.dirname(__file__)
results_dir = path.join(dirname, "../tests/boot/output")
extracted_dir = path.join(dirname, "extracted")
output_file = "boot_times.csv"
skip_files = [".gitignore"]

results = [f for f in listdir(results_dir) if path.isfile(path.join(results_dir, f)) and f not in skip_files]

data = {
    'metal': [],
    'kvm': [],
    'firecracker': [],
    'docker': [],
    'gvisor': []
}

for f in results:
    selector = None
    if 'metal' in f:
        selector = 'metal'
    elif 'vm' in f:
        selector = 'kvm'
    elif 'gvisor' in f:
        selector = 'gvisor'
    elif 'docker' in f:
        selector = 'docker'
    elif 'firecracker' in f:
        selector: 'firecracker'
    
    arr = data[selector]
    result_file = open(path.join(results_dir, f), 'r')
    line = result_file.readline()
    while line:
        m = re.search('real\t(\d+)m([0-9\.]+)s', line)
        if m:
            minutes = int(m.group(1))
            seconds = float(m.group(2))
            arr.append(seconds + (minutes * 60))
        line = result_file.readline()
    result_file.close()
    assert len(arr) == 10, "Expected to collect 10 results but got " + len(arr)

def avg_time(values) -> float:
    count = len(values)
    return sum(values) / count

print("Average for Docker: {:.3f} seconds".format(avg_time(data['docker'])))
print("Average for gVisor: {:.3f} seconds".format(avg_time(data['gvisor'])))
print("Average for KVM: {:.3f} seconds".format(avg_time(data['kvm'])))
print("Average for metal: {:.3f} seconds".format(avg_time(data['metal'])))
