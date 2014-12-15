#version 330

smooth in vec3 theColor;
in vec2 texCoord;
out vec4 outputColor;

uniform sampler2D gSampler;

in vec3 norm;
in vec3 tolight;

void main()
{
    float lambert = ( dot( normalize( norm ), normalize( tolight )));
    float k = clamp( lambert, 0.3, 1.0 );
    vec4 col= texture(gSampler, texCoord);

	outputColor = vec4( col.rgb*k, col.a ) * vec4(theColor,1);
}