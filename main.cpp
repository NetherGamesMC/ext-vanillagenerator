#include <phpcpp.h>
#include <phpcpp.h>
#include <iostream>
#include <perlin/PerlinNoise.hpp>
    
void testfunction(Php::Parameters &params)
{
  const siv::PerlinNoise perlin(123);
}


extern "C" {
    PHPCPP_EXPORT void *get_module() 
    {
	static Php::Extension extension("noisegen", "1.0");
	extension.add<testfunction>("testfunction", {
		Php::ByVal("seed", Php::Type::Numeric)
		}
	);
        return extension;
    }
}
