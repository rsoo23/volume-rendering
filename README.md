# volume-rendering

## Description
This project mainly focuses on rendering the atmosphere and cloud like objects by using volume rendering techniques, C++ and Raylib as the graphics library.

## Showcase

## Things learnt:
- Beer's Law
- Low Dynamic Range + High Dynamic Range
- Phase Functions (Henyey-Greenstein, Rayleigh, Mie)
  - Isotropic + Anisotropic Scattering
  - The steradian unit
  - Asymmetry factor
- Properties of light through a volume: In-scattering, Out-scattering, Absorption, Emission
- Coefficients: Absorption, Scattering, Extinction
- Noise generation
  - Perlin noise sampling
  - Fractal brownian motion procedural texture
- Jittering
- Numerical Integration
- Russian roulette optimization technique (Monte Carlo method)
- Homogenous + Heterongenous Participating Mediums
- Optical depth
- Atmospheric Model
  - Scale height
  - Aerosols
  - Rayleight + Mie scattering
  - Zenith position
- Tone Mapping (Reinhard)
- Single Scattering vs Multiple Scattering
- Sphere, Plane + Ray intersection equations
- C++ Concepts:
  - Unique pointer
  - Namespaces (Named, Anonymous)
  - Precompiled headers
  - std::function
  - Lambdas
  - Random number generation (std::mt19937)
  - assert statements
  - Static member variables
  - Enums
  - Using `target_include_directories` in CMake for absolute include import paths
- Visual Studio:
  - Debugger
  - Integrated Git
  - Debug + Release build

## Future improvements
- convert rendering logic into shaders for real-time performance
- handling of multiple scene objects
- add emissive sphere to simulate the sun
- implement more realistic cloud shapes
- render Earth's surface + ocean using procedural texturing / mip-mapping
- light shafts
- improved camera features (fov, direction)
