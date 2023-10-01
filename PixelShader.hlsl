struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color0 : COLOR;
    float3 color1 : COLOR1;
};

cbuffer constant : register(b0)
{
    float m_time;
}

float4 psmain (PS_INPUT input) : SV_Target
{
    return float4(lerp(input.color0, input.color1, ((sin(m_time)) + 1) / 2), 1.0f);
}