# Timed group arrivals

## Goal

New groups fall into a run that is already in progress. Configurable, e.g.
"a new group every 5 seconds of rendered video, up to 6 of them", driven from a
properties file like every other part of a scenario.

Clock: `Simulation::getOutputViewingTime()` (`src/Physics/Simulation.cpp:383`)
is `stepsElapsed / 24` — whole seconds of *video*, not wall clock. That is the
right clock for "after N seconds have been rendered".

## How to work this plan

ONLY DO 1 unchecked item, then stop and wait for a context reset.
- If the item is already done, tick it and stop.
- If it needs work, do it, verify it with the item's own check, tick it, stop.
- If it turns out to be bigger than one sitting, split it into smaller boxes
  in place and stop.

To reset: untick everything. To resume: do the first unticked box.

## Where things stand (verified 2026-08-03)

Working, uncommitted:
- `Simulation::addGroup(const ParticleList&)` declared `Simulation.h:58`,
  defined `Simulation.cpp:338`.
- Timed introduction block in `FullApplication::update()`,
  `src/FullApplication.cpp:110-146`: every 20 video seconds, up to 5, builds a
  hard-coded yellow `GroupSpec` and calls `addGroup`.

Broken / rough:
- **Does not compile.** `Simulation.cpp:340` calls `physicalObjects.addAll(...)`;
  `ParticleList` only has `addList(ParticleList)` (`ParticleList.h:34`).
- Interval, limit, and every group parameter are hard-coded in the frame loop.
- Function-level `static`s at `FullApplication.cpp:112-113` shadow an unused
  duplicate pair in the anonymous namespace at `FullApplication.cpp:23-25`.
- Arriving group is placed at a fixed `5e6` meters regardless of how large the
  scenario actually is, with no velocity — it just sits there.
- Engine-wide bookkeeping set once at t=0 (merge ramp, render scale, soft
  boundary) is unaware that particles can appear later. See phase 5.

---

## Phase 0 — get it building again

- [x] `Simulation.cpp:340`: `addAll` → `addList(newGroup)`.
      Check: `make build/./src/Physics/Simulation.cpp.o` succeeds.
- [x] Confirm the `refreshQuadrant` + `updateMinsAndMaxes` calls inside
      `addGroup` are right, and say why in a comment: `update()` rebuilds the
      octree at the *end* of a step, so a group added after that would exert no
      gravity for a frame without this. Check: comment reads true against
      `Simulation.cpp:222` (end of `update`).
- [x] Full build clean. Check: `make -j8` succeeds.

## Phase 1 — make the existing hack honest (no behavior change)

- [x] Delete the unused namespace-level statics, `FullApplication.cpp:23-25`.
      Check: builds; the function-level ones at `:112-113` are the live pair.
- [x] Delete the duplicated `newGroupSpec.position` assignment
      (`:122` is immediately overwritten by `:124`).
- [x] Move `nextGroupIntroduceTime` / `groupsIntroduced` from function statics
      to `FullApplication` members. Function statics persist across instances,
      so a second run in one process starts mid-schedule.
      Check: builds; a small run still adds groups at 20s / 40s.

      Verified: 7x400 chaotic, seed 7, 1010 frames. "Introducing new group" at
      step 480 (20s) and step 960 (40s), particle count 302 -> 802 and
      312 -> 812. The mechanism works. What happens next is phase 5.

## Phase 2 — extract the schedule, with tests

- [x] New `src/Physics/ArrivalSchedule.h/.cpp`: holds `firstAt`, `everySeconds`,
      `limit`, `arrivalsSoFar`; one method `bool due(second_t now)` that
      consumes a slot when it fires. No Simulation, graphics, or GL includes.
      **This is the single place the cadence is defined.** Changing "every 20
      seconds" to "every 5" is one edit in `ArrivalSchedule.h` and nowhere else.
- [x] Decide and encode the catch-up rule inside `due()`: if the clock jumps
      past several intervals, fire at most one arrival per call (paused frames
      and long steps shouldn't dump three groups in at once). Document it in the
      header. Slots do not bank - the schedule resumes from now.
- [x] `FullApplication` uses `ArrivalSchedule` instead of its own counters.
      Still 20s / 5 for now. Check: `make -j8`; a small run is unchanged.
- [x] `Test/ArrivalTests.cpp` (catch, same style as `Test/ScenarioTests.cpp`):
      first fire lands on `firstAt`; spacing equals `everySeconds`; never
      exceeds `limit`; a time jump fires once, not N times; `limit=0` and
      `everySeconds<=0` never fire. 7 cases, 25 assertions, all green.

      `make test` cannot link, and could not before this work: `TimeManagerTest.cpp`
      (committed in 05e12b9) defines its own `int main()`, which collides with
      catch's in `unitTests.cpp`. Verified with a standalone build instead:
      `g++ -std=c++17 -I Test -I src -o /tmp/at Test/unitTests.cpp
      Test/ArrivalTests.cpp src/Physics/ArrivalSchedule.cpp && /tmp/at "[arrivals]"`.
      Worth fixing separately - it is not arrivals work.

## Phase 3 — data-driven, from the properties file

- [ ] `struct ArrivalSpec` in `ScenarioSpec.h`: `everySeconds`, `firstAt`,
      `limit`, plus a `GroupSpec templateGroup` for what arrives. Add one
      `ArrivalSpec arrivals;` field to `ScenarioSpec`.
- [ ] Parse `arrivals.*` in `ScenarioParser::parse` using the existing
      `knob()` / `knobString()` helpers (`ScenarioSpec.cpp:78-89`) so every key
      lands in `spec.knobs` and therefore in the sidecar for free.
      Keys: `arrivals.every_seconds`, `arrivals.first_at`, `arrivals.limit`,
      `arrivals.count`, `arrivals.mass`, `arrivals.radius`,
      `arrivals.dispersion`, `arrivals.virial_ratio`.
- [ ] Default is off: `arrivals.limit=0` (or `every_seconds<=0`) means nothing
      arrives, so every existing config in `configs/` behaves exactly as before.
      Check: `--print-setup` on `configs/chaotic.properties` is byte-identical
      to before the change.
- [ ] Tests in `ScenarioTests.cpp`: defaults are off; values parse; the keys
      show up in `spec.knobs`.
- [ ] Thread the spec to the frame loop: `main.cpp:71` already has `scenario`;
      pass `scenario.arrivals` into the `FullApplication` constructor and build
      the `ArrivalSchedule` from it. Delete the hard-coded 20.0 / 5.
      Check: a config with `arrivals.every_seconds=5`, `arrivals.limit=3` adds
      groups at video seconds 5, 10, 15 and then stops.

## Phase 4 — place the arrival relative to the live run

Hard-coded `5e6` m only makes sense for one scenario size.

- [ ] Cache the centre of mass that `Simulation::updateMinsAndMaxes` already
      computes (`Simulation.cpp:88-96`) into a member, and add a getter. It is
      needed to aim an arrival at where the action currently is.
- [ ] Distance from the current centre of mass as `arrivals.distance_factor` ×
      the run's starting `diagnostics.systemRadius`, not absolute meters.
- [ ] Direction: random unit vector in the XY plane, from an RNG owned by the
      schedule and seeded off `scenario.seed`, so a seed still reproduces the
      whole run including its arrivals.
- [ ] Aim it inward: bulk velocity `arrivals.infall` × escape speed
      (`sqrt(2GM/r)` with the live total mass) toward the centre of mass.
      `infall=0` drops it in cold.
- [ ] Colour: rotate through the archetype palette (`paletteColor`,
      `ScenarioSpec.cpp:74`) instead of always yellow, so successive arrivals
      are distinguishable. It is in an anonymous namespace — expose it or move
      it somewhere shared.
- [ ] Build the arriving `ParticleList` by reusing `ScenarioBuilder::build` on a
      one-group `ScenarioSpec` with `virialRatio = -1` (no system-wide rescale;
      the group keeps the velocity it was given).
      Check: a 3-arrival small run, groups visibly enter frame and fall inward.

## Why arrivals get annihilated — measured 2026-08-03

Three causes, ranked by how much they actually matter. All measured on
7x400 chaotic, seed 7, arrivals at video seconds 20 and 40.

1. **The forced-merge floor, `minimum_merges_per_frame=10`.** Dominant by far.
   It picks the globally closest pairs, and a freshly arrived unmerged blob is
   the densest thing on a field of otherwise well-separated merged bodies - so
   the floor targets the newcomer almost exclusively, at 10 merges/frame = 240
   a second. A 400-particle group is gone in under two seconds. It re-arms
   because adding particles drops `progress` back below `merge_target_fraction`
   (`Simulation.cpp:181-186`), which had already switched it off at ~90% merged.

   Evidence: with `minimum_merges_per_frame=0` and nothing else changed, the
   arriving group survives intact - 866 particles at arrival, 846 forty frames
   later - then merges naturally as it falls in and meets the main body.

2. **Density mismatch.** The hard-coded group packed 500 particles into 5e5 m
   against the archetype's own 400 into 5.4e6 m: ~1580x the density, particles
   ~12x closer than the scenario's own. At any collision multiplier that is
   self-annihilating. Fixing this alone took the collapse from 4 frames to ~60.

3. **Collision multiplier level.** ~40 at the first arrival vs the 30 the
   original groups started at. Real, but small next to the other two.

Resolved, in the first box of phase 5: newcomers get a grace period from forced
merges, and the merge deadline is kicked back by however long the group was
held back, so arriving material is judged on its own runway rather than against
a schedule the original material has already been spending.

Still open: `ArrivalSchedule`'s defaults mean **every config currently gets
arrivals**, since nothing reads the properties file yet. Phase 3 makes them
opt-in, and that is what makes the change safe for existing scenarios.

## Phase 5 — bookkeeping the rest of the engine assumes is fixed at t=0

Each of these is set once during `Simulations::fromScenario` and quietly means
something different once particles can appear mid-run.

- [x] **Forced-merge grace period, and re-pacing the merge schedule.**
      `Particle::noteGroupArrived(count, step)` now, on every arrival:
      adds the newcomers to `initialParticleCount`; **kicks the merge deadline
      back by exactly how long the group was held back**
      (`mergeTargetSteps = currentStep + baseMergeTargetSteps`, so a group
      arriving at 20s gets the full configured window from its own
      introduction rather than inheriting one the original material has
      already spent 20 seconds against); resets the collision multiplier to its
      start value to ramp again; and clears the stall counter.

      Alongside it, arriving particles are stamped with their arrival step and
      skipped by the *forced*-merge loop for `Particle::mergeGraceSteps`
      (120 frames = 5s). Real collisions are untouched.

      Verified, merge floor left at its normal 10: arrival at step 480 holds at
      691 particles through step 540, versus being ground to ~40 within 60
      frames before. Deadline logged as 3360 = 480 + 2880, then 3840 = 960 +
      2880 for the second arrival. The group then merges on its own dynamics as
      it falls in - the same curve a run with the floor disabled entirely
      produces, which is the honest reference.
- [ ] Render scale: the 1px baseline is the lightest starting particle
      (`Simulations.cpp:75-77`). Arrival particles lighter than that draw
      sub-pixel. Either clamp the template's per-particle mass to the baseline
      or re-apply `setRenderScale` on arrival — pick one and note why.
- [ ] Soft boundary: radius and centre are frozen at t=0
      (`Simulations.cpp:48-54`). An arrival spawned outside it is pulled in by
      the boundary force, not by gravity. Either keep spawns inside the boundary
      radius or grow the boundary when a group arrives.
- [ ] Camera: `updateMinsAndMaxes` frames 95% of bound mass and drops anything
      above 2.5σ in velocity. Confirm an arrival doesn't cause a snap zoom-out;
      if it does, tune spawn distance rather than the camera.
      Check: watch one small recorded run end to end.

## Phase 6 — reporting and docs

- [ ] One log line per arrival: video time, label, count, mass, distance.
- [ ] Record arrival events in the sidecar via `RunReport` so a good run can be
      reproduced and a batch report can tell arrivals apart.
- [ ] `SCENARIOS.md`: an `arrivals.*` knob table next to the archetype tables.
- [ ] `configs/arrivals.properties`: a small worked example, 5s interval.

## Phase 7 — verify

Keep these small — a few thousand particles. Full size is for footage only.

- [ ] `--print-setup` on every config in `configs/` still prints what it did
      before (arrivals off by default).
- [ ] Bounded run with `arrivals.every_seconds=5`, `arrivals.limit=3`,
      `--max-frames 480`: particle count steps up at frames 120, 240, 360.
- [ ] `./build/Tests` all green.
- [ ] One full-size recorded run, last, once everything above is ticked.

## Design decisions

- Arrivals are part of the **scenario**, not a run flag: they change what the
  simulation *is*, so they belong in the properties file and the sidecar
  alongside the groups they extend.
- The schedule is a plain testable object with no simulation or GL dependency;
  `FullApplication::update()` only asks "is one due?".
- Arrivals are off unless asked for. No existing config changes behavior.
- Everything spatial is expressed relative to the run's own scale
  (system radius, centre of mass, escape speed), never in absolute meters.
