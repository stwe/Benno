#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include "BshFile.h"
#include "renderer/Zoom.h"

namespace sg::file
{
    struct BennoFile
    {
        std::string fullFilename;
        std::string path;

        BennoFile() = default;
        virtual ~BennoFile() = default;
    };

    struct BennoZoomableBshFile : BennoFile
    {
        renderer::Zoom::ZoomId zoomId{ renderer::Zoom::ZoomId::NOT_ZOOMABLE };
        BshFile::BshFileNameId bshFileNameId{ BshFile::BshFileNameId::INVALID };
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

        [[nodiscard]] std::optional<BennoZoomableBshFile> GetBshFile(renderer::Zoom::ZoomId t_zoomId, BshFile::BshFileNameId t_bshFileNameId) const;
        [[nodiscard]] const BennoFile& GetColFile() const;

    protected:

    private:
        std::unordered_multimap<renderer::Zoom::ZoomId, BennoZoomableBshFile> m_bshFiles;
        BennoFile m_colFile;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitDirectoryTree(const std::string& t_path);

        //-------------------------------------------------
        // Check for files
        //-------------------------------------------------

        void CheckForBshFiles();
        bool CheckForBshFile(renderer::Zoom::ZoomId t_zoomId, BshFile::BshFileNameId t_bshFileNameId);
        void CheckForColFile() const;
        std::optional<std::string> GetBshFilePath(renderer::Zoom::ZoomId t_zoomId, BshFile::BshFileNameId t_bshFileNameId);

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        [[nodiscard]] std::string ToLowerCase(const std::string& t_string) const;
        [[nodiscard]] std::string ToUpperCase(const std::string& t_string) const;
    };
}
