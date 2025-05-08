from collections import namedtuple

Property = namedtuple("Property", ["name", "type", "default_value", "flags"])
Component = namedtuple("Component", ["name", "props"])
Script = namedtuple("Script", ["name", "props"])