import os
from data_types import *
from utils import *

def gen_main_file_data(include_list, lambda_list, props_lambda_list, names_list):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Script/ScriptUtils.h"

#include "Engine/ECS/Script/NativeScript.h"
{include_list}

namespace Exp
{{
    static const std::unordered_map<std::string, NativeScript*(*)(Entity_ID, World*)> s_NameToCreateMap = {{
        {{ "None", [](Entity_ID entityID, World* world){{ return new NativeScript(entityID, world); }}}},{lambda_list}
    }};

    static const std::unordered_map<std::string, Unique<ScriptPropertiesBase>(*)()> s_NameToPropsMap = {{
        {{ "None", []{{ return MakeUnique<ScriptPropertiesBase>(); }} }},{props_lambda_list}
    }};

    static const std::vector<std::string> s_Names = {{
        "None",{names_list}
    }};

    static const std::vector<const char*> s_NamesLiterals = {{
        "None",{names_list}
    }};
    
    NativeScript* CreateScriptByName(const std::string& name, Entity_ID entityID, World* world)
    {{
        auto it = s_NameToCreateMap.find(name);
        if (it != s_NameToCreateMap.end())
        {{
            return it->second(entityID, world);
        }}
        EXP_LOG(Warning, "Attempt to create non-existing script %s", name.c_str());
        return s_NameToCreateMap.at("None")(entityID, world);
    }}

    Unique<ScriptPropertiesBase> CreateScriptPropertiesByName(const std::string& name)
    {{
        auto it = s_NameToPropsMap.find(name);
        if (it != s_NameToPropsMap.end())
        {{
            return it->second();
        }}
        EXP_LOG(Warning, "Attempt to create non-existing script properties %s", name.c_str());
        return s_NameToPropsMap.at("None")();
    }}

    const std::vector<std::string>& GetAllScriptNames()
    {{
        return s_Names;
    }}

    const std::vector<const char*>& GetAllScriptNamesLiterals()
    {{
        return s_NamesLiterals;
    }}
}}"""

def gen_script_header_data(script_type, props_list):
    return f"""
#pragma once

#include "Engine/ECS/Script/ScriptUtils.h"

namespace YAML
{{
    class Emitter;
    class Node;
}}

namespace Exp
{{
    struct {script_type}_Properties : public ScriptPropertiesBase
    {{{props_list}

        virtual void Draw() override;
        virtual void Serialize(YAML::Emitter& out) const override;
        virtual void Deserialize(const YAML::Node& node) override;

        virtual Unique<ScriptPropertiesBase> Duplicate() const override;
        
        virtual void InitScript(NativeScript* script) const override;
    }};
}}"""

def gen_script_base(script_type):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Script/ScriptUtils.h"

#include "imgui.h"
#include "Engine/ImGui/ExpImGui.h"
#include "yaml-cpp/yaml.h"
#include "Engine/Serialization/Serializer.h"
#include "Engine/Serialization/ExpYaml.h"

#include "Engine/ECS/Script/{script_type}.h"
#include "{script_type}.gen.h"

namespace Exp
{{"""

def gen_prop_draw(prop):
    if prop.type == "float":
        return f"""
        ImGui::DragFloat(\"{split_name(prop.name)}\", &{prop.name});"""
    elif prop.type == "glm::vec3":
        return f"""
        ImGui::DragFloat3(\"{split_name(prop.name)}\", glm::value_ptr({prop.name}));"""
    elif prop.type == "glm::vec4":
        return f"""
        ImGui::ColorEdit4(\"{split_name(prop.name)}\", glm::value_ptr({prop.name}));"""
    elif prop.type == "Shared<Texture>":
        return f"""
        ImGui::Button(\"{prop.name}\");"""
    elif prop.type == "std::string":
        return f"""
        ImGui::InputText("{prop.name}", {prop.name}.data(), {prop.name}.capacity() + 1, ImGuiInputTextFlags_CallbackResize, ExpImGui::InputTextCallback, &{prop.name});"""
    elif prop.type == "ScriptInfo":
        return f"""
        {{
            const std::vector<std::string>& scriptNames = GetAllScriptNames();
            std::vector<const char*> scriptLiterals = GetAllScriptNamesLiterals();
            int32 selected = 0;
            auto it = std::find(scriptNames.begin(), scriptNames.end(), {prop.name}.Name);
            if (it != scriptNames.end())
            {{
                selected = (int32)std::distance(scriptNames.begin(), it);
            }}
            const int32 oldSelected = selected;
            ImGui::Combo("{prop.name}", &selected, scriptLiterals.data(), (int32)scriptLiterals.size());
            {prop.name}.Properties->Draw();
            if (oldSelected != selected)
            {{
                {prop.name}.Name = scriptNames[selected];
                {prop.name}.Properties = CreateScriptPropertiesByName(scriptNames[selected]);
            }}
        }}"""
    else:
        return ""

def gen_prop_save(prop):
    return f"""
        out << YAML::Key << \"{prop.name}\" << YAML::Value << {prop.name};"""

def gen_prop_load(prop):
    return f"""
        {prop.name} = node[\"{prop.name}\"].as<{prop.type}>();"""

def gen_prop_duplicate(prop):
    return f"""
        Res->{prop.name} = {prop.name};"""

def gen_prop_init(prop):
    return f"""
        castedScript->{prop.name} = {prop.name};"""

def gen_prop_decl(prop):
    decl = f"""
        {prop.type} {prop.name}"""
    if prop.default_value is not None:
        decl += f" = {prop.default_value}"
    decl += ';'
    return decl

def gen_script_draw(script_type, draw_list):
    return f"""
    void {script_type}_Properties::Draw()
    {{
        ScriptPropertiesBase::Draw();
        {draw_list}
    }}"""

def gen_script_save(script_type, save_list, load_list):
    return f"""
    void {script_type}_Properties::Serialize(YAML::Emitter& out) const
    {{
        ScriptPropertiesBase::Serialize(out);
        
        out << YAML::BeginMap;{save_list}
        out << YAML::EndMap;
    }}
    
    void {script_type}_Properties::Deserialize(const YAML::Node& node)
    {{
        ScriptPropertiesBase::Deserialize(node);
        {load_list}
    }}"""

def gen_script_duplicate(script_type, duplicate_list):
    return f"""
    Unique<ScriptPropertiesBase> {script_type}_Properties::Duplicate() const
    {{
        Unique<TestScript_Properties> Res = MakeUnique<TestScript_Properties>();{duplicate_list}
        return Res;
    }}"""

def gen_script_init(script_type, init_list):
    return f"""
    void {script_type}_Properties::InitScript(NativeScript* script) const
    {{
        {script_type}* castedScript = ({script_type}*)script;
        {init_list}
    }}"""

def gen_script_header(script):
    props_list = ""

    for prop in script.props:
        props_list += gen_prop_decl(prop)

    return gen_script_header_data(script.name, props_list)

def gen_script_file(script):
    script_file = gen_script_base(script.name)

    draw_list = ""
    save_list = ""
    load_list = ""
    duplicate_list = ""
    init_list = ""
    for prop in script.props:
        if "draw" in prop.flags:
            draw_list += gen_prop_draw(prop)

        if "save" in prop.flags:
            save_list += gen_prop_save(prop)
            load_list += gen_prop_load(prop)

        duplicate_list += gen_prop_duplicate(prop)
        init_list += gen_prop_init(prop)

    script_file += gen_script_draw(script.name, draw_list)
    script_file += '\n' + gen_script_save(script.name, save_list, load_list)
    script_file += '\n' + gen_script_duplicate(script.name, duplicate_list)
    script_file += '\n' + gen_script_init(script.name, init_list)

    script_file += '\n}'
    return script_file

def gen_main_file(script_list):
    include_list = ""
    lambda_list = ""
    props_lambda_list = ""
    names_list = ""

    for script in script_list:
        include_list += f"""
#include \"Engine/ECS/Script/{script.name}.h\"
#include \"{script.name}.gen.h\""""
        lambda_list += f"""
        {{ "{split_name(script.name)}", [](Entity_ID entityID, World* world){{ return (NativeScript*) new {script.name}(entityID, world); }}}},"""
        props_lambda_list += f"""
        {{ "{split_name(script.name)}", []{{ return (Unique<ScriptPropertiesBase>) MakeUnique<{script.name}_Properties>(); }} }},"""
        names_list += f"""
        "{split_name(script.name)}","""

    return gen_main_file_data(include_list, lambda_list, props_lambda_list, names_list)

def gen_script_files(script_list):
    if not os.path.exists("../src"):
        os.makedirs("../src")

    for script in script_list:
        script_header = "// Generated file, DO NOT CHANGE\n" + gen_script_header(script)
        with open(f"../src/{script.name}.gen.h", "w") as file:
            file.write(script_header)
        
        script_file = "// Generated file, DO NOT CHANGE\n" + gen_script_file(script)
        with open(f"../src/{script.name}.gen.cpp", "w") as file:
            file.write(script_file)

    main_file = "// Generated file, DO NOT CHANGE\n" + gen_main_file(script_list)
    with open("../src/ScriptUtils.gen.cpp", "w") as file:
        file.write(main_file)