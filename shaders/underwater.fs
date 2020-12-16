#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

// Textures
uniform sampler2D oPosition;
uniform sampler2D oColor;
uniform sampler2D turbulentFlow;
uniform sampler2D pigmentDispersion;
uniform sampler2D paperLayer;

uniform int scr_width;
uniform int scr_height;

uniform vec3 camPos;

uniform int showEffects;
uniform bool showBlur;
uniform bool showTint;
uniform bool showEdges;
uniform bool showWobbling;
uniform bool showWatercolorTextures;

float w_offset = 1.0 / scr_width;
float h_offset = 1.0 / scr_height;

// Kind of a hack for now
float max_depth = 2000.0;

// Utility functions to change from rgb to hsv and opposite.
// Comes from: https://gist.github.com/yiwenl
vec3 rgb2hsv(vec3 rgb) {
    float Cmax = max(rgb.r, max(rgb.g, rgb.b));
    float Cmin = min(rgb.r, min(rgb.g, rgb.b));
    float delta = Cmax - Cmin;

    vec3 hsv = vec3(0.0, 0.0, Cmax);

    if (Cmax > Cmin) {
        hsv.y = delta / Cmax;

        if (rgb.r == Cmax)
            hsv.x = (rgb.g - rgb.b) / delta;
        else {
            if (rgb.g == Cmax)
                hsv.x = 2.0 + (rgb.b - rgb.r) / delta;
            else
                hsv.x = 4.0 + (rgb.r - rgb.g) / delta;
        }
        hsv.x = fract(hsv.x / 6.0);
    }
    return hsv;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


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
    float w = 0.5;
    
    // Base water blur
    vec3 col = color;
    
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
}

vec2 getPaperGradient() {
    float wobFactor = 0.05;
    float dx = texture(paperLayer, vertTexCoord + vec2(wobFactor, 0)).r - texture(paperLayer, vertTexCoord - vec2(wobFactor, 0)).r;
    float dy = texture(paperLayer, vertTexCoord + vec2(0, wobFactor)).r - texture(paperLayer, vertTexCoord - vec2(0, wobFactor)).r;
    return vec2(dx, dy);
}

vec3 darkenEdge(vec3 color, float factor)
{
    vec3 hsvColor = rgb2hsv(color);
    hsvColor.z *= factor;
    return hsv2rgb(hsvColor);
}

vec3 edgeDetection(vec3 litColor, vec2 texCoords)
{
    vec2 offsets[9] = vec2[](
        vec2(-w_offset,  h_offset), // top-left
        vec2( 0.0f,    h_offset), // top-center
        vec2( w_offset,  h_offset), // top-right
        vec2(-w_offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( w_offset,  0.0f),   // center-right
        vec2(-w_offset, -h_offset), // bottom-left
        vec2( 0.0f,   -h_offset), // bottom-center
        vec2( w_offset, -h_offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(oColor, texCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    float dst = length(col);
    if (dst > 0.5) return darkenEdge(litColor, 0.5);
    return litColor;
}

void main()
{
    vec2 texCoords = vertTexCoord;

    if (showEffects == 1 || (showEffects == 0 && showWobbling)) {
        texCoords += getPaperGradient() * 0.015;
        // For some reason, 1 still resulted in wrapping, but 0.9999 is fine :/
        texCoords = clamp(texCoords, 0.0, 0.9999);
    }

    vec3 color = texture(oColor, texCoords).rgb;
    vec3 pos = texture(oPosition, texCoords).rgb;

    if (showEffects == 1 || (showEffects == 0 && showEdges)) {
        color = edgeDetection(color, texCoords);
    }

    if (showEffects == 1 || (showEffects == 0 && showBlur))
        color = getBlur(texCoords, color, pos);

    // Blue tint effect
    if (showEffects == 1 || (showEffects == 0 && showTint))
        color = mix(color, vec3(0.0, 0.07, 0.1), 0.20); 

    // Darken edge effect
    float vpDist = length(texCoords - 0.5);
    if (vpDist > 0.45){
       color *= 1 - remap(vpDist, 0.45, sqrt(0.5), 0, 0.7);
    }

    if (showEffects == 1 || (showEffects == 0 && showWatercolorTextures)) {
        color = mix(color, color * texture(turbulentFlow, pos.xz / 15.0).rgb, 0.5);
        color = mix(color, color * texture(pigmentDispersion, pos.xz).rgb, 0.3);
        color = mix(color, color * texture(paperLayer, pos.xz / 5.0).rgb, 0.5);
    }

    fragColor = vec4(color, 1.0);
}