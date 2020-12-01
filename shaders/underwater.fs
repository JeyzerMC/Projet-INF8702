#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

// Textures
uniform sampler2D oPosition;
uniform sampler2D oColor;

uniform int scr_width;
uniform int scr_height;

uniform vec3 camPos;

uniform int showEffects;
uniform bool showBlur;
uniform bool showTint;

float w_offset = 1.0 / scr_width;
float h_offset = 1.0 / scr_height;

// Kind of a hack for now
float max_depth = 2000.0;


// Gaussian Filter 5x5 : sigma=2.5
// 0.028672	0.036333	0.039317	0.036333	0.028672
// 0.036333	0.046042	0.049824	0.046042	0.036333
// 0.039317	0.049824	0.053916	0.049824	0.039317
// 0.036333	0.046042	0.049824	0.046042	0.036333
// 0.028672	0.036333	0.039317	0.036333	0.028672
vec3 gaussianBlur(float x, float y, float range)
{
    vec4 sum = vec4(0.0);

	sum += texture(oColor, vec2(x - 2.0 * w_offset * range, y - 2.0 * h_offset * range)) * 0.028672;
	sum += texture(oColor, vec2(x - 2.0 * w_offset * range, y - 1.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x - 2.0 * w_offset * range, y)) * 0.039317;
	sum += texture(oColor, vec2(x - 2.0 * w_offset * range, y + 1.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x - 2.0 * w_offset * range, y + 2.0 * h_offset * range)) * 0.028672;

	sum += texture(oColor, vec2(x - 1.0 * w_offset * range, y - 2.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x - 1.0 * w_offset * range, y - 1.0 * h_offset * range)) * 0.046042;
	sum += texture(oColor, vec2(x - 1.0 * w_offset * range, y)) * 0.049824;
	sum += texture(oColor, vec2(x - 1.0 * w_offset * range, y + 1.0 * h_offset * range)) * 0.046042;
	sum += texture(oColor, vec2(x - 1.0 * w_offset * range, y + 2.0 * h_offset * range)) * 0.036333;

	sum += texture(oColor, vec2(x, y - 2.0 * h_offset * range)) * 0.039317;
	sum += texture(oColor, vec2(x, y - 1.0 * h_offset * range)) * 0.049824;
	sum += texture(oColor, vec2(x, y)) * 0.053916;
	sum += texture(oColor, vec2(x, y + 1.0 * h_offset * range)) * 0.049824;
	sum += texture(oColor, vec2(x, y + 2.0 * h_offset * range)) * 0.039317;

	sum += texture(oColor, vec2(x + 1.0 * w_offset * range, y - 2.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x + 1.0 * w_offset * range, y - 1.0 * h_offset * range)) * 0.046042;
	sum += texture(oColor, vec2(x + 1.0 * w_offset * range, y)) * 0.049824;
	sum += texture(oColor, vec2(x + 1.0 * w_offset * range, y + 1.0 * h_offset * range)) * 0.046042;
	sum += texture(oColor, vec2(x + 1.0 * w_offset * range, y + 2.0 * h_offset * range)) * 0.036333;

	sum += texture(oColor, vec2(x + 2.0 * w_offset * range, y - 2.0 * h_offset * range)) * 0.028672;
	sum += texture(oColor, vec2(x + 2.0 * w_offset * range, y - 1.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x + 2.0 * w_offset * range, y)) * 0.039317;
	sum += texture(oColor, vec2(x + 2.0 * w_offset * range, y + 1.0 * h_offset * range)) * 0.036333;
	sum += texture(oColor, vec2(x + 2.0 * w_offset * range, y + 2.0 * h_offset * range)) * 0.028672;

    return sum.xyz;
}

// Takes something in the range [src_low, src_high] and remaps it to the range [dst_low, dst_high]
float remap(float value, float src_low, float src_high, float dst_low, float dst_high) {
    return dst_low + (value - src_low) * (dst_high - dst_low) / (src_high - src_low);
}

vec3 getBlur(vec2 tex, vec3 color, vec3 pos)
{
    vec3 blur = gaussianBlur(tex.x, tex.y, 5);
    // blur += gaussianBlur(tex.x, tex.y, 10);
    // blur /= 2;

    float w = 0.5;
    
    // Base water blur
    vec3 col = color;
    // col = mix(color, blur, w);
    
    // Depth blur
    blur = gaussianBlur(tex.x, tex.y, 3);
    float fragDepth = length(pos - camPos);
    w = min(1, max(0, pow(fragDepth, 3) / max_depth));
    col = mix(col, blur, w);

    // Viewport blur
    blur = gaussianBlur(tex.x, tex.y, 10);

    float vpDist = length(tex - 0.5);
    if (vpDist > 0.35){
       w = remap(vpDist, 0.35, sqrt(0.5), 0, 1);
       col = mix(col, blur, w);
    }

    return col;
    // return vec3(0.0, 0.0, fragDepth / max_depth);
}

void main()
{
    vec3 color = texture(oColor, vertTexCoord).rgb;
    vec3 pos = texture(oPosition, vertTexCoord).rgb;

    color = getBlur(vertTexCoord, color, pos);

    // Blue tint effect
    color = mix(color, vec3(0.0, 0.07, 0.1), 0.25); 

    // Darken edge effect
    float vpDist = length(vertTexCoord - 0.5);
    if (vpDist > 0.45){
       color *= 1 - remap(vpDist, 0.45, sqrt(0.5), 0, 0.7);
    }

    fragColor = vec4(color, 1.0);
}