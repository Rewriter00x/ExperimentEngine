from data_types import *

def parse_comp(line):
    line = line.strip()

    parts = line.split()

    return Component(name=parts[1], props=[])

def parse_script(line):
    line = line.strip()

    parts = line.split()

    return Script(name=parts[1])

def parse_prop_def(line):
    line = line.strip()

    p_pos = line.find("//p")
    line = line[p_pos + 4:].strip()

    parts = line.split(',')

    flags = []
    for part in parts:
        flags.append(part.strip())

    return flags

def parse_prop(line):
    line = line.strip()

    eq_pos = line.find('=')

    if eq_pos != -1:
        line = line[:eq_pos].strip()

    parts = line.split()

    name = parts[1]
    if (name[-1] == ';'):
        name = name[:-1]

    if len(parts) >= 2:
        return Property(name=name, type=parts[0], flags=[])

    return None

def parse_component_file(file):
    components = []
    component = None
    try:
        with open(file) as file:
            iterator = iter(file)
            line = next(iterator, None)

            while line is not None:
                if "//c" in line:
                    if component is not None:
                        components.append(component)
                    line = next(iterator, None)
                    component = parse_comp(line)
                elif "//p" in line:
                    flags = parse_prop_def(line)
                    line = next(iterator, None)
                    prop = parse_prop(line)
                    prop = prop._replace(flags=flags)
                    props = component.props + [prop]
                    component = component._replace(props=props)
                line = next(iterator, None)
    except Exception as e:
        print(e) 

    if component is not None:
        components.append(component)
        
    return components

def parse_script_file(file):
    scripts = []
    try:
        with open(file) as file:
            iterator = iter(file)
            line = next(iterator, None)

            while line is not None:
                if "//sc" in line:
                    line = next(iterator, None)
                    scripts.append(parse_script(line))
                line = next(iterator, None)

    except Exception as e:
        print(e)
        
    return scripts