#version 330 core

out vec4 outCol;
uniform bool isDay;

void main()
{
	if (isDay)
	{
		outCol = vec4(0.3, 0.7, 0.8, 1.0);
	}
	if (!isDay)
	{
		outCol = vec4(0.08, 0.2, 0.25, 1.0);
	}
	
}