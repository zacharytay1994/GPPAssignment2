Texture2D tex;

SamplerState samplr;

float4 main(float3 normal : Normal, float2 tc : TexCoord) : SV_Target
{
	/*float3 samplenormal = normal * 1.0f;
	if (normal.y == 1.0f) {
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}*/
	float4 colour = /*tex.Sample(samplr,tc);*/ {1.0f,0.0f,0.0f,1.0f};
	// apply chroma filter, {255,0,255} Magenta
	if (colour.x == 1.0f && colour.y == 0.0f && colour.z == 1.0f && colour.w == 1.0f) {
		discard;
	}
	return colour;
}