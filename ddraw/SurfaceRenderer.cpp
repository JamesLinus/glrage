#include "SurfaceRenderer.hpp"

#include "FragmentShader.hpp"
#include "VertexShader.hpp"
#include "GLUtils.hpp"

using glrage::VertexShader;
using glrage::FragmentShader;
using glrage::GLUtils;

namespace ddraw {

SurfaceRenderer::SurfaceRenderer() :
    m_config("DirectDraw"),
    m_surfaceTexture(GL_TEXTURE_2D),
    m_width(0),
    m_height(0)
{
    // configure buffer
    m_surfaceBuffer.bind();
    m_surfaceBuffer.data(0, nullptr, GL_STATIC_DRAW);

    // configure sampler
    std::string filterMethod = m_config.getString("filter_method", "linear");
    GLint filterMethodEnum;

    if (filterMethod == "nearest") {
        filterMethodEnum = GL_NEAREST;
    } else {
        filterMethodEnum = GL_LINEAR;
    }

    m_sampler.bind(0);
    m_sampler.parameteri(GL_TEXTURE_MAG_FILTER, filterMethodEnum);
    m_sampler.parameteri(GL_TEXTURE_MIN_FILTER, filterMethodEnum);

    // configure shaders
    m_program.attach(VertexShader().fromFile("shaders\\ddraw.vsh"));
    m_program.attach(FragmentShader().fromFile("shaders\\ddraw.fsh"));
    m_program.link();
    m_program.fragmentData("fragColor");

    GLUtils::checkError("DirectDrawRenderer::DirectDrawRenderer");
}

void SurfaceRenderer::upload(DDSURFACEDESC& desc, std::vector<uint8_t>& data) {
    m_surfaceTexture.bind();

    // update buffer if the size is unchanged, otherwise create a new one
    if (desc.dwWidth != m_width || desc.dwHeight != m_height) {
        m_width = desc.dwWidth;
        m_height = desc.dwHeight;
        glTexImage2D(GL_TEXTURE_2D, 0, TEX_INTERNAL_FORMAT, m_width, m_height, 0,
            TEX_FORMAT, TEX_TYPE, &data[0]);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height,
            TEX_FORMAT, TEX_TYPE, &data[0]);
    }
}

void SurfaceRenderer::render() {
    m_program.bind();
    m_surfaceBuffer.bind();
    m_surfaceFormat.bind();
    m_surfaceTexture.bind();
    m_sampler.bind(0);

    GLboolean texture2d = glIsEnabled(GL_TEXTURE_2D);
    if (!texture2d) {
        glEnable(GL_TEXTURE_2D);
    }

    GLboolean blend = glIsEnabled(GL_BLEND);
    if (blend) {
        glDisable(GL_BLEND);
    }

    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    if (depthTest) {
        glDisable(GL_DEPTH_TEST);
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (!texture2d) {
        glDisable(GL_TEXTURE_2D);
    }

    if (blend) {
        glEnable(GL_BLEND);
    }

    if (depthTest) {
        glEnable(GL_DEPTH_TEST);
    }

    GLUtils::checkError("SurfaceRenderer::render");
}

}