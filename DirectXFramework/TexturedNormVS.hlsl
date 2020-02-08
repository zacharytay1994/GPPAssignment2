struct VSOut {
	float3 normal : Normal;
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

cbuffer CBuf {
	matrix transform;
};

VSOut main(float3 pos : Position, float3 norm : Normal, float2 tex : TexCoord)
{
	VSOut vso;
	vso.normal = norm;
	vso.tex = tex;
	vso.pos = mul(float4(pos, 1.0f), transform);
	return vso;
}