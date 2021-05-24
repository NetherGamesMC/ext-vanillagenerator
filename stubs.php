<?php

final class SimplexOctaveGenerator{

        public static function fromRandomAndOctaves(int $seed, int $octavesNum, int $size_x, int $size_y, int $size_z) : \SimplexOctaveGenerator{}

        public function setScale(float $scale) : void{}

        public function getSizeX() : int{}

        public function getSizeZ() : int{}

        public function getFractalBrownianMotion(float $x, float $y, float $z, float $lacunarity, float $persistence) : array{}

        public function noise(float $x, float $y, float $z, float $frequency, float $amplitude, bool $normalized) : float{}
}

final class PerlinOctaveGenerator{

        public static function fromRandomAndOctaves(int $seed, int $octavesNum, int $size_x, int $size_y, int $size_z) : \SimplexOctaveGenerator{}

        public function setXScale(float $x_scale) : void{}

        public function setYScale(float $x_scale) : void{}

        public function setZScale(float $x_scale) : void{}

        public function getFractalBrownianMotion(float $x, float $y, float $z, float $lacunarity, float $persistence) : array{}

        public function getSizeY() : int{}
}