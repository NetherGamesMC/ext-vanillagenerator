<?php

class Random{

        public function __construct(int $seed = -1){}

        public function setSeed(int $seed) : void{}

        public function getSeed() : int{}

        public function nextInt() : int{}

        public function nextSignedInt() : int{}

        public function nextFloat() : float{}

        public function nextSignedFloat() : float{}

        public function nextBoolean() : int{}

        public function nextRange(int $start = 0, int $end = 0x7fffffff) : int{}

        public function nextBoundedInt(int $bound) : int{}
}

final class SimplexOctaveGenerator{

        public static function fromRandomAndOctaves(\Random $seed, int $octavesNum, int $size_x, int $size_y, int $size_z) : \SimplexOctaveGenerator{}

        public function setScale(float $scale) : void{}

        public function getSizeX() : int{}

        public function getSizeZ() : int{}

        public function getFractalBrownianMotion(array &$array, float $x, float $y, float $z, float $lacunarity, float $persistence) : void{}

        public function noise(float $x, float $y, float $z, float $frequency, float $amplitude, bool $normalized) : float{}
}

final class PerlinOctaveGenerator{

        public static function fromRandomAndOctaves(\Random $seed, int $octavesNum, int $size_x, int $size_y, int $size_z) : \PerlinOctaveGenerator{}

        public function setXScale(float $x_scale) : void{}

        public function setYScale(float $x_scale) : void{}

        public function setZScale(float $x_scale) : void{}

        public function getFractalBrownianMotion(array &$array, float $x, float $y, float $z, float $lacunarity, float $persistence) : void{}

        public function getSizeY() : int{}
}