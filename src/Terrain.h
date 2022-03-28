
#ifndef TERRAIN_H
#define TERRAIN_H
#include <SDL.h>

#include <iostream>
#include <random>
#include <vector>

#include "Framework.h"
#include "PerlinNoise.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "Vector.h"
#include "loadBMP.cpp"
#include "settings.h"
class Terrain {
   public:
    int worldWidth;
    int worldHeight;

    int blockCount;
    int blockWidth;
    int blockHeight;

    int dirtLayerHeight;
    double terrainFreq;
    double caveFreq;
    float heightMultiplier;
    float heightAddition;
    double noiseCaveValue;
    unsigned int seed;

    Vector spawnPoint;

    double* noiseTexture;

    QuadTree* terrain;

    std::vector<SDL_Surface*> stoneSurfaceList;
    std::vector<SDL_Surface*> dirtSurfaceList;
    std::vector<SDL_Surface*> grassDirtSurfaceList;

    std::vector<SDL_Surface*> treeSurfaceList;

    Terrain(int worldWidth, int worldHeight, double noiseCaveValue,
            double terrainFreq, double caveFreq, float heightMultiplier,
            float heightAddition, int dirtLayerHeight, unsigned int seed);

    void generate(SDL_Surface* screen);

    void generateTerrain();

    void generateNoiseTexture();

    void placeTree(int x, int y);

    void placeTile(int x, int y, SDL_Surface* tileSurface,
                   bool collidable = true, bool drawScaledToHitbox = true);
};

#endif