float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos + float4(+0.7f,0.2f,0,0);
}