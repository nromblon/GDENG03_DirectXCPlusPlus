struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};



float4 psmain (PS_INPUT input) : SV_Target
{
    return float4(input.color, 1.0f);
}