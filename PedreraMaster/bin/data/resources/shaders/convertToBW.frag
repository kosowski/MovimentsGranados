uniform sampler2DRect tex0;

varying vec4 vertColor;

void main (void){
   vec2 st = gl_TexCoord[0].st;
   vec4 src = texture2DRect(tex0, st);

    gl_FragColor = vec4((src.b + src.g ) * vertColor.rgb, (1. - src.r) * vertColor.a);
}