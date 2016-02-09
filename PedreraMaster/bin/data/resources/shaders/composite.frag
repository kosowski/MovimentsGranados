uniform sampler2DRect srcTex;
uniform sampler2DRect dstTex;
uniform float alpha;

varying vec4 vertColor;

void main (void){
   vec2 st = gl_TexCoord[0].st;
   vec4 src = texture2DRect(srcTex, st);
   vec4 dst = texture2DRect(dstTex, st);

    gl_FragColor = mix(src, dst, alpha);
}