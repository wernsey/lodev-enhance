# Extending Lode's Raycaster

The code comes from Lode Vandevenne's tutorial [Raycasting III: Sprites][raycast3].

The purpose of systematically apply some enhancements and document them along the way.

The rest of the original tutorials are here [Lode's Computer Graphics Tutorial][cgtutor]

[cgtutor]: https://lodev.org/cgtutor/
[raycast3]: https://lodev.org/cgtutor/raycasting3.html

## Step 1

commit 71797ccd2dc8cd6d633d8ef4bb3a4e2ccc1cc242

* Fixed some compiler warnings
* Replaced the Wolfenstein graphics with CC0 graphics from Dungeon Crawl Stone Soup (DCSS)

The Dungeon Crawl Stone Soup graphics are from here:
<https://opengameart.org/content/dungeon-crawl-32x32-tiles>

# Step 2: Better vertical strips

Step 2 is to convert the code that draws the vertical strips to use integer only arithmetic

commit e0f5cbbc6e2bb3011b210c3a2a2a779c6896769a

