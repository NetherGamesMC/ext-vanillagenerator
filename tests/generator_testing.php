<?php

$data = file_get_contents("data.json");
$contents = json_decode($data);

foreach($contents as [$typeId, $blockMeta, $blockStates]){
	OverworldGenerator::registerBlock($typeId, $blockMeta, $blockStates);
}

$gen = new OverworldGenerator(312321312, false);

$chunks = [];
for($x = 0; $x < 30; $x++){
	for($z = 0; $z < 30; $z++){
		$chunkHash = morton2d_encode($x, $z);
		for($y = 0; $y < 24; $y++){
			$chunks[$chunkHash][0][$y] = new pocketmine\world\format\PalettedBlockArray(2560016);
			$chunks[$chunkHash][1][$y] = new pocketmine\world\format\PalettedBlockArray(0);
		}

		print "Generating $x | $z | $chunkHash" . PHP_EOL;

		$gen->generateChunk($chunks[$chunkHash][0], $chunks[$chunkHash][1], $chunkHash);

		$blockEntries = [];
		for($x1 = $x - 1; $x1 < ($x + 1); $x1++){
			for($z1 = $z - 1; $z1 < ($z + 1); $z1++){
				$chunkHash2 = morton2d_encode($x1, $z1);

				if(!isset($chunks[$chunkHash2])){
					$blockEntries = [];
					break 2;
				}

				$blockEntries[] = [$chunkHash2, [$chunks[$chunkHash2][0], $chunks[$chunkHash2][1]], false];
			}
		}

		if(!empty($blockEntries)){
			print "Populating $x | $z | $chunkHash" . PHP_EOL;

			$gen->populateChunk($blockEntries, $chunkHash);
		}
	}
}
