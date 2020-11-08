#include <fstream>
#include <filesystem>
#include "Files.h"
#include "Log.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::Files::Files(const std::string& t_path)
{
    Log::SG_LOG_DEBUG("[Files::Files()] Create Files.");

    InitDirectoryTree(t_path);
}

sg::file::Files::~Files()
{
    Log::SG_LOG_DEBUG("[Files::~Files()] Destruct Files.");
}

//-------------------------------------------------
// Getter read-only
//-------------------------------------------------

std::optional<sg::file::BennoZoomableBshFile> sg::file::Files::GetBshFile(const renderer::Zoom::ZoomId t_zoomId, const BshFile::BshFileNameId t_bshFileNameId) const
{
    const auto its{ m_bshFiles.equal_range(t_zoomId) };
    for (auto it{ its.first }; it != its.second; ++it)
    {
        if (it->second.bshFileNameId == t_bshFileNameId)
        {
            return it->second;
        }
    }

    return std::nullopt;
}

const sg::file::BennoFile& sg::file::Files::GetColFile() const
{
    return m_colFile;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::file::Files::InitDirectoryTree(const std::string& t_path)
{
    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] Starts initializing filesystem...");

    for (const auto& entry : std::filesystem::recursive_directory_iterator(t_path, std::filesystem::directory_options::follow_directory_symlink))
    {
        if (is_directory(entry))
        {
            for (const auto& f : std::filesystem::directory_iterator(entry))
            {
                if (is_regular_file(f.status()))
                {
                    auto extension{ ToLowerCase(f.path().extension().string()) };

                    // BSH files
                    if (extension == ".bsh")
                    {
                        const auto zoomId{ renderer::Zoom::StringToZoomId(ToUpperCase(entry.path().filename().string())) };

                        if (zoomId != renderer::Zoom::ZoomId::NOT_ZOOMABLE)
                        {
                            BennoZoomableBshFile bshFile;
                            bshFile.fullFilename = f.path().filename().string();  // e.g. stadtfld.bsh
                            bshFile.path = f.path().string();                     // e.g. E:\\Anno\\GFX\\stadtfld.bsh
                            bshFile.zoomId = zoomId;
                            bshFile.bshFileNameId = BshFile::StringToBshFileNameId(ToUpperCase(f.path().stem().string()));

                            m_bshFiles.emplace(bshFile.zoomId, bshFile);
                        }
                        else
                        {
                            // todo: other (not zoomable) bsh files
                        }
                    }

                    // COL file
                    if (extension == ".col")
                    {
                        m_colFile.fullFilename = f.path().filename().string();
                        m_colFile.path = f.path().string();
                    }

                    // GAM files
                    if (extension == ".gam")
                    {

                    }
                }
            }
        }
    }

    CheckForBshFiles();
    CheckForColFile();

    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] Filesystem successfully initialized.");
}

//-------------------------------------------------
// Check for files
//-------------------------------------------------

void sg::file::Files::CheckForBshFiles()
{
    Log::SG_LOG_DEBUG("[Files::CheckForBshFiles()] Check for of all zoomable BSH files.");
    auto result{ true };
    for (const auto& bshFileId : BshFile::BSH_FILENAME_ID_TABLE)
    {
        for (auto zoomId : renderer::Zoom::ZOOM_TABLE)
        {
            const auto check{ CheckForBshFile(zoomId, bshFileId) };
            if (!check)
            {
                result = false;
            }

            Log::SG_LOG_INFO("[Files::CheckForBshFiles()] Does the file {}/{} exist? [{}]", renderer::Zoom::ZoomToString(zoomId), BshFile::BshFileNameIdToString(bshFileId), result);
        }
    }

    if (!result)
    {
        throw SG_EXCEPTION("[Files::CheckForBshFiles()] One or more BSH file(s) could not be found. Check the error messages above.");
    }
}

bool sg::file::Files::CheckForBshFile(const renderer::Zoom::ZoomId t_zoomId, const BshFile::BshFileNameId t_bshFileNameId)
{
    auto path{ GetBshFilePath(t_zoomId, t_bshFileNameId) };
    if (path.has_value())
    {
        const std::ifstream f(path.value());
        return f.good();
    }

    return false;
}

void sg::file::Files::CheckForColFile() const
{
    auto result{ true };
    if (m_colFile.path.empty() || m_colFile.fullFilename.empty())
    {
        result = false;
    }
    else
    {
        const std::ifstream f(m_colFile.path);
        result = f.good();
    }

    if (!result)
    {
        throw SG_EXCEPTION("[Files::CheckForColFile()] The Palette file could not be found.");
    }
}

std::optional<std::string> sg::file::Files::GetBshFilePath(const renderer::Zoom::ZoomId t_zoomId, const BshFile::BshFileNameId t_bshFileNameId)
{
    const auto its{ m_bshFiles.equal_range(t_zoomId) };
    for (auto it{ its.first }; it != its.second; ++it)
    {
        if (it->second.bshFileNameId == t_bshFileNameId)
        {
            return it->second.path;
        }
    }

    return std::nullopt;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::string sg::file::Files::ToLowerCase(const std::string& t_string) const
{
    auto newString{ t_string };
    for (auto& c : newString)
    {
        c = std::tolower(c);
    }

    return newString;
}

std::string sg::file::Files::ToUpperCase(const std::string& t_string) const
{
    auto newString{ t_string };
    for (auto& c : newString)
    {
        c = std::toupper(c);
    }

    return newString;
}
