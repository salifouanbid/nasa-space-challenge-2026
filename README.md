# ASTRA — Space Mission Design & Exploration Simulator

ASTRA is an educational space-mission simulation for the NASA International Space Apps Challenge 2026. Players design a mission, balance limited resources, explore a planetary environment, collect scientific observations, and review the mission outcome.

## Challenge and solution

Space missions are systems of connected decisions. ASTRA makes those connections visible: equipment affects mass and energy, exploration consumes time and oxygen, instruments produce data, and communication capacity limits what can be transmitted.

The game combines two layers:

1. **Mission design:** select objectives, equipment, instruments, communication capacity, and an exploration strategy.
2. **Planetary exploration:** move through a terrain, use an astronaut or rover, investigate points of interest, collect samples, and evaluate the results.

The first source delivery implements the portable mission rules and data foundation for three scenarios:

- **Alpha — Basaltic Plains:** characterize an ancient volcanic surface.
- **Beta — Highlands and Craters:** compare landforms and geological units.
- **Gamma — South Pole:** study a polar site and investigate possible ice indicators without guaranteeing a discovery.

The first delivery uses local simulated collaboration. It does not include network multiplayer, accounts, or matchmaking.

## Educational goals

ASTRA is designed for beginners, students, and players interested in space science. It introduces mission planning, engineering trade-offs, resource management, scientific observation, data handling, and exploration logistics without requiring prior aerospace knowledge.

Each mission produces a report explaining objectives, discoveries, resources consumed, mission outcome, and areas for improvement. This makes the game decision-driven rather than a simple walking or vehicle demo.

## Scientific foundation

The Gamma scenario is based on NASA/GSFC information from the LRO/LOLA South Pole DEM products. NASA observations about lunar water and ice are represented as scientific investigation targets, not as guaranteed resources. Alpha and Beta use lunar terrain concepts informed by the NASA-referenced SLDEM2015 product.

The project does not reproduce professional flight or mission-design software. Gameplay values such as energy, oxygen, time, mass, budget, and score are simplified and explicitly kept separate from real spacecraft specifications.

See [`docs/NASA_DATA_SOURCES.md`](docs/NASA_DATA_SOURCES.md) for sources, facts used, citations, and data-handling rules.

## Current source delivery

This repository currently contains:

- a portable C++11 mission core independent of Unreal Engine and Unity;
- resource, observation, objective, and mission-report rules;
- JSON scenario data for Alpha, Beta, and Gamma;
- French and English localization data;
- Unreal Engine 4.8 and Unity integration guides;
- NASA data provenance and architecture documentation;
- automated tests for the portable mission core.

The current environment does not include Unreal Engine 4.8 or Unity. Therefore, engine-specific levels, Blueprints, widgets, prefabs, terrain imports, and 3D assets still need to be assembled and tested in the selected editor.

## Build and test the portable core

CMake is the recommended build path:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

A direct C++11 build is also possible:

```bash
g++ -std=c++11 -Wall -Wextra -Werror \
  -Iinclude src/MissionCore.cpp tests/MissionCoreTests.cpp \
  -o astra_core_tests
./astra_core_tests
```

## Repository structure

- `include/AstraCore` and `src`: engine-independent mission rules.
- `data/scenarios`: scenario definitions and gameplay parameters.
- `data/localization`: French and English interface strings.
- `docs/NASA_DATA_SOURCES.md`: NASA sources and scientific-data policy.
- `docs/ARCHITECTURE.md`: portable architecture and engine boundaries.
- `adapters/unreal`: Unreal Engine 4.8 integration contract.
- `adapters/unity`: Unity integration contract.
- `tests`: portable core tests.

## Engine strategy

Mission rules, scenario identifiers, localization keys, and scientific-data references are shared across engines. Unreal-specific actors, Blueprints, widgets, levels, and input remain in the Unreal layer. Unity-specific scenes, prefabs, scripts, and input remain in the Unity layer.

This approach provides practical portability without claiming that a complete Unreal project can run unchanged in Unity.

## NASA submission materials

The NASA Space Apps project page should contain the project summary, challenge connection, demonstration video, source-code link, scientific-data credits, and a clear statement of what is implemented versus planned. The repository is the source-code reference for the project; no demonstration video or external presentation link is claimed here until one is produced.

## License

The source code in this repository is released under the MIT License. NASA datasets and media may have separate attribution and usage conditions. Those conditions must be preserved whenever original terrain, imagery, or other external assets are added.

## Team motto

> Design the Mission. Explore the Unknown. Expand Humanity's Horizon.
