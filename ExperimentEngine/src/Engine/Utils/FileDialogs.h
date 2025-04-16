#pragma once

namespace Exp
{
    namespace FileDialogs
    {
        struct DialogData
        {
            std::string DefaultName;
            std::filesystem::path DefaultPath;
        };
        
        std::filesystem::path OpenFile(const DialogData& data = DialogData());
        std::filesystem::path SaveFile(const DialogData& data = DialogData());
    }
}
