struct MatrixBuffer {
	float4x4 WorldMat;
	float4x4 WorldViewProjMat;
};

ConstantBuffer<MatrixBuffer> s_matrixBuffer : register(b0);

struct VSInput {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct PSInput {
	float4 Position : SV_POSITION;
	float3 WorldPos : POSITION;
	float3 Normal : NORMAL;
};

PSInput main(VSInput input) {
	PSInput output;
	output.Position = mul(float4(input.Position, 1.f), s_matrixBuffer.WorldViewProjMat);
	output.WorldPos = mul(float4(input.Position, 1.f), s_matrixBuffer.WorldMat).xyz;
	output.Normal = mul(float4(input.Normal, 1.f), s_matrixBuffer.WorldMat).xyz;

	return output;
}
