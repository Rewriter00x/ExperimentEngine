#include "exppch.h"
#include "Engine/Utils/FileDialogs.h"

#include <windows.h>
#include <commdlg.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#undef GLFW_EXPOSE_NATIVE_WIN32

namespace Exp::FileDialogs
{
    static constexpr int bufSize = 256;
    
    std::filesystem::path OpenFile(const DialogData& data)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[bufSize] = {};
        if (!data.DefaultName.empty())
        {
            strncpy(szFile, data.DefaultName.c_str(), bufSize - 1);
        }
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        std::string defPath; 
        if (!data.DefaultPath.empty())
        {
            defPath = data.DefaultPath.string();
            ofn.lpstrInitialDir = defPath.c_str();
        }
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string{};
    }

    std::filesystem::path SaveFile(const DialogData& data)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[bufSize] = {};
        if (!data.DefaultName.empty())
        {
            strncpy(szFile, data.DefaultName.c_str(), bufSize - 1);
        }
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        std::string defPath;
        if (!data.DefaultPath.empty())
        {
            defPath = data.DefaultPath.string();
            ofn.lpstrInitialDir = defPath.c_str();
        }
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string{};
    }
}
