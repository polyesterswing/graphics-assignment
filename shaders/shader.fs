#version 330 core

out vec4 FragColor;
in float vScalar;

in vec3 vBary;
in vec3 vNormal;
in vec3 vWorldPos;

uniform bool uShowWireframe;
uniform bool uShowIsoline;
uniform float uIsoline;
uniform int uColormap;
uniform float uMinVal;
uniform float uMaxVal;

vec3 viridis(float t) {
    t = clamp(t, 0.0, 1.0);
    vec3 c0 = vec3(0.2777, 0.0050, 0.3342);
    vec3 c1 = vec3(0.1056, 0.4703, 0.5836);
    vec3 c2 = vec3(0.9834, 0.9018, 0.1424);
    // Simple 3-stop mix
    float s = t * 2.0;
    if (s < 1.0) return mix(c0, c1, s);
    else         return mix(c1, c2, s - 1.0);
}

vec3 jet(float t) {
    t = clamp(t, 0.0, 1.0);
    float r = clamp(1.5 - abs(4.0*t - 3.0), 0.0, 1.0);
    float g = clamp(1.5 - abs(4.0*t - 2.0), 0.0, 1.0);
    float b = clamp(1.5 - abs(4.0*t - 1.0), 0.0, 1.0);
    return vec3(r, g, b);
}

void main()
{

	vec3 lightDir = normalize(vec3(0.4, 1.0, 0.3));
	float diffuse = max(dot(normalize(vNormal), lightDir), 0.0);

	float ambient = 0.2;

	float minimum = min(vBary.x, min(vBary.y, vBary.z));
	float scalar = vScalar;

	if (scalar < uMinVal || scalar > uMaxVal) {
		scalar = 1.0;
	}

	float isolineThickness = 0.005;

	if (minimum < 0.01 && uShowWireframe) {
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else if (uShowIsoline && (scalar < uIsoline + isolineThickness && scalar > uIsoline - isolineThickness)) {
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} else {
		vec3 color;
		if (uColormap == 0) {
			color = viridis(scalar);
		} else if (uColormap == 1) {
			color = jet(scalar);
		}

		vec3 finalColor = color * (ambient + diffuse);
		FragColor = vec4(finalColor, 1.0);
	}

}
