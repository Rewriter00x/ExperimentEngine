from collections import namedtuple

Property = namedtuple("Property", ["name", "type", "flags"])
Component = namedtuple("Component", ["name", "props"])
Script = namedtuple("Script", ["name"])