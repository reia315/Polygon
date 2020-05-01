
struct VS_INPUT
{
	float4 position : POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input, float4 color : COLOR)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.color = color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_target{
	return input.color;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);

}

