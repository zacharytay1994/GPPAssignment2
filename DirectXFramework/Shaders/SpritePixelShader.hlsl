Texture2D tex;

SamplerState samplr;

float4 main(float2 tc : TexCoord) : SV_Target
{
	//float4 colour = tex.Sample(samplr,tc);
	float4 colour = {0.0f,0.0f,0.0f, 1.0f};
	// apply chroma filter, {255,0,255} Magenta
	if (colour.x == 1.0f && colour.y == 0.0f && colour.z == 1.0f && colour.w == 1.0f) {
		discard;
	}
	return colour;
}