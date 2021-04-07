# noiselib
PHP Extension for noise generation

This implements various noise generation methods in one class.

# Building from source
As for now, PHP-CPP only supports linux host, the guide given here is to only build 
this source in linux machines. You will have to install `make` in your machine in order to build this source correctly.

Step 2 and 3 is not a neccessary thing to be done, it is more of a hack to make sure the compiler knows what path to find, if you
have linked all php binaries into your bin folder, you can skip these steps. But if you don't, then follow the steps given to avoid confusion.

### 1. Building PHP Source
The first step to build this noise is first to build the PHP source from our
[php-build-scripts](https://github.com/NetherGamesMC/php-build-scripts) source code, before you run `./compile`
make sure that you have [edited](https://github.com/NetherGamesMC/php-build-scripts/blob/master/compile.sh#L113) 
`./compile` to set `DO_CLEANUP` to `no`. This is to make sure that the PHP sources will not get removed as we need it for the next step.

### 2. Identifying PHP Includes
After building the source, run the command `php-build-scripts/bin/php7/bin/php-config --includes`. The output would be like this:
```bash
-I/root/php-build-scripts/bin/php7/include/php -I/root/php-build-scripts/bin/php7/include/php/main -I/root/php-build-scripts/bin/php7/include/php/TSRM -I/root/php-build-scripts/bin/php7/include/php/Zend -I/root/php-build-scripts/bin/php7/include/php/ext -I/root/php-build-scripts/bin/php7/include/php/ext/date/lib
```
You will have to remember these paths as we require these includes in the next step.

### 3. Building PHP-CPP sources
Now, this library is NOT a php-extension, but rather a dynamic linked library for your server. Clone your preferred [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) sources and edit `Makefile` file. You will have to change
`PHP_COMPILER_FLAGS` to 
```
PHP_COMPILER_FLAGS = ${COMPILER_FLAGS} -I/root/php-build-scripts/bin/php7/include/php -I/root/php-build-scr...
```
The ommitted arguments is the includes path that we have seen in Step 2. Save the file and run `make` then followed by `make install`

Congratulations, you have installed PHP-CPP in your machine, but wait you are not done yet.

### 4. Building noiselib extension
Clone this repository, and then run `make`, there will be a file named `noisegen.so`, you will have to copy this file into your server extensions folder, which can be found with `opcache.so` file, `~/php-build-scripts/bin/php7/lib/php/extensions/no-debug-zts-20200930`. 

Then edit `php.ini` to add `extension=noisegen.so` in the file. Now after saving you changes, you can now
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
