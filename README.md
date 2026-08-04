# 3D CPU Black Hole Ray Tracer (in C++)

A lightweight real-time software renderer written in C++ using Raylib. It simulates light bending around a black hole and rendering an accretion disk—running entirely on the CPU without using GPU shaders.

<img width="1290" height="1891" alt="image" src="https://github.com/user-attachments/assets/ca738f94-c372-4dff-8607-d933757eca3e" />


## Why I Built This

I wanted to experiment with software rendering and physics math from scratch. Instead of writing a GPU shader on Shadertoy where hardware handles the parallelization automatically, I wanted to see how far I could push single-threaded CPU ray marching in pure C++.

## How It Works

1. **Resolution Upscaling Trick:** Ray-marching on a single CPU thread is expensive. To keep frame rates usable, the physics simulation renders to a low-res $150 \times 150$ raw pixel buffer in memory. Raylib's `DrawTexturePro` then upscales the buffer to $500 \times 500$ on the screen.
2. **Gravitational Lensing:** For every pixel, a ray is stepped forward in 3D space. On each step, the velocity vector curves toward the black hole using a simplified inverse-square law ($F = \frac{m}{d^2}$).
3. **Event Horizon & Escape:** 
   - If a ray crosses the event horizon radius ($d < 2.0$), it turns black.
   - If it escapes past $d > 40.0$, it renders the background color.
4. **Accretion Disk Detection:** The code checks when a light ray crosses the $y = 0$ horizontal plane. If the intersection distance falls between $4.0$ and $11.0$ units from the singularity, it hits the accretion disk.

## Controls

- **W / A / S / D or Arrow Keys:** Orbit camera angle & pinch pitch
- **Mouse Wheel:** Zoom camera in / out

## How to Run

### Prerequisites
- C++ Compiler (GCC, Clang, or MSVC)
- [Raylib](https://www.raylib.com/) installed

### Build Command (GCC example)
```bash
g++ main.cpp -lraylib -O3 -o blackhole
./blackhole
