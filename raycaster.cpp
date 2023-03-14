/*
Copyright (c) 2004-2020, Lode Vandevenne

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmath>
#include <string>
#include <vector>
#include <stack>
#include <iostream>

#include "quickcg.h"
using namespace QuickCG;

/*
g++ *.cpp -lSDL -O3 -W -Wall -ansi -pedantic
g++ *.cpp -lSDL
*/

#define screenWidth 640
#define screenHeight 480
#define texWidth 32 // must be power of two
#define texHeight 32 // must be power of two
#define mapWidth 24
#define mapHeight 24

#define SKYBOX 1
#define SKYBOX_WIDTH    320
#define SKYBOX_HEIGHT   200
#define SKYBOX_REPEATS  4

#define FOG_LEVEL 2
#define FOG_COLOR 0xA09EE7
#define FOG_CONSTANT ((mapWidth + mapHeight)/2)

int worldMap[mapWidth][mapHeight] =
{
  {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
  {8, 0, 0, 0, 0, 0, 0, 0,12, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
  {8, 0, 3, 3, 0, 0, 0, 0, 8, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
  {8, 0, 0, 3, 0, 0, 0, 0,10, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
  {8, 0, 3, 3, 0, 0, 0, 0, 8, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
  {8, 0, 0, 0, 0, 0, 0, 0,11, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
  {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
  {7, 7, 7, 7, 9, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
  {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
  {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
  {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
  {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6,10, 6, 6, 6},
  {7, 7, 7, 7, 9, 7, 7, 7, 7, 8, 8, 4,12, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
  {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
  {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
  {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
  {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2,12, 6, 0, 0, 5, 0, 5, 0, 5},
  {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 5, 0, 0, 5, 5},
  {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5,12, 5,12, 5, 0, 5, 0, 5},
  {1,12, 2, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 5},
  {2, 0,11, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5,12, 5,12, 5, 0, 5, 0, 5},
  {2, 2, 1, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
  {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5}
};

struct Sprite
{
  double x;
  double y;
  int texture;
};

#define numSprites 23

Sprite sprite[numSprites] =
{
  {20.5, 10.5, 22},
  {20.5, 11.5, 22},
  {18.5,4.5, 22},
  {10.0,4.5, 22},
  {10.0,12.5,22},
  {3.5, 6.5, 22},
  {3.5, 20.5,22},
  {3.5, 14.5,22},
  {14.5,20.5,22},
  {17.5,18.5,22},

  {18.5, 10.5, 21},
  {18.5, 11.5, 21},
  {18.5, 12.5, 21},
  { 1.5,  9.5, 21},
  { 5.5,  9.5, 21},

  {21.5, 1.5, 20},
  {15.5, 1.5, 20},
  {16.0, 1.8, 20},
  {16.2, 1.2, 20},
  {3.5,  2.5, 20},
  {9.5, 15.5, 20},
  {10.0, 15.1,20},
  {10.5, 15.8,20},
};

Uint32 buffer[screenHeight][screenWidth]; // y-coordinate first because it works per scanline

double posX = 22.0, posY = 11.5; //x and y start position
double dirX = -1.0, dirY = 0.0; //initial direction vector
double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

std::vector<Uint32> texture[23];

double lookVert = 0;

double eyePos = 0;

// lookVert should have values between -LOOK_UP_MAX and LOOK_UP_MAX
#define LOOK_UP_MAX 128

//function used to sort the sprites
void sortSprites(int* order, double* dist, int amount);

#define MAX_DOORS 20
enum DoorState {closed, opening, open, closing};
struct Door {
  int x, y;
  int counter;
  DoorState state;
} doors[MAX_DOORS];
int numDoors = 0;

void processDoors() {
  numDoors = 0;
  for(int x = 0; x < mapWidth; x++) {
    for(int y = 0; y < mapHeight; y++) {
      if(worldMap[x][y] == 9) {
        if(numDoors == MAX_DOORS) {
          std::cout << "Too many doors!" << std::endl;
          return;
        }
        Door *door = &doors[numDoors++];
        door->x = x;
        door->y = y;
        door->state = closed;
        door->counter = texWidth;
      }
    }
  }
}

Door *findDoor(int x, int y) {
  for(int i = 0; i < numDoors; i++) {
    Door *door = &doors[i];
    if(door->x == x && door->y == y) {
      return door;
    }
  }
  return NULL;
}

void updateDoors() {
  for(int i = 0; i < numDoors; i++) {
    Door *door = &doors[i];
    switch(door->state) {
      case opening:
        if(--door->counter == texWidth/4) {
          door->state = open;
        }
      break;
      case closing:
        if(++door->counter == texWidth) {
          door->state = closed;
        }
      break;
      default: break;
    }
  }
}

#if FOG_LEVEL
Uint32 color_lerp(Uint32 color1, Uint32 color2, double t) {

  if(t < 0)
    return color1;
  if(t >= 1)
    return color2;

  Uint32 r1 = (color1 >> 16) & 0xFF;
  Uint32 r2 = (color2 >> 16) & 0xFF;

  Uint32 g1 = (color1 >> 8) & 0xFF;
  Uint32 g2 = (color2 >> 8) & 0xFF;

  Uint32 b1 = (color1) & 0xFF;
  Uint32 b2 = (color2) & 0xFF;

  r1 = (1-t) * r1 + t * r2;
  if(r1 > 0xFF) r1 = 0xFF;

  g1 = (1-t) * g1 + t * g2;
  if(g1 > 0xFF) g1 = 0xFF;

  b1 = (1-t) * b1 + t * b2;
  if(b1 > 0xFF) b1 = 0xFF;

  return (r1 << 16) + (g1 << 8) + (b1);
}
#endif

bool canPass(int x, int y) {
  if(worldMap[x][y] == 9) {
    Door *door = findDoor(x, y);
    return (door->state == open);
  } else if(worldMap[x][y] == 10 || worldMap[x][y] == 13)
    return true;
  return worldMap[x][y] == 0;
}

struct Strip {
  int x;
  int drawStart, drawEnd;
  double perpWallDist;
  std::vector<Uint32> &texture;
  int texX;
  double fog;
  int side;
  bool seeThrough;
};

void drawStrip(Strip &strip) {
  // Draw the vertical strip
  int texY = 0, c = 0;
  int dy = strip.drawEnd - strip.drawStart;

  if(strip.drawStart < 0) {
      c = -strip.drawStart * texHeight;
      if(c > dy) {
          div_t res = div(c, dy);
          texY += res.quot;
          c = res.rem;
      }
      strip.drawStart = 0;
  }
  if(strip.drawEnd >= h)
    strip.drawEnd = (h-1);

  for(int y = strip.drawStart; y <= strip.drawEnd; y++) {

    Uint32 color = strip.texture[texHeight * texY + strip.texX];

    if((color & 0xFF000000) != 0) {
      //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
      if(strip.side == 1) color = (color >> 1) & 8355711;

#if FOG_LEVEL
      color = color_lerp(color, FOG_COLOR, strip.fog);
#endif
      if(strip.seeThrough)
        color = ((color & 0xFEFEFE) >> 1) + ((buffer[y][strip.x] & 0xFEFEFE) >> 1);

      buffer[y][strip.x] = color;
    }

    c += texHeight;
    while(c > dy) {
      texY++;
      c -= dy;
    }
  }
}

struct SpritePrepare {
  int drawStartX, drawEndX;
  int dX, cX, texX;
  int drawStartY, drawEndY;
  int dY, texY0, cY0;
  int texNum;
  double transformY;
  double fog;
};

bool prepsSort (SpritePrepare &i, SpritePrepare &j) { return (i.transformY < j.transformY); }

std::vector<SpritePrepare> preps;

//SPRITE CASTING
void prepareSprites() {

  preps.clear();

  SpritePrepare prep;

  for(int i = 0; i < numSprites; i++)
  {
    //translate sprite position to relative to camera
    double spriteX = sprite[i].x - posX;
    double spriteY = sprite[i].y - posY;

    //transform sprite with the inverse camera matrix
    // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
    // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
    // [ planeY   dirY ]                                          [ -planeY  planeX ]

    double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

    double transformX = invDet * (dirY * spriteX - dirX * spriteY);
    prep.transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

    if(prep.transformY < 0) continue;

    int spriteScreenX = int((w / 2) * (1 + transformX / prep.transformY));

    //parameters for scaling and moving the sprites
    #define uDiv 2
    #define vDiv 2
    // Note that vMove is 128 rather than 64 to get the sprites on the ground.
    // It's because the textures are 32x32, rather than 64x64 as in the original.
    #define vMove 128.0
    int vMoveScreen = int(vMove / prep.transformY);

    //calculate height of the sprite on screen
    int spriteHeight = abs(int(h / (prep.transformY))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
    //calculate lowest and highest pixel to fill in current stripe
    prep.drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen + lookVert + eyePos/prep.transformY;
    prep.drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen + lookVert + eyePos/prep.transformY;

    //calculate width of the sprite
    int spriteWidth = abs(int (h / (prep.transformY))) / uDiv; // same as height of sprite, given that it's square
    prep.drawStartX = -spriteWidth / 2 + spriteScreenX;
    prep.drawEndX = spriteWidth / 2 + spriteScreenX;

    if(prep.drawStartX >= w || prep.drawEndX < 0) continue;

    // Precompute some variables for the vertical strips
    prep.dY = prep.drawEndY - prep.drawStartY;
    prep.cY0 = 0;
    prep.texY0 = 0;
    if(prep.drawStartY < 0) {
        prep.cY0 = -prep.drawStartY * texHeight;
        if(prep.cY0 > prep.dY) {
            div_t res = div(prep.cY0, prep.dY);
            prep.texY0 += res.quot;
            prep.cY0 = res.rem;
        }
        prep.drawStartY = 0;
    }
    if(prep.drawEndY >= h)
      prep.drawEndY = (h-1);

    prep.texX = 0;
    prep.dX = prep.drawEndX - prep.drawStartX;
    prep.cX = 0;

    if(prep.drawStartX < 0) {
        prep.cX = -prep.drawStartX * texWidth;
        if(prep.cX > prep.dX) {
            div_t res = div(prep.cX, prep.dX);
            prep.texX += res.quot;
            prep.cX = res.rem;
        }
        prep.drawStartX = 0;
    }
    if(prep.drawEndX > w) prep.drawEndX = w;

#if FOG_LEVEL
    prep.fog = prep.transformY / FOG_CONSTANT * FOG_LEVEL;
#endif

    prep.texNum = sprite[i].texture;
    preps.push_back(prep);
  }

  //sort sprites from far to close
  std::sort(preps.begin(), preps.end(), prepsSort);
}

void drawSpriteStrip(SpritePrepare &prep, int stripe) {

  if(stripe < prep.drawStartX || stripe >= prep.drawEndX)
    return;

  // If the left side of the sprite is concealed by a wall
  // then we need to adjust texX accordingly.
  int delta = stripe - prep.drawStartX;
  if(delta) {
      prep.drawStartX += delta;
      div_t res = div(prep.cX + delta * texWidth, prep.dX);
      prep.texX += res.quot;
      prep.cX = res.rem;
  }

  int texY = prep.texY0, cY = prep.cY0;
  for(int y = prep.drawStartY; y <= prep.drawEndY; y++) {

    Uint32 color = texture[prep.texNum][texWidth * texY + prep.texX]; //get current color from the texture
    if((color & 0x00FFFFFF) != 0) {
#if FOG_LEVEL
      color = color_lerp(color, FOG_COLOR, prep.fog);
#endif
      buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
    }

    cY = cY + texHeight;
    while(cY > prep.dY) {
      texY++;
      cY -= prep.dY;
    }
  }

  prep.drawStartX++;
  prep.cX += texWidth;
  while(prep.cX > prep.dX) {
    prep.texX++;
    prep.cX -= prep.dX;
  }
}

int main(int /*argc*/, char */*argv*/[])
{
  double time = 0; //time of current frame
  double oldTime = 0; //time of previous frame

  for(int i = 0; i < 11; i++) texture[i].resize(texWidth * texHeight);

#if SKYBOX
  std::vector<Uint32> skybox{320 * 240};
#endif

  screen(screenWidth,screenHeight, 0, "Raycaster");

  //load some textures
  unsigned long tw, th, error = 0;
  error |= loadImage(texture[0], tw, th, "pics/wall1.png");
  error |= loadImage(texture[1], tw, th, "pics/wall2.png");
  error |= loadImage(texture[2], tw, th, "pics/wall3.png");
  error |= loadImage(texture[3], tw, th, "pics/tile1.png");
  error |= loadImage(texture[4], tw, th, "pics/tile2.png");
  error |= loadImage(texture[5], tw, th, "pics/tile3.png");
  error |= loadImage(texture[6], tw, th, "pics/tile4.png");
  error |= loadImage(texture[7], tw, th, "pics/tile5.png");
  error |= loadImage(texture[8], tw, th, "pics/door.png");
  error |= loadImage(texture[9], tw, th, "pics/entry.png");
  error |= loadImage(texture[10], tw, th, "pics/gate.png");
  error |= loadImage(texture[11], tw, th, "pics/glass.png");
  error |= loadImage(texture[12], tw, th, "pics/glass-break.png");
  if(error) { std::cout << "error loading images" << std::endl; return 1; }

  //load some sprite textures
  error |= loadImage(texture[20], tw, th, "pics/altar.png");
  error |= loadImage(texture[21], tw, th, "pics/fountain.png");
  error |= loadImage(texture[22], tw, th, "pics/statue.png");
  if(error) { std::cout << "error loading images" << std::endl; return 1; }
#if SKYBOX
  error |= loadImage(skybox, tw, th, "pics/skybox.png");
  if(error) { std::cout << "error loading skybox" << std::endl; return 1; }
#endif

  double doorTime = 0;
  processDoors();

  //start the main loop
  while(!done())
  {

#if SKYBOX
    {
      int texX;
      double rayDirX0 = dirX - planeX;
      double rayDirY0 = dirY - planeY;
      double rayDirX1 = dirX + planeX;
      double rayDirY1 = dirY + planeY;

      int texX0 = (int)(-atan2(rayDirY0, rayDirX0) * (double)SKYBOX_WIDTH/(2 * M_PI) * SKYBOX_REPEATS);
      int texX1 = (int)(-atan2(rayDirY1, rayDirX1) * (double)SKYBOX_WIDTH/(2 * M_PI) * SKYBOX_REPEATS);
      while(texX1 < texX0)
          texX1 += SKYBOX_WIDTH;
      while(texX0 < 0) {
        texX0 += SKYBOX_WIDTH;
        texX1 += SKYBOX_WIDTH;
      }

      int dtexX = texX1 - texX0;
      int dy = h/2 + lookVert;
      int dtexY = SKYBOX_HEIGHT * (h/2 + lookVert) / (h/2 + LOOK_UP_MAX) - 1;
      int texY0 = SKYBOX_HEIGHT - 1 - dtexY;

      for(int x = 0, cX = 0; x < w; x++) {

          if(texX0 >= SKYBOX_WIDTH) {
              texX = texX0 - SKYBOX_WIDTH;
          } else
              texX = texX0;

          for(int y = 0, texY = texY0, cY = 0; y < dy; y++) {

              Uint32 color = skybox[SKYBOX_WIDTH * texY + texX];
              buffer[y][x] = color;

              cY = cY + dtexY;
              while(cY > dy) {
                  texY = texY + 1;
                  cY = cY - dy;
              }
          }

          cX = cX + dtexX;
          while(cX > w) {
              texX0 = texX0 + 1;
              cX = cX - w;
          }
      }
    }
#endif

    //FLOOR CASTING
    for(int y = screenHeight / 2 + lookVert + 1, p = 1; y < screenHeight; ++y, ++p)
    {
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = dirX - planeX;
      float rayDirY0 = dirY - planeY;
      float rayDirX1 = dirX + planeX;
      float rayDirY1 = dirY + planeY;

      // Vertical position of the camera.
      float posZ = 0.5 * screenHeight;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = (posZ + eyePos) / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = posX + rowDistance * rayDirX0;
      float floorY = posY + rowDistance * rayDirY0;

#if FOG_LEVEL
      double fog = rowDistance / FOG_CONSTANT * FOG_LEVEL;
#endif

      for(int x = 0; x < screenWidth; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
        int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int checkerBoardPattern = (int(cellX + cellY)) & 1;
        int floorTexture;
        if(checkerBoardPattern == 0) floorTexture = 3;
        else floorTexture = 4;

        Uint32 color = texture[floorTexture][texWidth * ty + tx];
        color = (color >> 1) & 8355711; // make a bit darker
#if FOG_LEVEL
        color = color_lerp(color, FOG_COLOR, fog);
#endif
        buffer[y][x] = color;
      }
    }

#if !SKYBOX
    //CEILING CASTING
    for(int y = screenHeight / 2 + lookVert + 1, p = 1; y >= 0; y--, ++p)
    {
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = dirX - planeX;
      float rayDirY0 = dirY - planeY;
      float rayDirX1 = dirX + planeX;
      float rayDirY1 = dirY + planeY;

      // Vertical position of the camera.
      float posZ = 0.5 * screenHeight;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = (posZ - eyePos) / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = posX + rowDistance * rayDirX0;
      float floorY = posY + rowDistance * rayDirY0;

#if FOG_LEVEL
      double fog = rowDistance / FOG_CONSTANT * FOG_LEVEL;
#endif
      for(int x = 0; x < screenWidth; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
        int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int ceilingTexture = 6;
        Uint32 color = texture[ceilingTexture][texWidth * ty + tx];
        color = (color >> 1) & 8355711; // make a bit darker
#if FOG_LEVEL
        color = color_lerp(color, FOG_COLOR, fog);
#endif
        buffer[y][x] = color;
      }
    }
#endif

    prepareSprites();

    // WALL CASTING
    std::stack<Strip> stack;
    for(int x = 0; x < w; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      //which box of the map we're in
      int mapX = int(posX);
      int mapY = int(posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

      //length of ray from one x or y-side to next x or y-side
      double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?

      //calculate step and initial sideDist
      if(rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if(rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

rayscan:
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if(sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if(worldMap[mapX][mapY] > 0) hit = 1;
      }

      //texturing calculations
      int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

      Door *door = NULL;
      if(texNum == 8 || texNum == 9 || texNum == 10 || texNum == 11 || texNum == 12) {
        /* Sunken wall encountered */
        if(texNum == 8)
          door = findDoor(mapX, mapY); /* Door encountered */
        if(side == 0) {
          double dist = sideDistX - 0.5;
          if(sideDistY < dist) {
            hit = 0;
            goto rayscan;
          }
          perpWallDist = dist;
        } else {
          double dist = sideDistY - 0.5;
          if(sideDistX < dist) {
            hit = 0;
            goto rayscan;
          }
          perpWallDist = dist;
        }
      } else {
        //Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect!)
        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);
      }

      //Calculate height of line to draw on screen
      int lineHeight = (int)(h / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2 + lookVert + eyePos/perpWallDist;
      int drawEnd = lineHeight / 2 + h / 2 + lookVert + eyePos/perpWallDist;

      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (side == 0) wallX = posY + perpWallDist * rayDirY;
      else           wallX = posX + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = int(wallX * double(texWidth));

      if(door) {
        texX -= texWidth - door->counter;
        if(texX < 0) {
          hit = 0;
          goto rayscan;
        }
      }

      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

#if FOG_LEVEL
      double fog = perpWallDist / FOG_CONSTANT * FOG_LEVEL;
#endif

      Strip strip = {
        x, drawStart, drawEnd, perpWallDist, texture[texNum], texX, fog, side,
        texNum == 11 || texNum == 12
      };
      stack.push(strip);

      if(texNum == 9 || texNum == 10 || texNum == 11 || texNum == 12) {
        hit = 0;
        goto rayscan;
      }

      // Tracks the furthest sprite we haven't drawn yet
      int farSprite = preps.size() - 1;

      // Skip sprites behind the furthest wall
      // note perpWallDist == stack.top().perpWallDist
      while(farSprite >= 0 && preps[farSprite].transformY > perpWallDist)
        farSprite--;

      while(!stack.empty()) {
        Strip &strip = stack.top();

        // Draw any sprites behind the wall strip we're currently drawing
        while(farSprite >= 0 && preps[farSprite].transformY > strip.perpWallDist) {
          drawSpriteStrip(preps[farSprite--], x);
        }

        // Now draw the strip itself
        drawStrip(strip);
        stack.pop();
      }
      // Draw sprites in front of the nearest wall:
      while(farSprite >= 0)
        drawSpriteStrip(preps[farSprite--], x);
    }

    drawBuffer(buffer[0]);
    // No need to clear the screen here, since everything is overdrawn with floor and ceiling

    //timing for input and FPS counter
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; //frametime is the time this frame has taken, in seconds
    print(1.0 / frameTime); //FPS counter

    print((int)posX, 10, 10);
    print((int)posY, 40, 10);

    redraw();

    doorTime += frameTime;
    if(doorTime > 1.0/texWidth) {
      updateDoors();
      doorTime -= 1.0/texWidth;
    }

    //speed modifiers
    double moveSpeed = frameTime * 3.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 2.0; //the constant value is in radians/second
    readKeys();
    //move forward if no wall in front of you
    if (keyDown(SDLK_UP))
    {
      if(canPass(int(posX + dirX * moveSpeed), int(posY))) posX += dirX * moveSpeed;
      if(canPass(int(posX),int(posY + dirY * moveSpeed))) posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if(keyDown(SDLK_DOWN))
    {
      if(canPass(int(posX - dirX * moveSpeed),int(posY))) posX -= dirX * moveSpeed;
      if(canPass(int(posX),int(posY - dirY * moveSpeed))) posY -= dirY * moveSpeed;
    }
    //rotate to the right
    if(keyDown(SDLK_RIGHT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if(keyDown(SDLK_LEFT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if(keyDown(SDLK_SPACE))
    {
      int faceX = int(posX + dirX);
      int faceY = int(posY + dirY);
      Door *door = findDoor(faceX, faceY);
      if(door) {
        switch(door->state) {
          case closed: door->state = opening; break;
          case open: door->state = closing; break;
          default: break;
        }
      } else if(worldMap[faceX][faceY] == 12) {
        worldMap[faceX][faceY] = 13;
      }
    }
    if(keyDown(SDLK_q)) {
      if(lookVert < LOOK_UP_MAX) {
        lookVert += frameTime * 600;
        if(lookVert > LOOK_UP_MAX)
          lookVert = LOOK_UP_MAX;
      }
    } else if(keyDown(SDLK_a)) {
      if(lookVert > -LOOK_UP_MAX) {
        lookVert -= frameTime * 600;
        if(lookVert < -LOOK_UP_MAX)
          lookVert = -LOOK_UP_MAX;
      }
    } else {
      if(lookVert > 0) {
        lookVert -= frameTime * 400;
        if(lookVert < 0)
          lookVert = 0;
      } else if(lookVert < 0) {
        lookVert += frameTime * 400;
        if(lookVert > 0)
          lookVert = 0;
      }
    }
    if(keyDown(SDLK_x)) {
      if(eyePos == 0)
        eyePos = 128;
    } else if(keyDown(SDLK_c)) {
      if(eyePos <= 0 && eyePos > -128) {
        eyePos -= frameTime * 400;
        if(eyePos < -128) {
          eyePos = -128;
        }
      }
    } else {
      if(eyePos < 0) {
        eyePos += frameTime * 400;
        if(eyePos > 0) {
          eyePos = 0;
        }
      }
    }
    if(eyePos > 0) {
      eyePos -= frameTime * 400;
      if(eyePos < 0) {
        eyePos = 0;
      }
    }
    if(keyDown(SDLK_ESCAPE))
    {
      break;
    }
  }
  return 0;
}

//sort the sprites based on distance
void sortSprites(int* order, double* dist, int amount)
{
  std::vector<std::pair<double, int>> sprites(amount);
  for(int i = 0; i < amount; i++) {
    sprites[i].first = dist[i];
    sprites[i].second = order[i];
  }
  std::sort(sprites.begin(), sprites.end());
  // restore in reverse order to go from farthest to nearest
  for(int i = 0; i < amount; i++) {
    dist[i] = sprites[amount - i - 1].first;
    order[i] = sprites[amount - i - 1].second;
  }
}
