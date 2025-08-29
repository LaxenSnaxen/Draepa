#include <iostream>
#include <vector>

// Standard logik för terräng generation baserat på perlin-brus.
// Perlin-brus generar en svart vit karta, vi vill ge varje punkt på kartan ett värde mellan 0 och 1.
// 0 är svart, dem kommer vara de lägsta punkterna i terrängen. 1 är vitt, och dem kommer vara de högsta punkterna i terrängen.

// Detta kommer i princcip vara pseudo kod tills allting är integrerat med Unreal Engine 5.
// Vi kommer att använda biblioteket från https://github.com/Auburn/FastNoise2/.
// ALlt detta kommer även vara bserat på lärdomarna från denna artikel: https://www.redblobgames.com/maps/terrain-from-noise/.

std::vector<float> noiseOutput(64 * 64 * 64);

void noiseGenerator()
{
    fnGenerator->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, 16, 16, 16, 0.2f, 1337);
    int index = 0;
    
    for (int z = 0; z < 16; z++)
    {
        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < 16; x++)
            {
                ProcessVoxelData(x, y, z, noiseOutput[index++]);			
            }			
        }
    }
}