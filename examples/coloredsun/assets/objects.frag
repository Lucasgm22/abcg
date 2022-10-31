#version 300 es

precision mediump float;

in vec4 fragColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform vec3 u_color1;
uniform vec3 u_color2;

out vec4 outColor;

void main() {
  vec2 st = gl_FragCoord.xy/u_resolution.xy;

  float mixValue = distance(st,u_mouse/u_resolution.xy);
  vec3 color = mix(u_color1,u_color2,mixValue);

  outColor = vec4(color,1);
}