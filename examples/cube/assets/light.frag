#version 300 es

precision mediump float;

uniform vec4 color;
uniform mat4 viewMatrix;

//Light Properties
uniform vec3 lightPosition;
uniform float Ia;
uniform float Id;
uniform float Is;
//Material properties
uniform float Ka;
uniform float Kd;
uniform float Ks;

uniform samplerCube cubeTex;



in vec3 fragN;
in vec3 fragV;





out vec4 outColor;

vec4 BlinnPhong(vec3 N, vec3 V) {
  N = normalize(N);
  
  vec3 L = normalize((viewMatrix * vec4(lightPosition, 1.0)).xyz + V);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, 25.0);
  }

  vec4 diffuseColor = vec4(Ka * Ia * lambertian);
  vec4 specularColor = vec4(Kd * Id * specular);
  vec4 ambientColor = vec4(Ks * Is); 

  return ambientColor + diffuseColor + specularColor;
}

void main() {
  if (gl_FrontFacing) {
    outColor = color * BlinnPhong(fragN, fragV);
  } else {
    float i = (color.r + color.g + color.b) / 3.0;
    outColor = vec4(i, 0, 0, 1.0);
  }
}
