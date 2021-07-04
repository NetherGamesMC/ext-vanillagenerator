$ErrorActionPreference = "Stop"

$env:PATH = "C:\php_7.3\devel;C:\php_7.3\bin;C:\php_7.3\deps\bin;$env:PATH"

$task = New-Item 'task.bat' -Force
Add-Content $task 'call phpize 2>&1'
Add-Content $task "call configure --disable-all --enable-mcgenerator 2>&1"
Add-Content $task 'nmake /nologo 2>&1'
Add-Content $task 'exit %errorlevel%'
& "C:\php_7.3\php-sdk-2.2.0\phpsdk-vc15-x64.bat" -t $task
if (-not $?) {
    rm config.nice.bat
    rm configure.bat
    rm configure.js
    rm Makefile
    rm Makefile.objects
    rm run-tests.php
    rm task.bat
    rm -r x64

    throw "building failed with errorlevel $LastExitCode"
}

Copy-Item "x64\Release_TS\php_mcgenerator.dll" "C:\php_7.3\bin\ext\php_mcgenerator.dll"

rm config.nice.bat
rm configure.bat
rm configure.js
rm Makefile
rm Makefile.objects
rm run-tests.php
rm task.bat
rm -r x64

$ini = New-Item "C:\php_7.3\bin\php.ini" -Force
Add-Content $ini "extension_dir=C:\php_7.3\bin\ext"
Add-Content $ini "extension=php_mcgenerator.dll"