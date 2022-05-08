struct MatrixBuffer {
	float4x4 WorldViewProj;
};

ConstantBuffer<MatrixBuffer> s_matrixBuffer : register(b0);

float4 main(float3 position : POSITION) : SV_POSITION {
	return mul(float4(position, 1.0), s_matrixBuffer.WorldViewProj);
}
