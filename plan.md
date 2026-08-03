# Physics Sandbox Development Plan

## Project Overview
This codebase is a C++ physics simulation system using OpenGL visualization. The simulation models gravitational particle interactions with a focus on group introduction mechanics.

## Implementation Checklist

ONLY DO 1 of these things 1 time before waiting for me to reset your context
- confirm next item is already implemented and check it off
- confirm next item *needs* to be implemented and work till you can check it off, or break it into smaller components, to be easy enough to check off

### Core Implementation
- [ ] Integrate time-based group introduction in FullApplication update loop
- [ ] Add timing logic using simulation's built-in time tracking
- [ ] Implement 20-second intervals for group introduction
- [ ] Limit to 5 total groups to prevent resource exhaustion
- [ ] Maintain compatibility with existing codebase patterns

### Code Changes
- [ ] Modify `src/FullApplication.cpp` to add group introduction timing logic
- [ ] Clean up unused method declaration in `src/FullApplication.h`

### Testing
- [ ] Run simulations to verify groups are introduced at correct time intervals (20 seconds apart)
- [ ] Verify maximum of 5 groups are introduced
- [ ] Ensure simulation continues to operate normally during group introduction

### Documentation
- [ ] Update README with new group introduction functionality
- [ ] Document timing logic and group limits
- [ ] Add comments to modified source files

## Current Status
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