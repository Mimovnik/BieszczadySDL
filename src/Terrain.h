#pragma once
#include <SDL.h>

#include <iostream>
#include <random>

#include "LoadBMP.cpp"
#include "PerlinNoise.h"
#include "QuadTree.h"
#include "RigidBody.h"

class Terrain {
   public:
    int worldWidth;
    int worldHeight;

    int dirtLayerHeight;
    double terrainFreq;
    double caveFreq;
    float heightMultiplier;
    float heightAddition;
    double noiseCaveValue;
    unsigned int seed;

    double* noiseTexture;

    QuadTree* terrain;
    SDL_Surface* stoneSurface;
    SDL_Surface* dirtSurface;
    SDL_Surface* grassDirtSurface;
    SDL_Surface* trunkSurface;
    SDL_Surface* leafSurface;

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
        this->terrain = new QuadTree(Rectangle(
            20000, 20000, Vector(10000, -10000)));
    }
    void generate(SDL_Surface* charset, SDL_Surface* screen,
                  SDL_Texture* screenTexture, SDL_Window* window,
                  SDL_Renderer* renderer) {
        this->stoneSurface = loadBMP("../bmp/stone.bmp", charset, screen,
                                     screenTexture, window, renderer);
        this->dirtSurface = loadBMP("../bmp/dirt.bmp", charset, screen,
                                    screenTexture, window, renderer);
        this->grassDirtSurface =
            loadBMP("../bmp/dirt_grass.bmp", charset, screen, screenTexture,
                    window, renderer);
        this->trunkSurface = loadBMP("../bmp/trunk_side.bmp", charset, screen,
                                     screenTexture, window, renderer);
        this->leafSurface = loadBMP("../bmp/leaves_transparent.bmp", charset,
                                    screen, screenTexture, window, renderer);

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
                if(height > worldHeight){
                    height = worldHeight;
                }

            SDL_Surface* tileSurface;
            for (int y = 0; y < height; y++) {
                if (y < height - dirtLayerHeight) {
                    tileSurface = stoneSurface;
                } else if (y < height - 1) {
                    tileSurface = dirtSurface;
                } else {
                    // generate grass
                    tileSurface = grassDirtSurface;
                    // generate Tree

                    // int t = rand() % 100;
                    // if (t < 10) {
                    //     placeTree(x, y + 1);
                    // }
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

    // void placeTree(int x, int y) {

    //     int h = rand() % 4 + 4;
    //     for (int i = 0; i < h; i++) {
    //         placeTile(x, y + i, trunkSurface);
    //     }
    //     placeTile(x, y + h, leafSurface);
    // }

    void placeTile(int x, int y, SDL_Surface* tileSurface) {
        Vector blockPosition = Vector(x * 64, -y * 64);
        terrain->insert(RigidBody(blockPosition, tileSurface, 64, 64));
    }
};