#pragma once

#include <cstdint>

namespace sg::gl
{
    class Texture
    {
    public:
        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        static uint32_t GenerateNewTextureId();

        //-------------------------------------------------
        // Filter
        //-------------------------------------------------

        /**
         * @brief Use the default texture filtering method (nearest neighbor filtering) of OpenGL.
         *        OpenGL selects the pixel which center is closest to the texture coordinate.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseNoFilter()
         */
        static void UseNoFilter(uint32_t t_target);
        static void UseNoFilter();

        /**
         * @brief Takes an interpolated value from the texture coordinate's neighboring texels.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseBilinearFilter()
         */
        static void UseBilinearFilter(uint32_t t_target);
        static void UseBilinearFilter();

        /**
         * @brief Linearly interpolates between the two closest mipmaps and samples the texture via linear interpolation.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseBilinearMipmapFilter()
         */
        static void UseBilinearMipmapFilter(uint32_t t_target);
        static void UseBilinearMipmapFilter();

        //-------------------------------------------------
        // Wrapping
        //-------------------------------------------------

        /**
         * @brief The default behavior for textures. Repeats the texture image.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseRepeatWrapping()
         */
        static void UseRepeatWrapping(uint32_t t_target);
        static void UseRepeatWrapping();

        /**
         * @brief Clamps the coordinates between 0 and 1.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseClampToEdgeWrapping()
         */
        static void UseClampToEdgeWrapping(uint32_t t_target);
        static void UseClampToEdgeWrapping();

        //-------------------------------------------------
        // Bind / Unbind
        //-------------------------------------------------

        static void Bind(uint32_t t_textureId, uint32_t t_target);
        static void Bind(uint32_t t_textureId);

        static void Unbind(uint32_t t_target);
        static void Unbind();

        static void BindForReading(uint32_t t_textureId, uint32_t t_textureUnit, uint32_t t_target);
        static void BindForReading(uint32_t t_textureId, uint32_t t_textureUnit);

    protected:

    private:

    };
}
