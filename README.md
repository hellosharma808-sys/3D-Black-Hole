# 3D-Black-Hole
A real-time 3D simulation of gravitational lensing around a Schwarzschild black hole, built from scratch using C++ and Raylib. The project simulates general relativity by bending light rays through space using numerical integration.
## Features
* **Real-Time Ray Tracing:** Simulates light paths directly from the camera viewport on the CPU.
* **Accretion Disk Collision:** Features a mathematically accurate horizontal accretion disk that warps visually based on the gravity well.
* **Interactive Camera Controls:** Complete 3D orbital camera handling via keyboard inputs (WASD / Arrows) and mouse scroll zooming.

## Visual Evolution

### Phase 1: Particle Trail Simulation
Initially, the engine tracked individual light particles moving past the singularity, leaving vector path lines. This was excellent for analyzing the physical trajectories but lacked a cinematic lens view.

<img width="446" height="366" alt="Screenshot 2026-06-14 163508" src="https://github.com/user-attachments/assets/a17f7e69-4d1b-4e6f-8c63-4b97f09d5c1e" />


### Phase 2: True Screen-Space Ray Tracer (Final)
The final architecture shifts the logic to trace rays directly from the camera sensor pixels. This accurately captures the visual warping of the accretion disk around the event horizon, creating the iconic silhouette.

<img width="254" height="262" alt="Screenshot 2026-06-14 203116" src="https://github.com/user-attachments/assets/c0bf2468-9ed0-4261-9643-a435bf3f3937" />


## Technical Challenges & Optimization
Running 30,000 independent physics simulations ($200 \times 150$ view resolution) on a single CPU core every single frame creates a massive mathematical bottleneck. 

To maintain a smooth **60 FPS** on standard hardware without relying on complex GPU shaders, the resolution of the internal rendering buffer was downscaled and dynamically upscaled via bilinear filtering to fill the $800 \times 600$ application window. Ray velocity steps were also scaled up to minimize unnecessary calculations in deep space.

## Controls
* `W` / `Up Arrow` - Tilt Camera Up
* `S` / `Down Arrow` - Tilt Camera Down
* `Mouse Wheel` - Zoom In / Out

## Dependencies
* [Raylib 5.0+](https://www.raylib.com/)
