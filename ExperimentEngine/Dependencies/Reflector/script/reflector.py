import sys
from pathlib import Path

from data_types import *
from parser import *
from component_generator import *
from script_generator import *

def main():
    directory = Path("../../../src/Engine/ECS/Components")
    components = []
    for file in directory.glob("*.h"):
        components += parse_component_file(file)

    directory = Path("../../../src/Engine/ECS/Script")
    scripts = []
    for file in directory.glob("*.h"):
        scripts += parse_script_file(file)

    gen_component_files(components)
    gen_script_files(scripts)

if __name__ == "__main__":
    main()
    