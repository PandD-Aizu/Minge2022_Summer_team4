# version 410

//
//	Textures
//
uniform sampler2D Texture0;

//
//	PSInput
//
layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 UV;

//
//	PSOutput
//
layout(location = 0) out vec4 FragColor;

//
//	Constant Buffer
//
layout(std140) uniform PSConstants2D
{
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColor;
	vec4 g_sdfShadowColor;
	vec4 g_internal;
};

// PS_1
layout(std140) uniform Blink
{
	int time;
};
// [C++]
//struct PoissonDisk
//{
//	float freq;
//};

//
//	Functions
//
void main()
{
	vec2 ra = texture(Texture0, UV + vec2(-0.02, 0.0)).ra;
	vec2 ga = texture(Texture0, UV).ga;
	vec2 ba = texture(Texture0, UV + vec2(+0.02, 0.0)).ba;

	vec4 a = vec4(1,1,1, (time/5)%2);
    
	vec4 texColor = texture(Texture0, UV);	

	FragColor = texColor * a;
}
