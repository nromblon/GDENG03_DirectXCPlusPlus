struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color0 : COLOR;
    float3 color1 : COLOR1;
};

cbuffer constant : register(b0)
{
    unsigned int m_time;
}

float4 psmain (PS_INPUT input) : SV_Target
{
    return float4(lerp(input.color0, input.color1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f), 1.0f);
}