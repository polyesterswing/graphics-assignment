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

uniform float uDisp;

void main()
{
	vec3 disp = vec3(sin(aPos.x), cos(aPos.y), sin(aPos.z));
	vec3 displaced = aPos + uDisp * disp;

	vec4 world = model * vec4(displaced, 1.0);
	vWorldPos = world.xyz;

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vNormal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * world;

	float base = sin(world.x) * cos(world.z);

	vScalar = (base + 1.0) * 0.5;

	vec3 bary = vec3(0.0);
	bary[gl_VertexID % 3] = 1.0;
	vBary = bary;

}
