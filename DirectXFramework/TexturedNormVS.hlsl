struct VSOut {
	float3 worldPos : Position;
	float3 normal : Normal;
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

cbuffer CBuf {
	matrix transform;
	matrix model;
};

VSOut main(float3 pos : Position, float3 norm : Normal, float2 tex : TexCoord)
{
	VSOut vso;
	vso.worldPos = (float3)mul(float4(pos, 1.0f), model);
	vso.normal = mul(norm, (float3x3)model);
	vso.tex = tex;
	vso.pos = mul(float4(pos, 1.0f), transform);
	return vso;
}