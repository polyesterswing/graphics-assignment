# Scientific Field Visualizer
This is a visualizer written using OpenGL, C++, and Dear Imgui

<img width="1824" height="1488" alt="image" src="https://github.com/user-attachments/assets/06f665f5-c927-4bbf-8ead-6392845ce578" />


# Building and Running
Downloading deps

```bash
brew install glfw glm
```

Make sure to modify `CXXFLAGS` in the `Makefile` to point it to the right include paths for your system
```bash
git clone --recursive "https://github.com/polyesterswing/graphics-assignment.git"
cd graphics-assignment
make run
```

# Implemented Features
## Core
- [x] Load a triangle mesh
  - Loading obj files using tiny_obj_loader
- [x] Per-vertex scalar field visualization
  - Scalar value generated using `(sin(x)*cos(z) + 1) * 0.5`
  - Colourmaps generated using formulas from https://www.shadertoy.com/view/WlfXRN
- [x] Orbital Camera
- [x] Lighting using Lambertian diffuse shading

## Level 2
- [x] Colourmap selector
  - Imgui dropdown for viridis, plasma, magma, inferno
     
- [x] Wireframe drawing
  - Wireframe drawn by providing barycentric coordinates from the vertex shader, which the fragment shader interpolates.
    Fragments where the minimum of x, y, z is close to zero must be the edge

- [x] Scalar clamping

## Level 3
- [x] Displacement overlay
  - Displacement calculated using `vec3(sin(x), cos(y), sin(z))`
- [x] Isoline rendering
- [ ] GPU Compute pass
  - MacOS does not support OpenGL 4.3 which introduced Compute Shaders

# Something I would do differently
- Use fwidth for actual one-pixel wide wireframes and isolines
- Code cleanups, more structs and classes
