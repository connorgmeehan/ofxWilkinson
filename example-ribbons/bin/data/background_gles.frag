
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2 resolution;
uniform float i_time;
uniform vec4 bg_color;
uniform vec4 bg_color;

varying vec2 texCoordVarying;

float Perlin2D( vec2 P ){
    //  https://github.com/BrianSharpe/Wombat/blob/master/Perlin2D.glsl

    // establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec4 Pf_Pfmin1 = P.xyxy - vec4( Pi, Pi + 1.0 );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    // calculate the gradient results
    vec4 grad_x = hash_x - 0.49999;
    vec4 grad_y = hash_y - 0.49999;
    vec4 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * Pf_Pfmin1.xzxz + grad_y * Pf_Pfmin1.yyww );

    // Classic Perlin Interpolation
    grad_results *= 1.4142135623730950488016887242097;  // scale things to a strict -1.0->1.0 range  *= 1.0/sqrt(0.5)
    vec2 blend = Pf_Pfmin1.xy * Pf_Pfmin1.xy * Pf_Pfmin1.xy * (Pf_Pfmin1.xy * (Pf_Pfmin1.xy * 6.0 - 15.0) + 10.0);
    vec4 blend2 = vec4( blend, vec2( 1.0 - blend ) );
    return dot( grad_results, blend2.zxzx * blend2.wwyy );
}

void main() {
    vec2 uv = gl_FragCoord.xy/resolution.xy;

    // Get frag at this coordinate
    vec4 metaball_frag = texture2D(tex0, uv.xy);

    vec4 noise_frag = vec4(
        Perlin2D(vec2(uv.x + i_time, uv.y)),
        Perlin2D(vec2(uv.x, uv.y + i_time)),
        Perlin2D(vec2(uv.x - i_time, uv.y)),
        1
    );

    gl_FragColor = mix(noise_frag, bg_color, bg_strength);
}
