#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include "renderer/Zoom.h"

namespace sg::file
{
    struct BennoFile
    {
        std::string fullFilename;
        std::string stem;
        std::string path;
    };

    class Files
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Files() = delete;

        explicit Files(const std::string& t_path);

        Files(const Files& t_other) = delete;
        Files(Files&& t_other) noexcept = delete;
        Files& operator=(const Files& t_other) = delete;
        Files& operator=(Files&& t_other) noexcept = delete;

        ~Files();

        //-------------------------------------------------
        // Getter read-only
        //-------------------------------------------------

        [[nodiscard]] std::optional<BennoFile> GetFile(renderer::Zoom t_zoom, const std::string& t_stem) const;

    protected:

    private:
        std::unordered_multimap<renderer::Zoom, BennoFile> m_bshFiles;
        std::vector<std::string> m_filesToCheck{ "STADTFLD", "EFFEKTE", "FISCHE" };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitDirectoryTree(const std::string& t_path);

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        bool CheckFile(renderer::Zoom t_zoom, const std::string& t_stem);
        std::optional<std::string> GetPath(renderer::Zoom t_zoom, const std::string& t_stem);

        [[nodiscard]] std::string ToLowerCase(const std::string& t_string) const;
        [[nodiscard]] std::string ToUpperCase(const std::string& t_string) const;
    };
}
