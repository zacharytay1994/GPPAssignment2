Texture2D tex;

SamplerState samplr;

cbuffer LightPositions : register(b1) {
	float4 point1;
	float4 point2;
	float4 directional1;
}

static const float3 lightPos = { 20.0f, 0.0f, -50.0f };
static const float3 dirPos = { -100.0f, 500.0f, -500.0f };
static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 20.0f;
static const float directionalIntensity = 1.2f;
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

	normal = normal / length(normal);

	// calculate direction vector
	const float3 directionLight = directional1 - worldPos;
	const float directionLightLen = length(directionLight);
	const float3 directionLightNorm = directionLight / directionLightLen;
	const float3 dirIntensity = max(0.0f, dot(directionLightNorm, normal));
	const float3 dirColour = colour * dirIntensity * directional1.w;

	// fragment to light vector data - Point1
	float3 vTol = point1 - worldPos;
	float distTol = length(vTol);
	float3 dirTol = vTol / distTol;
	// diffuse attenuation
	float att = 1.0f / (attConst + attLin * distTol + attQuad * (distTol * distTol));
	// diffuse intensity
	float3 diffuse = colour * point1.w * att * max(0.0f, dot(dirTol, normal));

	// fragment to light vector data - Point2
	vTol = point2 - worldPos;
	distTol = length(vTol);
	dirTol = vTol / distTol;
	// diffuse attenuation
	att = 1.0f / (attConst + attLin * distTol + attQuad * (distTol * distTol));
	// diffuse intensity
	float3 diffuse2 = colour * point2.w * att * max(0.0f, dot(dirTol, normal));

	// final color
	return float4(saturate(diffuse + diffuse2 + ambient + dirColour), 1.0f);
}