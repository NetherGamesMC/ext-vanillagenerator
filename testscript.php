<?php

declare(strict_types=1);
$noise = new Noise();
$noise->initialize();
$start = hrtime(true);
$noise->generate2d(32, 32);
var_dump(hrtime(true) - $start);
$noise->free();
?>
