# volume-rendering

## Description
This project mainly focuses on rendering the atmosphere and cloud like objects by using volume rendering techniques, C++ and Raylib as the graphics library.

## Showcase
Assuming Earth's radius is 6,360km and the height of the atmosphere is 60km:

Camera Altitude: 10m
<img width="797" height="395" alt="image" src="https://github.com/user-attachments/assets/bc881c5e-73e8-42f8-a935-82a2920fc5ed" />

Camera Altitude: 2km
<img width="798" height="397" alt="image" src="https://github.com/user-attachments/assets/93ede305-f224-4549-a15e-f1915fe44387" />

Camera Altitude: 40km + Cloud
<img width="891" height="448" alt="image" src="https://github.com/user-attachments/assets/055f4830-e095-47cc-b6fa-401049aad81d" />

Camera Altitude: 50km
<img width="800" height="400" alt="image" src="https://github.com/user-attachments/assets/eecc727c-db63-4743-91a3-bc8ac30c21a2" />

Camera Altitude: 60km
<img width="798" height="399" alt="image" src="https://github.com/user-attachments/assets/21c616d1-baaa-472e-a73e-ade606183895" />

## Building

**Requirements**
- Visual Studio 2022 with the **Desktop development with C++** workload
- CMake (bundled with Visual Studio)

**Steps**
1. Clone the repository
git clone https://github.com/yourname/yourrepo.git
2. Open the cloned folder in Visual Studio via **File → Open → Folder**
3. Set configuration to **Release x64** in the toolbar
4. Build with **Ctrl+Shift+B** or **Build → Build All**
5. Start without debugging with the **Play Button** or **Ctrl + F5**

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
