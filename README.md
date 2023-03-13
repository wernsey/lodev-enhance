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

commit c1d6246654886222c54d7fccdcbda7f228669b99

# Step 4: Fog effects

commit bb0c927871fbf0d17798a698c4c453b262d15436

# Step 5: Doors

commit f5980c0319457d9fc188d3ac40d5f0643f586973

# Step 6: Look up and down

commit 0ef7d398abb09a70e6952e61d83cc6d2b70f29b7

# Step 7: Jump and crouch

commit 61224aa5a8c08def242fb696b6947d0d12ffd35b

# Step 8: Walls with Transparency

commit 1940b5c13ca01b096249729e97733f27571d1c9f

# Step 9: Glass windows