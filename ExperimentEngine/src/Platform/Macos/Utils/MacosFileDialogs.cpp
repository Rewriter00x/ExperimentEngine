#include "exppch.h"
#include "Engine/Utils/FileDialogs.h"

namespace Exp::FileDialogs
{
    std::filesystem::path OpenFile(const DialogData& data)
    {
        std::string script = R"(
        osascript -e '
        try
            set theFile to choose file default location ")" + data.DefaultPath.string() + R"("
            POSIX path of theFile
        on error
            return ""
        end try
        '
        )";

        FILE* pipe = popen(script.c_str(), "r");
        if (!pipe) return "";

        char buffer[512];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
        }
        int status = pclose(pipe);
        if (status)
        {
            return "";
        }

        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    }
    std::filesystem::path SaveFile(const DialogData& data)
    {
        const std::string script = R"(
        osascript -e '
        try
            set theFile to choose file name default name ")" + data.DefaultName + R"(" default location ")" + data.DefaultPath.string() + R"("
            POSIX path of theFile
        on error
            return ""
        end try
        '
        )";

        FILE* pipe = popen(script.c_str(), "r");
        if (!pipe) return "";

        char buffer[512];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
        }
        int status = pclose(pipe);
        if (status)
        {
            return "";
        }

        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    }
}
