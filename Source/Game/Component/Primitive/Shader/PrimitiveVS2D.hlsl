#include "Primitive2D.hlsli"



PS_INPUT VS(float4 pos : POSITION)
{
    PS_INPUT output;
    
    output.Pos = mul(pos, g_W);

    output.Pos.x = (output.Pos.x / g_ViewPortWidth) * 2 - 1;  // 2D‚ÌÀ•W‚É•ÏŠ·
    output.Pos.y = 1 - (output.Pos.y / g_ViewPortHeight) * 2; // 2D‚ÌÀ•W‚É•ÏŠ·
    
    return output;
}