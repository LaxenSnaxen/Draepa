#include <iostream>
#include <vector>
#include <cmath>
#include "deps/FastNoise2-master/src/FastNoise/FastNoise_C.cpp"
#include "deps/FastNoise2-master/src/FastNoise/SmartNode.cpp"
#include "deps/FastNoise2-master/src/FastNoise/Metadata.cpp"
#include "deps/FastNoise2-master/src/FastNoise/Base64.h"
// Standard logik för terräng generation baserat på perlin-brus.
// Perlin-brus generar en svart vit karta, vi vill ge varje punkt på kartan ett värde mellan 0 och 1.
// 0 är svart, dem kommer vara de lägsta punkterna i terrängen. 1 är vitt, och dem kommer vara de högsta punkterna i terrängen.

// Detta kommer i princcip vara pseudo kod tills allting är integrerat med Unreal Engine 5.
// Vi kommer att använda biblioteket från https://github.com/Auburn/FastNoise2/.
// Det ovanstående var den intiala planen, överväg att byta till FastNoiseLite. https://github.com/Auburn/FastNoiseLite?tab=readme-ov-file.
// ALlt detta kommer även vara bserat på lärdomarna från denna artikel: https://www.redblobgames.com/maps/terrain-from-noise/.

//auto fnPerlin = FastNoise::New<FastNoise::Perlin>();
//std::vector<float> noiseOutput(64 * 64 * 64);
//FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree( "DQAFAAAAAAAAQAgAAAAAAD8AAAAAAA==" );
float frequency = 1.0f;
float exponent = 1.0f;
float fudgeFactor = 1.2f;
float waterLevel = 0.4f;
void noiseGenerator()
{
    //fnGenerator->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, 64, 64, 64, 0.2f, 1337);
    int dimension = 64;

    for (int z = 0; z < 64; z++)
    {
        for (int y = 0; y < 64; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                double nx = x/dimension - 0.5, ny = y/dimension - 0.5, nz = z/dimension - 0.5;
                auto e = (1 * noise(frequency * nx, frequency * ny, frequency * nz)) + (0.5 * noise(frequency * nx, frequency * ny, frequency * nz)) + (0.25 * noise(frequency * nx, frequency * ny, frequency * nz));
                e = e / (1 + 0.5 + 0.25);
                e = pow(e * fudgeFactor, exponent)

                //ProcessVoxelData(nx, ny, nz, noiseOutput[index++]);			
            }			
        }
    }
}

// Eftersom terrängen skall vara baserad på Värmlands klimat skall följande biomer inkluderas, även om biomerna endast har visuell funktionalitet i denna prototyp.
// Sjö, strand, träsk, äng, barrskog, fjällskog, berg och hedar. (Glöm inte Grums, den viktigaste biomen av dom alla, den farligaste)

std::string biome(float e, float m) {
  // these thresholds will need tuning to match your generator
  if (e < 0.1) return "OCEAN";
  else if (e < 0.12) return "BEACH";

}

float ridgenoise(float nx, float ny) {
  return 2 * (0.5 - abs(0.5 - noise(nx, ny)));
}


FastNoiseLite gen; // if using fastnoiselite
double noise(double nx, double ny) { // if using fastnoiselite
  // Rescale from -1.0:+1.0 to 0.0:1.0
  return gen.GetNoise(nx, ny) / 2.0 + 0.5;
}


const int height = 64;
const int width = 64;
double value[height][width];
for (int y = 0; y < height; y++) {
  for (int x = 0; x < width; x++) {
    double nx = x/width - 0.5, 
           ny = y/height - 0.5;
    value[y][x] = noise(nx, ny);
  }

}
