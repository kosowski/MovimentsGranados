uniform sampler2DRect tex0;

varying vec4 vertColor;

void main (void){
   vec2 st = gl_TexCoord[0].st;
   vec4 src = texture2DRect(tex0, st);

float alpha =  src.r;
if(alpha > 0.8)
	alpha *= 2.;
 gl_FragColor = vec4((src.b + src.g ) * vertColor.rgb, (1. - clamp(alpha, 0., 1.)) * vertColor.a);
}