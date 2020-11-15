#pragma once

#include <string>
#include <vector>

namespace sg::chunk
{
    class Chunk
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Chunk() = delete;

        explicit Chunk(std::ifstream& t_input);

        Chunk(const Chunk& t_other) = delete;
        Chunk(Chunk&& t_other) noexcept = delete;
        Chunk& operator=(const Chunk& t_other) = delete;
        Chunk& operator=(Chunk&& t_other) noexcept = delete;

        ~Chunk();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const char* GetId() const noexcept;
        [[nodiscard]] uint32_t GetLength() const noexcept;
        [[nodiscard]] const std::vector<uint8_t>& GetData() const noexcept;

    protected:

    private:
        static constexpr auto ID_SIZE_IN_BYTES{ 16 };

        std::string m_id;
        uint32_t m_length{ 0u };
        std::vector<uint8_t> m_data;
    };
}
