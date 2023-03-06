# Extending Lode's Raycaster

The code comes from Lode Vandevenne's tutorial [Raycasting III: Sprites][raycast3].

The purpose of systematically apply some enhancements and document them along the way.

The rest of the original tutorials are here [Lode's Computer Graphics Tutorial][cgtutor]

[cgtutor]: https://lodev.org/cgtutor/
[raycast3]: https://lodev.org/cgtutor/raycasting3.html

## Step 1

* Fixed som compiler warnings
* Replaced the Wolfenstein graphics with CC0 graphics from Dungeon Crawl Stone Soup (DCSS)

The Dungeon Crawl Stone Soup graphics are from here:
<https://opengameart.org/content/dungeon-crawl-32x32-tiles>

# Step 2: Better vertical strips

## Step 2a

commit e0f5cbbc6e2bb3011b210c3a2a2a779c6896769a

Convert the code that draws the vertical strips to use integer only arithmetic

## Step 2b

commit 8039f41cc5cb93458bb6b80109ac77edf3ab47be

Then you can use the same trick to draw the vertical strips of the sprites.

## Step 2c

commit 34993e41b08c4ec51b0b936ae5d99afa2d5f9ac5

While you're at it, you can do the same for the horizontal part of the sprite loop.

# Step 3: Skybox
