vec2 getPaperGradient(vec3 tex) {
    float wobF = 0.05;

    float x_p = tex + vec2(wobF, 0);
    float x_m = tex - vec2(wobF, 0);
    float y_p = tex + vec2(0, wobF);
    float y_m = tex - vec2(0, wobF);

    float dx = texture(paper, x_p).r
                - texture(paper, x_m).r;
    float dy = texture(paper, y_p).r
                - texture(paper, y_m).r;

    return vec2(dx, dy);
}
