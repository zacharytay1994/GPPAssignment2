Texture2D tex;

SamplerState samplr;

static const float3 lightPos = { 5.0f, 10.0f, 30.0f };
static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 3.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 normal : Normal, float2 tc : TexCoord) : SV_Target
{
	float4 colour = tex.Sample(samplr, tc); /*{1.0f,1.0f,0.0f,1.0f};*/
	// apply chroma filter, {255,0,255} Magenta
	if (colour.x == 1.0f && colour.y == 0.0f && colour.z == 1.0f && colour.w == 1.0f) {
		discard;
	}

	// fragment to light vector data
	const float3 vTol = lightPos - worldPos;
	const float distTol = length(vTol);
	const float3 dirTol = vTol / distTol;
	// diffuse attenuation
	const float att = 1.0f / (attConst + attLin * distTol + attQuad * (distTol * distTol));
	// diffuse intensity
	const float3 diffuse = colour * diffuseIntensity * att * max(0.0f, dot(dirTol, normal));
	// final color
	return float4(saturate(diffuse + ambient), 1.0f);
}