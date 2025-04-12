import os
from data_types import *

def split_name(name):
    res = ""
    for i, char in enumerate(name):
        if char.isupper() and i != 0:
            res += ' '
        res += char

    return res

def lower_name(name):
    return name[0].lower() + name[1:]

def gen_main_file_data(include_list, init_list, array_init_list):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Components/ComponentUtils.h"

{include_list}
namespace Exp
{{
    {init_list}
    static std::vector<ComponentWrapperBase*> s_AllComponents = {{ {array_init_list} }};

    const std::vector<ComponentWrapperBase*>& GetAllComponents()
    {{
        return s_AllComponents;
    }}
}}"""

def gen_comp_base(comp_type):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Components/ComponentUtils.h"

#include "imgui.h"
#include "yaml-cpp/yaml.h"
#include "Engine/Serialization/Serializer.h"

#include "Engine/ECS/Components/{comp_type}.h"

namespace Exp
{{"""

def gen_prop_draw(prop):
    match prop.type:
        case "float":
            return f"""
        ImGui::DragFloat(\"{split_name(prop.name)}\", &component.{prop.name});"""
        case "glm::vec3":
            return f"""
        ImGui::DragFloat3(\"{split_name(prop.name)}\", glm::value_ptr(component.{prop.name}));"""
        case "glm::vec4":
            return f"""
        ImGui::ColorEdit4(\"{split_name(prop.name)}\", glm::value_ptr(component.{prop.name}));"""

def gen_prop_save(prop):
    return f"out << YAML::Key << \"{prop.name}\" << YAML::Value << component.{prop.name};"

def gen_prop_load(prop):
    return f"component.{prop.name} = node[\"{prop.name}\"].as<{prop.type}>();"

def gen_comp_name(comp_type, name):
    return f"""
    template<>
    const char* GetComponentName<{comp_type}>()
    {{
        return "{name}";
    }}"""

def gen_comp_draw(comp_type, draw_list):
    return f"""
    template<>
    void DrawComponent<{comp_type}>(Entity& e)
    {{
        {comp_type}& component = e.GetComponent<{comp_type}>();
        {draw_list}
    }}"""

def gen_comp_save(comp_type, save_list, load_list):
    return f"""
    template<>
    void SerializeComponent<{comp_type}>(YAML::Emitter& out, const Entity& e)
    {{
        using namespace Serializer;
        
        const {comp_type}& component = e.GetComponent<{comp_type}>();
        out << YAML::Key << GetComponentName<{comp_type}>();
        {save_list}
        out << YAML::EndMap;
    }}
    
    template<>
    void DeserializeComponent<{comp_type}>(const YAML::Node& node, Entity& e)
    {{
        {comp_type}& component = e.AddComponent<{comp_type}>();
        {load_list}
    }}"""

def gen_comp_file(comp):
    comp_file = gen_comp_base(comp.name)
    comp_file += gen_comp_name(comp.name, split_name(comp.name))

    draw_list = ""
    save_list = ""
    load_list = ""
    for prop in comp.props:
        if "draw" in prop.flags:
            draw_list += gen_prop_draw(prop)

        if "save" in prop.flags:
            save_list += gen_prop_save(prop)
            load_list += gen_prop_load(prop)

    comp_file += '\n' + gen_comp_draw(comp.name, draw_list)
    comp_file += '\n' + gen_comp_save(comp.name, save_list, load_list)

    comp_file += '\n}'
    return comp_file

def gen_main_file(comp_list):
    include_list = ""
    init_list = ""
    array_init_list = ""
    for i, comp in enumerate(comp_list):
        include_list += f"#include \"Engine/ECS/Components/{comp.name}.h\"\n"
        init_list += f"static ComponentWrapper<{comp.name}> {lower_name(comp.name)};\n"
        if (i != 0):
            array_init_list += ", "
        array_init_list += '&'+ lower_name(comp.name)

    return gen_main_file_data(include_list, init_list, array_init_list)

def gen_files(comp_list):
    #os.makedirs("../src")
    for comp in comp_list:
        comp_file = "// Generated file, DO NOT CHANGE\n" + gen_comp_file(comp)
        with open(f"../src/{comp.name}.gen.cpp", "w") as file:
            file.write(comp_file)

    main_file = "// Generated file, DO NOT CHANGE\n" + gen_main_file(comp_list)
    with open("../src/ComponentUtils.gen.cpp", "w") as file:
        file.write(main_file)

