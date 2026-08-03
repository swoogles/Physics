# Scenarios, batching, and how to get good footage

Starting arrangements used to live in `Simulations.cpp`, selected by commenting
lines in and out and waiting on a rebuild. They now live in properties files:
new arrangements are new text files, which means a script can write a hundred
of them and run them all while you're asleep.

## The short version

```bash
make -j8

# One run, recorded, from a config
./build/PhysicsSandbox --config configs/cold-collapse.properties --record

# What would this config actually do? (no window, no run)
./build/PhysicsSandbox --config configs/pair-grazing.properties --print-setup

# Sample 12 variations overnight, then look at what scored well
tools/run_batch.py sample --archetype pair --runs 12
tools/run_batch.py report batches/<batch> --contact-sheet
tools/run_batch.py render batches/<batch> --top 3 --res 1440p
```

Every recorded run writes `<video>.json` next to the video: the seed, the
resolved scenario, the starting diagnostics and a coarse history. That file is
enough to reproduce the run exactly, so a good one is never lost.

## Command line

| flag | meaning |
|---|---|
| `--config <path>` | properties file to run (default `simulation.properties`) |
| `--out <path>` | output `.mp4` (default `./WorthyVideos/<timestamp>.mp4`) |
| `--seed <n>` | RNG seed; omitted means "pick one and record it" |
| `--record` / `--no-record` | record video (legacy `r` / `x` still work) |
| `--res 720p\|1080p\|1440p\|4k` | frame size, or `--width` / `--height` |
| `--max-frames <n>` | stop after n frames, so every run is the same length |
| `--max-seconds <n>` | wall-clock cap, overriding `maximum_runtime` |
| `--print-setup` | print the scenario diagnostics as JSON and exit |
| `--no-sidecar` | skip the `.json` run report |

Ctrl-C finalizes the video and writes the report instead of throwing the run
away.

## Choosing a scenario

`scenario.type` picks the shape of the starting arrangement.

### `cluster` — one cloud

The workhorse. One sphere of particles that collapses under its own gravity.

| knob | meaning |
|---|---|
| `cluster.radius` | radius of the sphere, in meters |
| `cluster.count` | particles |
| `cluster.mass` | total mass, kg |
| `cluster.spin` | rotation, as a fraction of break-up spin. Use an in-plane axis (`1,0,0`) to see the flattening |
| `cluster.dispersion` | random internal motion, as a fraction of virial velocity |

### `pair` — two clouds meeting

| knob | meaning |
|---|---|
| `pair.separation` | centre-to-centre starting distance, meters |
| `pair.impact_parameter` | perpendicular offset, as a fraction of the two radii summed. **0 is head-on; 0.4–0.9 is where tidal tails come from** |
| `pair.approach_speed` | closing speed, as a fraction of mutual escape speed. Below 1 stays bound, 1 is parabolic, above 1 is a flyby |
| `pair.mass_ratio` | heavier over lighter. 1 gives symmetric mergers; 5–10 gives stripping and accretion |
| `pair.spin_a`, `pair.spin_b` | per-group spin; opposite signs make a mess, matched signs make a disk |

### `chaotic` — N groups scattered and falling in

The one to sample when you want variety, because the placement itself depends
on the seed.

| knob | meaning |
|---|---|
| `chaotic.groups` | how many |
| `chaotic.spread` | radius they're scattered through |
| `chaotic.size_fraction` | each group's radius, as a fraction of the spread |
| `chaotic.infall` | inward speed, as a fraction of escape speed |
| `chaotic.tangential` | sideways speed; this is what gives the whole system spin |
| `chaotic.spin_max` | per-group spin ceiling |
| `chaotic.mass_spread` | how unequal the groups are |
| `chaotic.thickness` | how far out of the XY plane they're scattered |
| `chaotic.group_virial_min`, `chaotic.group_virial_max` | the band each group's own temperature is drawn from |

Those last two are what stop every group behaving the same way. Each group draws
its own kinetic-to-binding energy ratio log-uniformly from the band, and that
decides what it does before anything reaches it: around 0.5 it holds itself up
as a cloud and only moves when a neighbour arrives, by 0.02 it collapses on its
own. Measured on one isolated group over 240 frames, same seed, the difference
is a 29% contraction at 0.02 against 4% at 0.6. Leave both at 0 and every group
falls back to sharing `chaotic.dispersion`, which is how this behaved before.

Two things worth knowing when reading the results back:

`scenario.virial_ratio` is the *bulk* target - it scales how the groups move as
groups, and per-group temperatures are applied afterwards and deliberately not
rescaled with it, since a group's ratio is a statement about that group against
its own gravity. So `virial_ratio_after` in the report sits above the target by
whatever the internal motion adds, and that is the honest total rather than a
miss. Groups using the older `dispersion` knob are still placed hot and still
take part in the rescale, so every scenario written before this builds exactly
as it did.

Merge counts will not show you the difference. `minimum_merges_per_frame` forces
merges on a schedule whatever the physics is doing, so a stable cloud sheds
particles at about the same rate as a collapsing one - in that same experiment,
306 merges against 299. RMS radius is the measure that actually separates them.

### `diamond` — a ring swinging past the centre

The parameterized version of the old hard-coded `fourInADiamond()`.
`diamond.groups`, `diamond.ring_radius`, `diamond.infall`, `diamond.swirl`,
`diamond.spin`.

### `explicit` — exact arrangements

```
group.0=label=core; pos=0,0,0; n=12000; mass=3.0e9; radius=1.2e7; spin=0.5; dispersion=0.2; color=1,1,1
group.1=pos=-3.0e7,0,0; n=5000; mass=9.0e8; radius=6.0e6; spin=0.3
```

`pos` and `vel` are meters and m/s. Any field left out falls back to
`numShapes`, `mass` and `sandbox_width`. A scalar `spin` means rotation about
the z axis; give it as a vector (`spin=1,0,0`) to tilt the rotation axis. See
"Mind the axis" below for which one you want.

## The two knobs that matter most

**`scenario.virial_ratio`** is the ratio of kinetic to gravitational energy at
t=0, measured about the centre of mass. After the arrangement is built, every
velocity is rescaled to hit it, so it works the same whatever the scenario.

| value | what you get |
|---|---|
| 0.1–0.3 | cold collapse: violent infall, a bounce, re-expansion. Most of the good footage |
| ~0.5 | virial equilibrium: slow, stately, not much happens |
| > 1.0 | unbound: everything flies apart and the frame zooms out forever |

Set it negative to leave the archetype's own velocities alone.

**`spin`**, on any group, is a fraction of that group's break-up spin
(`sqrt(GM/R³)`), so 1.0 is centrifugally supported at the edge. Zero spin
collapses to a featureless blob; add spin and the collapse lands in a plane
instead. If your runs all look like fuzzy balls, this is the knob you're
missing.

**Mind the axis.** The camera looks down the z axis, so a bare scalar
(`spin=0.6`, meaning rotation about z) flattens the cloud *in the plane of the
screen* — a face-on disk, which still reads as a circle. To see the flattening,
put the spin axis in the view plane: `cluster.spin=1,0,0` gives an edge-on disk,
a bright equatorial band with plumes thrown off the poles. Group spins in `pair`
and `diamond` are about z on purpose, because there the structure worth seeing
(tails, arms) lies in the orbital plane, which is already facing you.

## Pacing: `dt=auto`

`dt=auto` sets the timestep from the scenario's own crossing time
(`frames_per_crossing`, default 400). Without it, doubling the size of an
arrangement silently makes the video three times slower, and every scenario
needs its dt retuned by hand. `--print-setup` reports
`video_seconds_per_crossing`: if that's in the thousands, nothing will appear
to move.

## Reading the diagnostics

```
./build/PhysicsSandbox --config configs/chaotic.properties --print-setup
```

| field | what to look for |
|---|---|
| `virial_ratio_after` | should match your target; this is what actually got built |
| `system_radius_m` | how big the thing is; the soft boundary sits at 1.5x this |
| `video_seconds_per_crossing` | pacing. 10–30 is watchable |
| `velocity_scale` | how hard the virial rescale had to push. Wildly far from 1 means the archetype's speeds disagree with your target |

## Batching

```bash
tools/run_batch.py sample --archetype chaotic --runs 16 --particles 1500 --frames 900
```

Each run gets a sampled parameter set, a seed, a 720p preview, and a report.
The sampler ranges encode where the interesting behaviour lives — they are the
part worth editing as you learn what you like.

Runs are scored from their own reports, on four questions:

- **collapse** — did it ever fall together, or just sit there?
- **bounded** — did it stay in frame, or spray off into nothing?
- **structure** — did a dominant body actually build up?
- **liveliness** — did anything change over the run?

Scores rank the batch in `summary.md`; empty or missing videos score zero, so
crashed runs can't masquerade as successes.

### Vetting a batch

The score is a proxy for "something happened," not for "this looks good" — it
never sees a pixel. Sorting by it is a first pass; your eye makes the call.

A `sample` run builds `contact-sheet.png` automatically: one filmstrip per run,
five frames spanning the whole thing, ranked best first and captioned with the
name, score and seed. One picture tells you which runs collapsed, which drifted
apart, and which never moved. Rebuild it any time, with more frames per row:

```bash
tools/run_batch.py report batches/<batch> --contact-sheet --strip 8
```

When the sheet isn't enough, watch them. `--reel` stitches every preview into
one labelled `reel.mp4`, ranked best first, so twelve previews are one file
instead of twelve:

```bash
tools/run_batch.py report batches/<batch> --reel
open batches/<batch>/reel.mp4
```

Then trim by name. `--exclude` drops runs before `--top` is applied; `--only`
renders exactly what you name and ignores the score entirely. `--dry-run`
prints the selection without rendering anything:

```bash
tools/run_batch.py render batches/<batch> --exclude chaotic-02,chaotic-09 --dry-run
tools/run_batch.py render batches/<batch> --only chaotic-05,chaotic-11 --res 1440p
```

Then re-render the survivors at full resolution:

```bash
tools/run_batch.py render batches/<batch> --top 3 --res 1440p --frames 2880
```

Same seed and same config means the same run, frame for frame.

### Re-rendering with more particles

`--particles` re-renders at higher fidelity. Group mass, position, velocity,
spin and radius are per-group quantities, so the gravitational setup is
unchanged — verified by `--print-setup` at 1500 vs 6000 particles per group:
identical total mass and virial ratio, system radius and crossing time within
0.3%. The bulk motion (collapse, infall, tails, orbits) holds up.

What does not carry over on its own:

- **The exact run.** A different particle count draws differently from the RNG,
  so you get the same setup with different particles. Statistically the same,
  not frame-for-frame the same.
- **Merge pacing.** `minimum_merges_per_frame` is an absolute count, so the
  same value eats a smaller fraction of a bigger population each frame.
- **Collision cross-section.** Each particle carries less mass, so its radius
  shrinks as N^(-1/3) — 4x the particles makes each one 1.59x smaller.
- **Graininess.** Low-particle previews have a lumpier potential. Small-scale
  clumping you liked in a preview may smooth out at higher resolution.

`--particles` compensates for the middle two by default, scaling
`minimum_merges_per_frame` linearly and `collision_radius_multiplier` by
N^(1/3). Pass `--no-scale-merges` to leave them alone. The collision multiplier
is also under a feedback controller that chases `merge_target_fraction` on a
step deadline, so it self-corrects to a degree either way.

## Why big bodies need help to be visible

Radius goes as the cube root of mass, so a body holding a fifth of the entire
system is only ~17x the radius of a single particle — about **0.03 pixels** on
a 1280-wide frame. The `glutSolidSphere` drawn at true physical scale is
invisible for every particle in every run; what you actually see is the
`GL_POINTS` dot next to it.

That dot used to be one pixel for every particle regardless of mass, which
made a heavily merged core look like it had emptied out and the biggest body
look like it had disappeared. Point size now follows the cube root of mass
relative to a single unmerged particle, so 1000 merged particles draw 10x
wider. `max_point_size` (default 14) caps it; set it to 1 for the old uniform
look. The points are drawn smoothed, so bodies read as round dots rather than
the aliased squares GL_POINTS gives by default.

Mass itself was never the problem — it is conserved exactly through merging,
which the run report lets you confirm: compare `setup.total_mass_kg` against
`final.total_mass_kg`, and watch `largest_mass_fraction` in the history, which
only ever climbs.

## Recording pitfalls

**A window cannot be larger than your display.** Asking for `--res 1440p` on a
smaller screen gets you whatever the window manager allowed - one run here
recorded at 1728x1053 and another at 2336x1428 from the same `1440p` request.
The run now prints a warning when it is clamped. Genuinely rendering above
screen resolution needs offscreen rendering, which doesn't exist yet.

**h264 cannot encode odd dimensions.** With `-pix_fmt yuv420p`, chroma is
subsampled 2x2, so an odd width or height makes ffmpeg exit without writing a
packet and leave a 0-byte file. Window sizes are whatever the window manager
hands back, so an odd one is a coin flip - a 1728x1053 window silently threw
away a 15-minute render. Capture dimensions are now rounded down to even.

The batch runner verifies every video before starting the next run and stops
the batch if one is empty, unreadable, or zero-length, printing the tail of
that run's log. Each run writes `<video>.log` next to its output; the runner
used to discard that output entirely, which is what hid the failure.

## Camera smoothing

The bounds the camera follows are recomputed from scratch every frame as the
extrema of a set whose membership keeps changing - particles cross the 2.5-sigma
velocity cut, and the 95%-of-mass cutoff moves as merging redistributes mass.
The camera used to be set straight from that, so it stuttered.

It now holds still until the framing is off by more than `camera_deadband`,
then eases the whole way in (`camera_zoom_in_rate`) or out
(`camera_zoom_out_rate`) until within `camera_settle`. Zoom-in is deliberately
about 5x slower than zoom-out: being a little too far back is much cheaper than
losing the action off the edge of frame, and it stops the camera chasing a
collapse inward.

Measured over 600 frames:

| | zoom range | frames moving | reversals | worst single frame |
|---|---|---|---|---|
| chaotic, before | 6.03x | 100% | 29 | 6.86% |
| chaotic, after | 3.79x | 52% | 6 | 1.12% |
| collapse, before | 1.30x | 79% | 40 | 0.75% |
| collapse, after | 1.10x | 7% | 0 | 0.22% |

`PHYSICS_LOG_ZOOM=1` dumps the camera distance per frame on stderr, which is
how those numbers were taken.

Still outstanding: the camera always centres on the origin rather than the
system's centre of mass, so a scenario whose mass ends up off-centre - `chaotic`
often does - is framed wider than it needs to be.

## Merge jumps

A merged body is placed at the centre of mass of the two originals, so the
heavier one shifts by `distance * lighterMass / totalMass`. Two things let that
distance grow: `calcRadius` scales radius as sqrt(mass) rather than the cube
root (so radii are 18-73x too large and get worse as bodies grow), and the
collision multiplier ramps without limit while merging runs behind schedule -
it was observed passing 100 in a 25k-particle run.

`max_collision_radius_multiplier` (default 120) bounds the ramp.
`max_merge_jump_fraction` (default 0.02 of the starting system radius) bounds
the resulting shift. The allowance scales inversely with how large the body is
drawn, because a speck hopping goes unnoticed while a fat dot doing it is
jarring - a flat cap on separation instead throttles early coalescing badly
(merges fell from 5424 to 455 in one test).

Run reports carry `largest_merge_jump_m` and `largest_visible_merge_jump_m`
(bodies drawn 4px or larger), so you can tell whether a run actually suffered
from this.

The underlying `calcRadius` bug is untouched - fixing it shrinks every radius
by 18-73x and would need every config's collision multiplier retuned.

## Merging

Real collisions almost never happen at these sizes — particles are tens of
meters across in a box millions of meters wide — so merging is driven by
`collision_radius_multiplier` and by `minimum_merges_per_frame`, which force
the closest pairs together every step. If you want structure to form on screen
rather than at astronomical timescales, those two are the levers.
