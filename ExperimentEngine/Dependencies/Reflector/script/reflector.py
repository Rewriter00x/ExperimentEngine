import sys
from pathlib import Path

from data_types import *
from parser import *
from generator import *

def main():
    directory = Path("../../../src/Engine/ECS/Components")
    components = []
    for file in directory.glob("*.h"):
        components += parse_file(file)

    gen_files(components)

if __name__ == "__main__":
    main()
    