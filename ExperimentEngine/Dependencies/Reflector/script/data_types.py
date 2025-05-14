from collections import namedtuple

Property = namedtuple("Property", ["name", "type", "default_value", "flags"])
Component = namedtuple("Component", ["name", "props", "flags"])
Script = namedtuple("Script", ["name", "props", "flags"])