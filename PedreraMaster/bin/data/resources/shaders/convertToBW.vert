#version 120

varying vec4 vertColor;

void main()
{
    gl_TexCoord[0]=gl_MultiTexCoord0;
    vertColor = gl_Color,
	gl_Position = ftransform();
}