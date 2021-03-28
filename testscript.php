<?php

declare(strict_types=1);
$noise = new SimplexNoise();
$noise->initialize();
$start = hrtime(true);
$noise->generate(32, 32);
var_dump(hrtime(true) - $start);
$noise->free();
?>
