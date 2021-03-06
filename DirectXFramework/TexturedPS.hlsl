Texture2D text;

SamplerState samplr;

cbuffer cbuf {
	float4 color;
};

float4 main(float2 tc : TexCoord) : SV_Target
{
	float4 colour = text.Sample(samplr,tc) + color; /*{1.0f,0.0f,0.0f,1.0f};*/
	// apply chroma filter, {255,0,255} Magenta
	if (colour.x == 1.0f && colour.y == 0.0f && colour.z == 1.0f) {
		discard;
	}
	return colour;
}