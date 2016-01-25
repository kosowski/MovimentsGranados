uniform sampler2DRect tex0;

void main (void){
   vec2 st = gl_TexCoord[0].st;
   vec4 src = texture2DRect(tex0, st);

    gl_FragColor = vec4(vec3(src.b + src.g ), src.r);
}