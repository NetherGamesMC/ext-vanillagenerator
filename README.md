# noiselib
PHP Extension for noise generation

This implements various noise generation methods in one class.

# Building from source
As for now, PHP-CPP only supports linux host, the guide given here is to only build 
this source in linux machines. You will have to install `make` in your machine in order to build this source correctly.

### 1. Building PHP Source
The first step to build this noise is first to build the PHP source from our
[php-build-scripts](https://github.com/NetherGamesMC/php-build-scripts) source code, with the arguments `./compile -n`. 
This is to make sure that the PHP sources will not get removed as we need it for the next step.

### 2. Installing PHP-CPP sources
After building the PHP sources, make sure you have symlinked `php` and `php-config` into `/usr/bin`, this is to make sure that
your php binaries can be known by PHP-CPP makefile to build itself.

These are the example on how to symlink php binaries.
```
ln -n /root/php-build-scripts/bin/php7/bin/php-config /usr/bin/php-config
ln -n /root/php-build-scripts/bin/php7/bin/php /usr/bin/php
```

Now, this library is NOT a php-extension, but rather a dynamic linked library for your server. Clone your preferred [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) sources and run `make` then followed by `make install`

Congratulations, you have installed PHP-CPP in your machine, but wait you are not done yet.

### 4. Building noiselib extension
Clone this repository, and then run `make`, there will be a file named `noisegen.so`, you will have to copy this file into your server extensions folder, which can be found with `opcache.so` file, `~/php-build-scripts/bin/php7/lib/php/extensions/no-debug-zts-20200930`. 

Then edit `php.ini` to add `extension=noisegen.so` in the file. Now after saving the changes you made, you can now
confirm that the noise has been installed with the command `php -m`

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
