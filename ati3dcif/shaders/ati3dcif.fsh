#version 330 core
#extension GL_EXT_gpu_shader4: enable

// ATI3DCIF enums

// C3D_ESHADE
#define C3D_ESH_NONE     0   // shading mode is undefined
#define C3D_ESH_SOLID    1   // shade using the clrSolid from the RC
#define C3D_ESH_FLAT     2   // shade using the last vertex to flat shade
#define C3D_ESH_SMOOTH   3   // shade using linearly interpolating vert clr

// C3D_ETEXOP
#define C3D_ETEXOP_NONE          0  // 
#define C3D_ETEXOP_CHROMAKEY     1  // select texels not equal to the chroma key
#define C3D_ETEXOP_ALPHA         2  // pass texel alpha to the alpha blender
#define C3D_ETEXOP_ALPHA_MASK    3  // lw bit 0: tex not drawn otw: alpha int

#define CHROMA_EPS 1.0 / 255.0

in vec4 vertColor;
flat in vec4 vertColorFlat;
in vec3 vertTexCoords;

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex0;
uniform vec4 solidColor;
uniform vec3 chromaKey;
uniform int shadeMode;
uniform bool tmapEn;
uniform int texOp;

void main(void) {
    switch (shadeMode) {
        case C3D_ESH_NONE:
            if (!tmapEn) {
                // no texture and no shading mode: nothing to render
                discard;
            }
            break;
    
        case C3D_ESH_SOLID:
            fragColor = solidColor;
            break;
        
        case C3D_ESH_FLAT:
            fragColor = vertColorFlat;
            break;
            
        case C3D_ESH_SMOOTH:
            fragColor = vertColor;
            break;
    }

    if (tmapEn) {
        if (texOp == C3D_ETEXOP_CHROMAKEY) {
            // discard fragment if texel matches chroma key
            ivec2 size = textureSize2D(tex0, 0);
            int tx = int((vertTexCoords.x / vertTexCoords.z) * size.x) % size.x;
            int ty = int((vertTexCoords.y / vertTexCoords.z) * size.y) % size.y;
            vec4 texel = texelFetch(tex0, ivec2(tx, ty), 0);
            float diff = abs(distance(texel.rgb, chromaKey));
            if (diff == 0) {
                discard;
            }
        }
        
        fragColor *= texture(tex0, vertTexCoords.xy / vertTexCoords.z);
    }
}