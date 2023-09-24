struct VS_INPUT
{
    float4 position0 : POSITION;
	float4 position1 : POSITION1;
    float3 color0 : COLOR;
    float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color0 : COLOR;
    float3 color1 : COLOR1;
};

cbuffer constant: register(b0)
{
    unsigned int m_time;
}

VS_OUTPUT vsmain (VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = lerp(input.position0, input.position1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);
    output.color0 = input.color0;
    output.color1 = input.color1;

    return output;
}