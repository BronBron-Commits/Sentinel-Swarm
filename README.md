# Sentinel-Swarm

**Sentinel-Swarm** is a deterministic multi-agent swarm simulation and visualization tool focused on **formation control, neighbor-based interaction, and real-time debug instrumentation**.

This project is part of the broader *Sentinel* simulation stack and is designed to demonstrate correctness-first engineering for autonomous systems, swarm coordination, and defense-style simulations.

---

## ✨ Key Capabilities

### Deterministic Simulation Core
- Fixed-step simulation loop
- Order-independent neighbor queries
- Repeatable results across runs
- No rendering feedback into simulation logic

### Multi-Agent Swarm Dynamics
- Neighbor-based interaction forces
- Configurable interaction radius
- Density-based behavior emergence
- Stable integration with damping

### Formation Control (Closed-Loop)
- Line, wedge, and orbit formations
- Slot-based formation assignment
- Continuous control forces driving convergence
- Smooth transitions between formations

### Real-Time Debug Visualization
- Agent positions and velocities
- Neighbor link visualization
- Neighbor radius overlays
- Error vectors (agent → formation slot)
- Convergence metrics overlay (avg/max error)
- Density-based agent coloring

### Interactive Controls
| Key | Action |
|----:|-------|
| `1` | Line formation |
| `2` | Wedge formation |
| `3` | Orbit formation |
| Close window | Exit simulation |

---

## 🧠 Architecture Overview

```

Sentinel-Swarm
├── simcore (submodule)
│   └── Deterministic simulation primitives
├── sentinel-orchestra (submodule)
│   └── Coordination & orchestration layer
├── swarm_update.cpp
│   └── Simulation & control laws
├── swarm_render.cpp
│   └── Visualization & debug tooling
└── main.cpp
└── Application loop & input handling

```

### Design Principles
- **Simulation and rendering are strictly separated**
- **Visualization is read-only**
- **All control logic lives in the simulation layer**
- **Debug tooling never affects determinism**

---

## 🧮 Formation Control Model

Each agent is assigned a **formation slot** computed from the swarm’s center of mass.

A proportional control force drives agents toward their assigned slot:

```

F = k * (slot_position − agent_position)

````

This force is blended with neighbor interaction forces and stabilized with damping to ensure smooth convergence without oscillation.

---

## 📊 Convergence Instrumentation

The renderer computes and displays:
- **Average formation error**
- **Maximum agent error**
- **Normalized convergence bar**

These metrics provide immediate visual feedback on system stability and control performance.

---

## 🔧 Build Instructions

### Dependencies
- C++17 compatible compiler
- SDL2
- OpenGL
- CMake ≥ 3.16

### Clone with Submodules
```bash
git clone --recurse-submodules git@github.com:BronBron-Commits/Sentinel-Swarm.git
cd Sentinel-Swarm
````

### Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/sentinel-swarm
```

---

## 🎯 Intended Use

This project is intended as:

* A **technical portfolio piece**
* A **multi-agent autonomy demo**
* A **deterministic simulation reference**
* A foundation for:

  * Drone swarm simulations
  * Vehicle convoys
  * Missile pack coordination
  * Distributed control experiments

---

## 🔮 Planned Extensions

* Formation blending and transitions
* Leader–follower control modes
* Agent failure and recovery testing
* Rollback and replay visualization
* Lockstep/networked simulation experiments

---

## 📜 License

MIT License.
