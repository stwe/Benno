#include "Texture.h"
#include "OpenGL.h"
#include "SgAssert.h"

//-------------------------------------------------
// Generate
//-------------------------------------------------

uint32_t sg::gl::Texture::GenerateNewTextureId()
{
    uint32_t textureId{ 0 };

    glGenTextures(1, &textureId);
    SG_ASSERT(textureId, "[Texture::GenerateNewTextureId()] Invalid texture Id.");

    return textureId;
}

//-------------------------------------------------
// Filter
//-------------------------------------------------

void sg::gl::Texture::UseNoFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void sg::gl::Texture::UseNoFilter()
{
    UseNoFilter(GL_TEXTURE_2D);
}

void sg::gl::Texture::UseBilinearFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void sg::gl::Texture::UseBilinearFilter()
{
    UseBilinearFilter(GL_TEXTURE_2D);
}

void sg::gl::Texture::UseBilinearMipmapFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void sg::gl::Texture::UseBilinearMipmapFilter()
{
    UseBilinearMipmapFilter(GL_TEXTURE_2D);
}

//-------------------------------------------------
// Wrapping
//-------------------------------------------------

void sg::gl::Texture::UseRepeatWrapping(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void sg::gl::Texture::UseRepeatWrapping()
{
    UseRepeatWrapping(GL_TEXTURE_2D);
}

void sg::gl::Texture::UseClampToEdgeWrapping(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void sg::gl::Texture::UseClampToEdgeWrapping()
{
    UseClampToEdgeWrapping(GL_TEXTURE_2D);
}

//-------------------------------------------------
// Bind / Unbind
//-------------------------------------------------

void sg::gl::Texture::Bind(const uint32_t t_textureId, const uint32_t t_target)
{
    SG_ASSERT(t_textureId, "[Texture::Bind()] Invalid texture Id.");
    glBindTexture(t_target, t_textureId);
}

void sg::gl::Texture::Bind(const uint32_t t_textureId)
{
    Bind(t_textureId, GL_TEXTURE_2D);
}

void sg::gl::Texture::Unbind(const uint32_t t_target)
{
    glBindTexture(t_target, 0);
}

void sg::gl::Texture::Unbind()
{
    Unbind(GL_TEXTURE_2D);
}

void sg::gl::Texture::BindForReading(const uint32_t t_textureId, const uint32_t t_textureUnit, const uint32_t t_target)
{
    SG_ASSERT(t_textureId, "[Texture::BindForReading()] Invalid texture Id.");
    glActiveTexture(t_textureUnit);

    Bind(t_textureId, t_target);
}

void sg::gl::Texture::BindForReading(const uint32_t t_textureId, const uint32_t t_textureUnit)
{
    BindForReading(t_textureId, t_textureUnit, GL_TEXTURE_2D);
}
