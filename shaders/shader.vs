#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out float vScalar;
out vec3 vBary;
out vec3 vNormal;
out vec3 vWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float hash(vec3 p) {
    p = fract(p * vec3(127.1, 311.7, 74.7));
    p += dot(p, p + 19.19);
    return fract(p.x * p.y * p.z);
}

float smoothNoise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
    vec3 u = f * f * (3.0 - 2.0 * f); // smoothstep

    return mix(
        mix(mix(hash(i),             hash(i + vec3(1,0,0)), u.x),
            mix(hash(i + vec3(0,1,0)), hash(i + vec3(1,1,0)), u.x), u.y),
        mix(mix(hash(i + vec3(0,0,1)), hash(i + vec3(1,0,1)), u.x),
            mix(hash(i + vec3(0,1,1)), hash(i + vec3(1,1,1)), u.x), u.y),
        u.z
    );
}

void main()
{
	vec4 world = model * vec4(aPos, 1.0);
	vWorldPos = world.xyz;

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vNormal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * world;

	float base = sin(world.x) * cos(world.z);
	float noise = smoothNoise(world.xyz * 2.0) * 0.4;
	float raw = base + noise;

	vScalar = clamp((raw + 1.4) / 2.8, 0.0, 1.0);

	vec3 bary = vec3(0.0);
	bary[gl_VertexID % 3] = 1.0;
	vBary = bary;

}
