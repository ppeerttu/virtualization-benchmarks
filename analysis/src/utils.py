from os import listdir, path
from typing import Callable, List

def get_files(results_dir: str, skip_files: List[str] = [".gitignore"]) -> List[str]:
    """Get result files from given `results_dir`.

    Args:

        results_dir (str): Results directory
        skip_files (List[str], optional): Filenames to skip. Defaults to [".gitignore"].

    Returns:

        List[str]: List of filenames without absolute path
    """
    return [f for f in listdir(results_dir) if path.isfile(path.join(results_dir, f)) and f not in skip_files]

def get_platform_selector(filename: str) -> str:
    """Detect the platform from the filename and return respective selector.

    Args:

        filename (str): Results filename

    Returns:

        str: Selector
    """
    selector = None
    if 'metal' in filename:
        selector = 'metal'
    elif 'vm' in filename:
        selector = 'kvm'
    elif 'gvisor' in filename:
        selector = 'gvisor'
    elif 'docker' in filename:
        selector = 'docker'
    elif 'firecracker' in filename:
        selector = 'firecracker'
    return selector

def read_file_per_line(filepath: str, callback: Callable[[str], None]) -> None:
    """Read file line by line, and pass those line to the callback.

    Args:

        filepath (str): Path to the file
        callback (Callable[[str], None]): The callback function that accepts a single line as an argument

    """
    result_file = open(filepath, 'r')
    line = result_file.readline()
    while line:
        callback(line)
        line = result_file.readline()
    result_file.close()
