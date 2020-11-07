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
    InitDirectoryTree(t_path);

    Log::SG_LOG_DEBUG("[Files::Files()] Create Files.");
}

sg::file::Files::~Files()
{
    Log::SG_LOG_DEBUG("[Files::~Files()] Destruct Files.");
}

//-------------------------------------------------
// Getter read-only
//-------------------------------------------------

std::optional<sg::file::BennoFile> sg::file::Files::GetFile(const renderer::Zoom t_zoom, const std::string& t_stem) const
{
    const auto its{ m_bshFiles.equal_range(t_zoom) };
    for (auto it{ its.first }; it != its.second; ++it)
    {
        if (it->second.stem == ToUpperCase(t_stem))
        {
            return it->second;
        }
    }

    return std::nullopt;
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
                        const auto it{ renderer::STRING_TO_ZOOM.find(entry.path().filename().string()) };
                        if (it != renderer::STRING_TO_ZOOM.end())
                        {
                            BennoFile bennoFile;
                            bennoFile.fullFilename = f.path().filename().string();
                            bennoFile.stem = ToUpperCase(f.path().stem().string());
                            bennoFile.path = f.path().string();

                            m_bshFiles.emplace(it->second, bennoFile);
                        }

                        // todo: other bsh files
                    }

                    // COL file
                    if (extension == ".col")
                    {
                        
                    }

                    // GAM files
                    if (extension == ".gam")
                    {

                    }
                }
            }
        }
    }

    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] SGFX BSH files count: {}", m_bshFiles.count(renderer::Zoom::SGFX));
    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] MGFX BSH files count: {}", m_bshFiles.count(renderer::Zoom::MGFX));
    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] GFX BSH files count: {}", m_bshFiles.count(renderer::Zoom::GFX));

    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] Check the existence of some BSH files.");
    auto result{ true };
    for (const auto& file : m_filesToCheck)
    {
        for (auto zoomType : renderer::ZOOM_TYPES)
        {
            const auto check{ CheckFile(zoomType, file) };
            if (!check)
            {
                result = false;
            }

            Log::SG_LOG_INFO("[Files::InitDirectoryTree()] Does the file {}/{} exist? [{}]", renderer::ZOOM_TO_STRING.at(zoomType), file, result);
        }
    }

    if (!result)
    {
        throw SG_EXCEPTION("[Files::InitDirectoryTree()] A file could not be found. Check the error message above.");
    }

    Log::SG_LOG_DEBUG("[Files::InitDirectoryTree()] Filesystem successfully initialized.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::file::Files::CheckFile(const renderer::Zoom t_zoom, const std::string& t_stem)
{
    auto path{ GetPath(t_zoom, t_stem) };
    if (path.has_value())
    {
        const std::ifstream f(path.value());
        return f.good();
    }

    return false;
}

std::optional<std::string> sg::file::Files::GetPath(const renderer::Zoom t_zoom, const std::string& t_stem)
{
    const auto its{ m_bshFiles.equal_range(t_zoom) };
    for (auto it{ its.first }; it != its.second; ++it)
    {
        if (it->second.stem == ToUpperCase(t_stem))
        {
            return it->second.path;
        }
    }

    return std::nullopt;
}

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
