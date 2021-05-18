# noiselib
PHP Extension for noise generation

This implements various noise generation methods in one class.

# the Noise class
The `Noise` class is the class that contains all the utility for creating a noise generator.

**Constructor**
`Noise(int seed = 1337)`<br/>
Following the signature of the perlin noise, the default seed is set to 1337.<br/>

**Methods**
The most important methods are the generate2d and generate3d methods.

**generate2d(int x, int y)**<br/>
Generates a terrain with the specific x and y sizes<br/>

**generate3d(int x, int y, int z)**<br/>
Generates a 3d terrain with the specified x,y and z limits.<br/>

**Constants**
NoiseTypes:
- `NOISE_TYPE_OPENSIMPLEX2`
- `NOISE_TYPE_OPENSIMPLEX2S`
- `NOISE_TYPE_CELLULAR`
- `NOISE_TYPE_PERLIN`
