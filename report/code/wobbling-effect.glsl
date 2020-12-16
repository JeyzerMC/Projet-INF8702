vec2 getPaperGradient() {
    float wobFactor = 0.05;

    float x_p = vertTexCoord
                + vec2(wobFactor, 0);
    float x_m = vertTexCoord
                - vec2(wobFactor, 0);
    float y_p = vertTexCoord
                + vec2(0, wobFactor);
    float y_m = vertTexCoord
                - vec2(0, wobFactor);

    float dx = texture(paper, x_p).r
                - texture(paper, x_m).r;
    float dy = texture(paper, y_p).r
                - texture(paper, y_m).r;

    return vec2(dx, dy);
}
