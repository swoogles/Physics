# Physics Sandbox Development Plan

## Project Overview
This codebase is a C++ physics simulation system using OpenGL visualization. The simulation models gravitational particle interactions with a focus on group introduction mechanics.

## Current State Analysis
The project already has:
- TimeManager infrastructure for timing operations
- ScenarioSpec and GroupSpec structures for defining particle groups
- Simulation framework with update loops
- Application framework with FullApplication class

## Implementation Status
- [x] Integrated time-based group introduction in FullApplication update loop
- [x] Added timing logic using simulation's built-in time tracking
- [x] Implemented 20-second intervals for group introduction
- [x] Limited to 5 total groups to prevent resource exhaustion
- [x] Clean integration with existing codebase patterns

## Implementation Details
- Modified `src/FullApplication.cpp` to add group introduction timing logic
- Uses simulation time (`getOutputViewingTime()`) to determine when to introduce groups
- Groups are introduced every 20 seconds starting at 20 seconds
- Maintains compatibility with existing simulation and particle structures

## Files Modified
- `src/FullApplication.cpp` - Added group introduction timing logic
- `src/FullApplication.h` - Cleaned up unused method declaration

## Testing
The implementation will be tested in the next phase by running simulations and verifying:
- Groups are introduced at correct time intervals (20 seconds apart)
- Maximum of 5 groups are introduced
- Simulation continues to operate normally during group introduction