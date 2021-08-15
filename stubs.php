<?php

class OverworldGenerator{

    public function __construct(int $seed, bool $isUHC = false){}

    public function populateChunk(array &$palettedArray, array $biomeArray, array &$dirtyFlags, int $morton) : void{}

    public function generateChunk(array &$palettedArray, string $biomeArray, int $morton) : string{}
}