# Group arrivals

## Goal

New groups fall into a run that is already in progress, driven from a properties
file like every other part of a scenario.

**A group arrives when merging has made room for it**, not on a timer. When the
population has fallen far enough below what the run started with, a group is
built to fit the gap exactly. The rate is therefore never configured: a run that
merges quickly refills quickly, one that merges slowly refills slowly, and the
particle count - the thing that costs time - can never climb above where it
began. Nothing to tune, and no way to bog the run down by adding too much.

An earlier version of this plan fired arrivals on a fixed interval. That worked,
but it made the cadence a number someone had to guess, and guessing high meant a
run that ground to a halt. The population rule replaced it.

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

- [x] New `src/Physics/ArrivalSchedule.h/.cpp`, holding the population rule.
      No Simulation, graphics, or GL includes. **This is the single place
      arrivals are governed**, and it now has only two knobs:
      `minimumDeficitFraction` (how far the population must fall before a group
      is worth adding, default 0.15) and `limit` (a safety net, 0 = off).
      `roomFor(currentCount)` returns the exact shortfall, or 0 when the gap
      isn't yet worth filling - and that shortfall is also the size the arriving
      group gets built to, so an arrival can never overshoot the target.
- [x] `Simulation::particleCount()` - `O(1)`, unlike `getStats()`, because the
      trigger is checked every frame while `getStats()` walks every particle
      twice. `getStats()` is now only called when a group is actually arriving.
- [x] `FullApplication` sets the target population from the count the run
      started with, and asks `roomFor()` each frame.
- [x] `Test/ArrivalTests.cpp` (catch, same style as `Test/ScenarioTests.cpp`):
      a full run has no room; small gaps are ignored; the gap is filled exactly;
      a faster-merging run gets groups sooner; the limit is off by default and
      enforced when set; nothing arrives before a target is set; and a 5000-frame
      simulated run stays under its starting population throughout.
      7 cases, 522 assertions, all green.

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
      Keys: `arrivals.enabled`, `arrivals.minimum_deficit_fraction`,
      `arrivals.limit`, `arrivals.infall`, `arrivals.tangential`,
      `arrivals.shell_min`, `arrivals.shell_max`, `arrivals.grace_seconds`.
      The last five are currently constants at the top of
      `ScenarioBuilder.cpp` and in `Particle.cpp`.
- [ ] Default is off: `arrivals.enabled=false` means nothing arrives, so every
      existing config in `configs/` behaves exactly as before.
      Check: `--print-setup` on `configs/chaotic.properties` is byte-identical
      to before the change.
- [ ] Tests in `ScenarioTests.cpp`: defaults are off; values parse; the keys
      show up in `spec.knobs`.
- [x] Thread the spec to the frame loop: `main.cpp` passes the whole
      `ScenarioSpec` into `FullApplication`, which keeps it to build arrivals in
      its likeness. What remains is reading `spec.arrivals` from the properties
      file rather than using the defaults in `ArrivalSchedule.h`.

## The population rule, measured

7x400 chaotic, seed 7, 1200 frames, `minimumDeficitFraction=0.15`:

- Particle count stayed between **2737 and 3218** for the whole run, against a
  starting 3218. It never once exceeded where it began - the ceiling holds.
- 22 arrivals, sized 187 to 488 particles, each built to fit the gap.
- The rate self-regulated exactly as intended: arrivals at 2s, 3s, 4s, 4s, 5s
  while the run was merging hard, thinning to every 2-4 seconds by 45s as
  merging slowed. Nothing sets that rate; it falls out of the merging.

**The one thing that is not bounded is mass.** Population is capped, but every
arrival adds material: 4.83e9 kg at t=0 became 1.17e10 by 25 seconds, 2.4x, and
it keeps climbing. Consequences, in rough order of how much they matter:

- Gravity strengthens as the run goes on, so it collapses faster and faster.
  Possibly good footage; it is not the physics the scenario was tuned for.
- The soft boundary is still the one computed at t=0 (open box in phase 5), and
  it is now holding in several times the mass it was sized for.
- Per-particle mass is deliberately preserved when an arrival is trimmed to fit,
  so the render scale set at t=0 stays correct. That one is fine.

If a run should conserve its mass budget, the rule to add is a cap on total mass
introduced, which would stop arrivals once spent - a knob for phase 3.

## Phase 4 — place the arrival relative to the live run

All of this now lives in `ScenarioBuilder::arrivalGroup`.

- [x] Expose the centre of mass. `getStats()` already computed it and threw it
      away (`Simulation.cpp:349-380`); it is now a field on `SimulationStats`,
      so an arrival can be aimed at where the action currently is.
- [x] Distance as a random 1.1-1.4x the scenario's *own* spread, measured from
      `spec.groups` rather than read from a knob - so it works for every
      archetype including `explicit`. Always outside the original spread, and
      measured from the live centre of mass rather than the origin.
- [x] Direction: random angle, flattened by the same ratio the scenario's own
      groups are, from an RNG seeded off `scenario.seed`.
- [x] Inward at 0.5x escape speed against the *live* total mass, plus a 0.35x
      tangential kick with the same 85%-prograde bias the archetypes use, so an
      arrival swings through instead of dropping down the middle.
- [x] Colour continues the archetype palette from where the original groups
      left off. `paletteColor` moved out of the anonymous namespace in
      `ScenarioSpec.cpp` and is declared in `ScenarioSpec.h`.
- [x] Built via `ScenarioBuilder::build` on a one-group spec with
      `virialRatio = -1`, so the group keeps the approach velocity it was given.
- [x] **Size, mass, count and temperature are drawn from a randomly chosen
      group of the original scenario**, so an arrival is the same kind of object
      the run is already made of. This is what makes them as varied as the
      originals rather than five copies of one hard-coded blob.

      Verified across seeds and archetypes. Seed 7 chaotic: arrivals of 891,
      187, 258, 242 and 925 particles from five different directions. Seed 42:
      a completely different sequence. `pair` behaves the same way.

      Note for `cluster` scenarios (one group, e.g. `cold-collapse`): an arrival
      is necessarily a clone of that single group, so `cluster.count=25000`
      means each arrival adds 25000 particles. Correct by the rule above, but
      expensive - the `arrivals.count` / `arrivals.mass` knobs in phase 3 are
      what let a run scale that down.

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

## A run is not reproducible from its seed — and was not before this work

`main.cpp:63` says a run is reproducible from its seed. It is not. Two runs at
seed 7, with no arrivals yet in play, agree at step 20 and step 50 and have
diverged by step 99 (1862 vs 1855 particles). Almost certainly ordering in the
OpenMP force and merge accumulation.

Arrivals inherit exactly that and no more: which group arrives, how big it is,
and which direction it comes from are all seed-stable (891, 187, 258 particles
in both runs), because they are drawn from an RNG seeded off `scenario.seed`.
Only the aim point drifts, since it is measured from the live centre of mass.

Worth fixing on its own merits - the batch workflow's whole premise is that a
good run can be reproduced from its sidecar - but it is not arrivals work.

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
- [ ] Bounded run: particle count stays between the deficit threshold and the
      starting count for the whole run, and arrivals thin out as merging slows.
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
