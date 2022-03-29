#include "Terrain.h"

#include <random>
#include "Framework.h"
#include "PerlinNoise.h"
#include "RigidBody.h"
#include "../functions/loadBMP.h"
#include "../functions/settings.h"

Terrain::Terrain(int worldWidth, int worldHeight, double noiseCaveValue,
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
    this->blockWidth = BLOCK_WIDTH;
    this->blockHeight = BLOCK_HEIGHT;
    this->terrain = new QuadTree(Rectangle(
        worldWidth * blockWidth, worldHeight * blockHeight,
        Vector(worldWidth * blockWidth / 2, -worldHeight * blockHeight / 2)));
}

Terrain::~Terrain(){
    delete noiseTexture;
    delete terrain;
    for(int i = 0; i < stoneSurfaceList.size(); i++){
        SDL_FreeSurface(stoneSurfaceList[i]);
    }
    for(int i = 0; i < dirtSurfaceList.size(); i++){
        SDL_FreeSurface(dirtSurfaceList[i]);
    }
    for(int i = 0; i < grassDirtSurfaceList.size(); i++){
        SDL_FreeSurface(grassDirtSurfaceList[i]);
    }
    for(int i = 0; i < treeSurfaceList.size(); i++){
        SDL_FreeSurface(treeSurfaceList[i]);
    }
}

void Terrain::generate(SDL_Surface* screen) {
    this->stoneSurfaceList.push_back(loadBMP("../bmp/stone.bmp"));

    this->dirtSurfaceList.push_back(loadBMP("../bmp/dirt.bmp"));
    this->dirtSurfaceList.push_back(loadBMP("../bmp/gravel_dirt.bmp"));

    this->grassDirtSurfaceList.push_back(loadBMP("../bmp/dirt_grass.bmp"));

    this->treeSurfaceList.push_back(loadBMP("../bmp/trunk_bottom.bmp"));
    this->treeSurfaceList.push_back(loadBMP("../bmp/trunk_mid.bmp"));
    SDL_Surface* treeCrown = loadBMP("../bmp/treeCrown.bmp");
    double resizeFactorW = 64 / BLOCK_WIDTH;
    double resizeFactorH = 64 / BLOCK_HEIGHT;
    SDL_Surface* treeCrownScaled =
        SDL_CreateRGBSurface(0, static_cast<int>(treeCrown->w / resizeFactorW),
                             static_cast<int>(treeCrown->h / resizeFactorH), 32,
                             0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitScaled(treeCrown, NULL, treeCrownScaled, NULL);
    this->treeSurfaceList.push_back(treeCrownScaled);

    generateNoiseTexture();
    generateTerrain();
}

void Terrain::generateTerrain() {
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

                if (noiseTexture[(y + 1) * worldWidth + x] > noiseCaveValue) {
                    int t = rand() % 100;
                    if (t < 10) {
                        placeTree(x, y + 1);
                    } else if (x <= worldWidth / 2 + 10 &&
                               x >= worldWidth / 2 - 10) {
                        spawnPoint =
                            Vector(x * BLOCK_WIDTH + BLOCK_WIDTH / 2,
                                   -y * BLOCK_HEIGHT - 3 * BLOCK_HEIGHT);
                    }
                }
            }

            if (noiseTexture[y * worldWidth + x] > noiseCaveValue) {
                placeTile(x, y, tileSurface);
            }
        }
    }
}

void Terrain::generateNoiseTexture() {
    noiseTexture = new double[worldWidth * worldHeight];
    PerlinNoise pn(seed);

    for (int y = 0; y < worldHeight; y++) {
        for (int x = 0; x < worldWidth; x++) {
            double v = pn.noise(x * caveFreq, y * caveFreq, 0.8);

            noiseTexture[y * worldWidth + x] = v;
        }
    }
}

void Terrain::placeTree(int x, int y) {
    int h = rand() % 4 + 2;

    placeTile(x, y, treeSurfaceList[0], false);
    for (int i = 1; i < h; i++) {
        placeTile(x, y + i, treeSurfaceList[1], false);
    }
    placeTile(x, y + h + 2, treeSurfaceList[2], false, false);
}

void Terrain::placeTile(int x, int y, SDL_Surface* tileSurface,
                        bool collidable,
                        bool drawScaledToHitbox) {
    blockCount++;
    Vector blockPosition = Vector(x * blockWidth + blockWidth / 2,
                                  -y * blockHeight - blockHeight / 2);
    std::vector<SDL_Surface*> tileSurfaceList;
    tileSurfaceList.push_back(tileSurface);

    terrain->insert(GameObject(
        Renderer(tileSurfaceList, drawScaledToHitbox),
        RigidBody(blockPosition, blockWidth, blockHeight, collidable)));
}