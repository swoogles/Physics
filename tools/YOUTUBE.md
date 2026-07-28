Uploading clips to YouTube
==========================

`tools/youtube-upload` cuts a clip out of a recorded run and uploads it, taking
the title and description from the `.json` report sitting next to the video.

    tools/youtube-upload WorthyVideos/2026-07-27_1157_chaotic-chaotic-01-full.mp4 60s

Add `--dry-run` to see exactly what would be posted without touching Google.
That works before any of the setup below, so it is the fastest way to iterate on
how the descriptions read.


One-time setup
--------------

Everything here happens once, in a browser, and takes about five minutes.

1. **Make a Google Cloud project.** https://console.cloud.google.com/projectcreate
   Name it whatever you like - it is only a container for the API credential.

2. **Enable the YouTube Data API v3.** In that project: APIs & Services ->
   Library -> "YouTube Data API v3" -> Enable.

3. **Fill in the OAuth consent screen.** APIs & Services -> OAuth consent screen.
   Pick *External*, give it an app name and your own email, and add your Google
   account under *Test users*.

   Then set the publishing status to **In production**. Left in *Testing*, Google
   expires the refresh token after seven days and you re-authorise constantly.
   Going to production does not require a review for personal use - you will see
   an "unverified app" warning the first time you authorise, and *Advanced ->
   Continue* gets past it.

4. **Create the credential.** APIs & Services -> Credentials -> Create
   credentials -> OAuth client ID -> **Desktop app**. Download the JSON.

5. **Drop it where the script looks:**

        mkdir -p ~/.config/physics-youtube
        mv ~/Downloads/client_secret_*.json ~/.config/physics-youtube/client_secret.json

   (Or point `YOUTUBE_CLIENT_SECRET` at it instead. Either way it stays out of
   this repo - it is a credential.)

The first real upload opens a browser, asks which channel to authorise, and
writes `~/.config/physics-youtube/token.json`. Nothing after that is
interactive. If the token ever goes stale, delete it and the next run
re-authorises.

The script keeps its Python dependencies in `tools/.venv-youtube`, which it
builds by itself on first use. Deleting that directory is always safe.


Two limits worth knowing before you plan a posting schedule
-----------------------------------------------------------

**Six uploads a day.** A new API project gets 10,000 quota units per day and
`videos.insert` costs 1,600 of them. The seventh upload fails until midnight
Pacific. Uploading by hand through YouTube Studio does not touch this quota, so
that stays available for a batch day. More quota is a request form, not a
setting.

**Uploads start private, and stay private until an audit.** YouTube restricts
videos uploaded by an API project that has not passed its compliance audit, so
`--public` will not actually make one public. Two ways round it: request the
audit (https://support.google.com/youtube/contact/yt_api_form), or leave the
upload private and flip it to public yourself in Studio - the second link the
script prints goes straight to that video's edit page.

Given that, `--unlisted` is the useful one day to day: it shares fine, it just
does not appear on the channel.


When is it actually watchable?
------------------------------

The upload call returns a link the moment YouTube has the bytes, which is why
the first thing you see there is "still being processed" - transcoding is a
separate job that has not started yet. So the script waits for it and says when
it is done:

      processing  38%, about 2m left
      processed - watchable now, though the highest resolution can
      keep transcoding for a while after this

YouTube publishes the low resolutions first, so a 2.5K clip usually plays at
360p minutes before the HD rendition exists. "Processed" means watchable, not
watchable in full quality - if the clip looks soft right after this, that is
the HD pass still running, and Studio shows it as "HD is processing".

`--no-wait` skips all of that and just prints the link. To check later, or to
check something you uploaded earlier:

    tools/youtube-upload --status 81HrUpoqQLY
    tools/youtube-upload --status https://youtu.be/81HrUpoqQLY

That takes a bare id or any YouTube link, prints processing state, upload state
and privacy, then waits if it is still going. Each poll costs 1 quota unit
against the same 10,000/day - nothing next to the 1,600 an upload costs.

A rejected upload (duplicate, a copyright claim) shows up here too, as
`rejected` with the reason, rather than as a video that never appears.


Usage
-----

    tools/youtube-upload <video> [duration] [options]

`duration` accepts `60s`, `90`, `1m30s` or `1:30`. Leave it off to upload the
whole video.

    --start 0:30        where the clip begins (default: the beginning)
    --dry-run           print the metadata and stop
    --no-wait           do not wait for processing
    --status ID|URL     check an earlier upload instead of making a new one
    --unlisted          upload unlisted instead of private
    --publish-at        RFC3339 time to go public, e.g. 2026-08-01T17:00:00Z
    --playlist NAME     add it to an existing playlist, by title or id
    --title / --description / --tags    override what the report derives
    --reencode          frame-accurate cut instead of a stream copy
    --keep-clip         leave the trimmed file next to the source

Cuts are stream copies, so no quality is lost, but the start snaps to the
nearest keyframe before `--start`. That is invisible from `0:00`; use
`--reencode` if a mid-run cut needs to land on an exact frame.

`WorthyVideos/latest.mp4` has no report of its own, so the script matches it
against the run reports beside it and uses the metadata of whichever run wrote
an identical file.
