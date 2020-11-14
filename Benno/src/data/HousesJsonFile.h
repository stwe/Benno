#pragma once

#include <string>
#include "Building.h"

namespace sg::data
{
    class HousesJsonFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        HousesJsonFile() = delete;

        explicit HousesJsonFile(const std::string& t_filePath);

        HousesJsonFile(const HousesJsonFile& t_other) = delete;
        HousesJsonFile(HousesJsonFile&& t_other) noexcept = delete;
        HousesJsonFile& operator=(const HousesJsonFile& t_other) = delete;
        HousesJsonFile& operator=(HousesJsonFile&& t_other) noexcept = delete;

        ~HousesJsonFile();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const std::unordered_map<int, Building>& GetBuildings() const noexcept;

        //-------------------------------------------------
        // Read
        //-------------------------------------------------

        void ReadFileData(const std::string& t_filePath);

    protected:

    private:
        std::unordered_map<int, Building> m_buildings;
    };
}
