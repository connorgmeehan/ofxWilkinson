
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0_in;
uniform vec2 resolution;
uniform float threshold;
uniform float i_time;
uniform float bg_strength;

varying vec2 texCoordVarying;

void main() {
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    float ripple_scale = 10.0;   

    // center origin
    uv-=.5;

    vec3 bg_frag = vec3(0, 0.5, 0.5);
    float dist = distance(uv * ripple_scale, vec2(0.5, 0.5)) - i_time;
    bg_frag.r += sin(dist*0.99)*cos(dist*0.97)*0.2;
    bg_frag.g += sin(dist*1.0)*cos(dist*1.)*0.5;
    bg_frag.b += sin(dist*1.01)*cos(dist*1.03)*0.7;

    vec4 metaball_frag = texture2D(tex0_in, uv.xy);

    gl_FragColor = vec4(metaball_frag.rgb + bg_frag*bg_strength, 1);
}
