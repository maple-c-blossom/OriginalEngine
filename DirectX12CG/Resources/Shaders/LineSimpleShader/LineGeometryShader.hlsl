#include "LineHeader.hlsli"

[maxvertexcount(2)]
void main(
	line VSOutput input[2] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 2; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos;
		element.normal = input[i].normal;
		element.worldpos = input[i].worldpos;
		element.uv = input[i].uv;
		output.Append(element);
	}
}