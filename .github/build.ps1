$ErrorActionPreference = "Stop"

$env:PATH = "C:\php\devel;C:\php\bin;C:\php\deps\bin;$env:PATH"

$task = New-Item 'task.bat' -Force
Add-Content $task 'call phpize 2>&1'
Add-Content $task "call configure --disable-all --enable-vanillagenerator 2>&1"
Add-Content $task 'nmake /nologo 2>&1'
Add-Content $task 'exit %errorlevel%'
& "C:\php\php-sdk-2.2.0\phpsdk-vs16-x64.bat" -t $task
if (-not $?) {
    throw "building failed with errorlevel $LastExitCode"
}

Copy-Item "x64\Release_TS\php_vanillagenerator.dll" "C:\php\bin\ext\php_vanillagenerator.dll"

$ini = New-Item "C:\php\bin\php.ini" -Force
Add-Content $ini "extension_dir=C:\php\bin\ext"
Add-Content $ini "extension=php_vanillagenerator.dll"
