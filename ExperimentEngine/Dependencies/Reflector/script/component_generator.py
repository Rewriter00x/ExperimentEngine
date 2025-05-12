import os
from data_types import *
from utils import *

def gen_main_file_data(include_list, init_list, array_init_list):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Components/ComponentUtils.h"

{include_list}
namespace Exp
{{{init_list}

    static std::vector<const ComponentWrapperBase*> s_AllComponents = {{ {array_init_list} }};

    const std::vector<const ComponentWrapperBase*>& GetAllComponents()
    {{
        return s_AllComponents;
    }}
}}"""

def gen_comp_base(comp_type):
    return f"""
#include "exppch.h"
#include "Engine/ECS/Components/ComponentUtils.h"

#include "imgui.h"
#include "Engine/ImGui/ExpImGui.h"
#include "Engine/ECS/Script/ScriptUtils.h"
#include "yaml-cpp/yaml.h"
#include "Engine/Serialization/Serializer.h"
#include "Engine/Serialization/ExpYaml.h"

#include "Engine/ECS/Components/{comp_type}.h"

namespace Exp
{{"""

def gen_prop_draw(prop):
    onEdit = prop.flags.get("onEdit", "")
    if len(onEdit) > 0:
        onEdit += "();"
        onEdit = "component." + onEdit

    if prop.type == "float":
        return f"""
        if (ImGui::DragFloat(\"{split_name(prop.name)}\", &component.{prop.name})) {{ {onEdit} }}"""
    elif prop.type == "glm::vec3":
        return f"""
        if (ImGui::DragFloat3(\"{split_name(prop.name)}\", glm::value_ptr(component.{prop.name}))) {{ {onEdit} }}"""
    elif prop.type == "glm::vec4":
        return f"""
        if (ImGui::ColorEdit4(\"{split_name(prop.name)}\", glm::value_ptr(component.{prop.name}))) {{ {onEdit} }}"""
    elif prop.type == "Shared<Texture>":
        return f"""
        {{
            const std::string fullPathString = (component.{prop.name} ? component.{prop.name}->GetFilepath() : \"\").generic_string();
            const std::string pathString = fullPathString.empty() ? "" : std::filesystem::relative(fullPathString, g_RootDirectory).generic_string();
            std::string path = pathString;
            ImGui::InputText(\"{split_name(prop.name)}\", path.data(), path.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
        
            if (ImGui::BeginDragDropTarget())
            {{
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ExpImGui::g_ImGuiContent_TextureFile))
                {{
                    const char* pathData = static_cast<const char*>(payload->Data);
                    std::filesystem::path acceptedPath(pathData);
                    path = std::filesystem::relative(acceptedPath, g_RootDirectory).generic_string();
                }}
                ImGui::EndDragDropTarget();
            }}
        
            if (ImGui::Button("Reset"))
            {{
                path = "";
            }}
        
            if (path != pathString)
            {{
                if (path.empty())
                {{
                    component.{prop.name} = nullptr;
                }}
                else
                {{
                    component.{prop.name} = AssetManager::GetTexture(g_RootDirectory / path);
                }}
                {onEdit}
            }}
        }}"""
    elif prop.type == "std::string":
        return f"""
        if (ImGui::InputText("{prop.name}", component.{prop.name}.data(), component.{prop.name}.capacity() + 1, ImGuiInputTextFlags_CallbackResize, ExpImGui::InputTextCallback, &component.{prop.name})) {{ {onEdit} }}"""
    elif prop.type == "ScriptInfo":
        return f"""
        {{
            const std::vector<std::string>& scriptNames = GetAllScriptNames();
            std::vector<const char*> scriptLiterals = GetAllScriptNamesLiterals();
            int32 selected = 0;
            auto it = std::find(scriptNames.begin(), scriptNames.end(), component.{prop.name}.Name);
            if (it != scriptNames.end())
            {{
                selected = (int32)std::distance(scriptNames.begin(), it);
            }}
            const int32 oldSelected = selected;
            ImGui::Combo("{prop.name}", &selected, scriptLiterals.data(), (int32)scriptLiterals.size());
            component.{prop.name}.Properties->Draw();
            if (oldSelected != selected)
            {{
                component.{prop.name}.Name = scriptNames[selected];
                component.{prop.name}.Properties = CreateScriptPropertiesByName(scriptNames[selected]);
                {onEdit}
            }}
        }}"""
    else:
        return ""

def gen_prop_save(prop):
    return f"""
        out << YAML::Key << \"{prop.name}\" << YAML::Value << component.{prop.name};"""

def gen_prop_load(prop):
    return f"""
        component.{prop.name} = node[\"{prop.name}\"].as<{prop.type}>();"""

def gen_prop_duplicate(prop):
    return f"""
        dstComp.{prop.name} = srcComp.{prop.name};"""

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
        {comp_type}& component = e.GetComponent<{comp_type}>();{draw_list}
    }}"""

def gen_comp_save(comp_type, save_list, load_list):
    return f"""
    template<>
    void SerializeComponent<{comp_type}>(YAML::Emitter& out, const Entity& e)
    {{
        using namespace Serializer;
        
        const {comp_type}& component = e.GetComponent<{comp_type}>();
        out << YAML::Key << GetComponentName<{comp_type}>();
        out << YAML::BeginMap;{save_list}
        out << YAML::EndMap;
    }}
    
    template<>
    void DeserializeComponent<{comp_type}>(const YAML::Node& node, Entity& e)
    {{
        {comp_type}& component = e.AddComponent<{comp_type}>();{load_list}
    }}"""

def gen_comp_duplicate(comp_type, duplicate_list):
    return f"""
    template<>
    void DuplicateComponent<{comp_type}>(Entity& dst, const Entity& src)
    {{
        const {comp_type}& srcComp = src.GetComponent<{comp_type}>();
        {comp_type}& dstComp = dst.AddComponent<{comp_type}>();
        {duplicate_list}
    }}"""

def gen_comp_file(comp):
    comp_file = gen_comp_base(comp.name)
    comp_file += gen_comp_name(comp.name, split_name(comp.name))

    draw_list = ""
    save_list = ""
    load_list = ""
    duplicate_list = ""
    for prop in comp.props:
        if "draw" in prop.flags:
            draw_list += gen_prop_draw(prop)

        if "save" in prop.flags:
            save_list += gen_prop_save(prop)
            load_list += gen_prop_load(prop)

        duplicate_list += gen_prop_duplicate(prop)

    comp_file += '\n' + gen_comp_draw(comp.name, draw_list)
    comp_file += '\n' + gen_comp_save(comp.name, save_list, load_list)
    comp_file += '\n' + gen_comp_duplicate(comp.name, duplicate_list)

    comp_file += '\n}'
    return comp_file

def gen_main_file(comp_list):
    include_list = ""
    init_list = ""
    array_init_list = ""
    for i, comp in enumerate(comp_list):
        include_list += f"#include \"Engine/ECS/Components/{comp.name}.h\"\n"
        init_list += f"""
    static const ComponentWrapper<{comp.name}> {lower_name(comp.name)};"""
        if (i != 0):
            array_init_list += ", "
        array_init_list += '&'+ lower_name(comp.name)

    return gen_main_file_data(include_list, init_list, array_init_list)

def gen_component_files(comp_list):
    if not os.path.exists("../src"):
        os.makedirs("../src")
    
    for comp in comp_list:
        comp_file = "// Generated file, DO NOT CHANGE\n" + gen_comp_file(comp)
        with open(f"../src/{comp.name}.gen.cpp", "w") as file:
            file.write(comp_file)

    main_file = "// Generated file, DO NOT CHANGE\n" + gen_main_file(comp_list)
    with open("../src/ComponentUtils.gen.cpp", "w") as file:
        file.write(main_file)