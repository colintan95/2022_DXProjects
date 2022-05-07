
float4 main(float2 screenPos : POSITION) : SV_POSITION {
	return float4(screenPos, 0, 1);
}
