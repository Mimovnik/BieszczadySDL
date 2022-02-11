#pragma once
#include <SDL.h>

#include <iostream>
#include <random>
#include <vector>

#include "PerlinNoise.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "loadBMP.cpp"

class Terrain {
   public:
    int worldWidth;
    int worldHeight;

    int blockCount;

    int dirtLayerHeight;
    double terrainFreq;
    double caveFreq;
    float heightMultiplier;
    float heightAddition;
    double noiseCaveValue;
    unsigned int seed;

    double* noiseTexture;

    QuadTree* terrain;

    std::vector<SDL_Surface*> stoneSurfaceList;
    std::vector<SDL_Surface*> dirtSurfaceList;
    std::vector<SDL_Surface*> grassDirtSurfaceList;

    std::vector<SDL_Surface*> treeSurfaceList;

    Terrain(int worldWidth, int worldHeight, double noiseCaveValue,
            double terrainFreq, double caveFreq, float heightMultiplier,
            float heightAddition, int dirtLayerHeight, unsigned int seed) {
        this->worldWidth = worldWidth;
        this->worldHeight = worldHeight;
        this->noiseCaveValue = noiseCaveValue;
        this->terrainFreq = terrainFreq;
        this->caveFreq = caveFreq;
        this->heightMultiplier = heightMultiplier;
        this->heightAddition = heightAddition;
        this->dirtLayerHeight = dirtLayerHeight;
        this->seed = seed;
        this->blockCount = 0;
        this->terrain = new QuadTree(
            Rectangle(worldWidth * 64, worldHeight * 64,
                      Vector(worldWidth * 64 / 2, -worldHeight * 64 / 2)));
    }
    void generate(SDL_Surface* charset, SDL_Surface* screen,
                  SDL_Texture* screenTexture, SDL_Window* window,
                  SDL_Renderer* renderer) {
        this->stoneSurfaceList.push_back(loadBMP("../bmp/stone.bmp", charset,
                                                 screen, screenTexture, window,
                                                 renderer));

        this->dirtSurfaceList.push_back(loadBMP("../bmp/dirt.bmp", charset,
                                                screen, screenTexture, window,
                                                renderer));
        this->dirtSurfaceList.push_back(loadBMP("../bmp/gravel_dirt.bmp",
                                                charset, screen, screenTexture,
                                                window, renderer));

        this->grassDirtSurfaceList.push_back(
            loadBMP("../bmp/dirt_grass.bmp", charset, screen, screenTexture,
                    window, renderer));

        this->treeSurfaceList.push_back(loadBMP("../bmp/trunk_bottom.bmp",
                                                charset, screen, screenTexture,
                                                window, renderer));
        this->treeSurfaceList.push_back(loadBMP("../bmp/trunk_mid.bmp", charset,
                                                screen, screenTexture, window,
                                                renderer));
        this->treeSurfaceList.push_back(loadBMP("../bmp/treeCrown.bmp", charset,
                                                screen, screenTexture, window,
                                                renderer));

        generateNoiseTexture();
        generateTerrain();
    }

    void generateTerrain() {
        PerlinNoise pn(seed);
        for (int x = 0; x < worldWidth; x++) {
            double height =
                pn.noise((x + seed) * terrainFreq, seed * terrainFreq, 0.8) *
                    heightMultiplier +
                heightAddition;
            if (height > worldHeight) {
                height = worldHeight;
            }

            SDL_Surface* tileSurface;
            for (int y = 0; y < height; y++) {
                if (y < height - dirtLayerHeight) {
                    int tileVariant = rand() % stoneSurfaceList.size();
                    tileSurface = stoneSurfaceList[tileVariant];
                } else if (y < height - 1) {
                    int tileVariant = rand() % dirtSurfaceList.size();
                    tileSurface = dirtSurfaceList[tileVariant];
                } else {
                    // generate grass
                    int t = rand() % grassDirtSurfaceList.size();
                    tileSurface = grassDirtSurfaceList[t];
                    // generate Tree

                    if (noiseTexture[(y + 1) * worldWidth + x] >
                        noiseCaveValue) {
                        int t = rand() % 100;
                        if (t < 10) {
                            placeTree(x, y + 1);
                        }
                        int b = rand() % 100;
                        if (b < 20) {
                        }
                    }
                }

                if (noiseTexture[y * worldWidth + x] > noiseCaveValue) {
                    placeTile(x, y, tileSurface);
                }
            }
        }
    }

    void generateNoiseTexture() {
        noiseTexture = new double[worldWidth * worldHeight];
        PerlinNoise pn(seed);

        for (int y = 0; y < worldHeight; y++) {
            for (int x = 0; x < worldWidth; x++) {
                double v = pn.noise(x * caveFreq, y * caveFreq, 0.8);

                noiseTexture[y * worldWidth + x] = v;
            }
        }
    }

    void placeTree(int x, int y) {
        int h = rand() % 4 + 2;

        placeTile(x, y, treeSurfaceList[0], false);
        for (int i = 1; i < h; i++) {
            placeTile(x, y + i, treeSurfaceList[1], false);
        }
        placeTile(x, y + h + 2, treeSurfaceList[2], false, false);
    }

    void placeTile(int x, int y, SDL_Surface* tileSurface,
                   bool collidable = true, bool drawScaledToHitbox = true) {
        blockCount++;
        Vector blockPosition = Vector(x * 64 + 64 / 2, -y * 64 - 64 / 2);
        terrain->insert(RigidBody(blockPosition, tileSurface, 64, 64,
                                  collidable, drawScaledToHitbox));
    }
};