#version 300 es

precision mediump float;

in vec4 fragColor;

uniform vec4 u_resolution;

out vec4 outColor;

void main() {
    outColor = fragColor;
}