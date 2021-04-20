#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

void main()
{
    vec4 color1 = texture(ourTexture1, TexCoord);
    //vec4 color2 = texture(ourTexture2, TexCoord);
    //color = mix(color1, color2, 1.0f);
    color = color1;
}

    /*
#version 330 core

in vec3 ourColor;

out vec4 outColor;

uniform vec2 u_resolution;

vec3 get_square(vec2 st, vec2 start, vec2 end, vec3 color) {
    vec2 left_bottom = step(start, st);
    vec2 right_top = step(end, 1.0 - st);

    return color * (left_bottom.x * left_bottom.y * right_top.x * right_top.y);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 canvas = ourColor;
    const int squares = 8;
    float increments = 1.0 / float(squares);
    bool start_color = false;
    bool flip = start_color;
    for (int i = 0; i < squares; i++) {
        for (int j = 0; j < squares; j++) {
            vec3 square = get_square(
                st,
                vec2(increments * float(j), increments * float(i)),
                vec2(1.0 - increments * float(j + 1), 1.0 - increments * float(i + 1)),
                vec3(flip)
            );
            canvas = mix(canvas, square, square);
            flip = !flip;
            start_color = !start_color;
            flip = start_color;
        }
    }
    outColor = vec4(canvas, 1.0);
}
    */