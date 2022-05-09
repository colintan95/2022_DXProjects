struct PSInput {
	float4 Position : SV_POSITION;
	float3 WorldPos : POSITION;
	float3 Normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET {
	float3 position = input.WorldPos.xyz;
	float3 normal = normalize(input.Normal);

	float3 lightPos = { 0.f, 2.f, -6.f };
	float3 lightDir = normalize(lightPos - position);

	float3 ambientColor = { 0.1f, 0.1f, 0.1f };

	float diffuseCoeff = clamp(dot(lightDir, normal), 0.f, 1.f);
	float3 diffuseColor = { 0.5f, 0.5f, 0.5f };

	return float4(ambientColor + diffuseCoeff * diffuseColor, 1.f);
}
