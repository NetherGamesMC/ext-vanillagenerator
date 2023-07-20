$ErrorActionPreference = "Stop"

if (-not (Test-Path 'C:\php')) {
    [void](New-Item 'C:\php' -ItemType 'directory')
}

# PHP SDK
$bname = "php-sdk-2.2.0.zip"
if (-not (Test-Path C:\php\$bname)) {
    echo "Download: https://github.com/microsoft/php-sdk-binary-tools/archive/$bname"
    Invoke-WebRequest "https://github.com/microsoft/php-sdk-binary-tools/archive/$bname" -OutFile "C:\php\$bname"
}
$dname0 = "php-sdk-binary-tools-php-sdk-2.2.0"
$dname1 = "php-sdk-2.2.0"
if (-not (Test-Path "C:\php\$dname1")) {
    Expand-Archive "C:\php\$bname" "C:\php"
    Move-Item "C:\php\$dname0" "C:\php\$dname1"
}

# PHP releases
Invoke-WebRequest "https://windows.php.net/downloads/releases/releases.json" -OutFile "C:\php\releases.json"
$php_version = (Get-Content -Path "C:\php\releases.json" | ConvertFrom-Json | ForEach-Object {
    return "8.2.7"
})

# PHP devel pack: "C:\php\devel"
$ts_part = ''
if ('nts' -eq $env:TS) {
    $ts_part = '-nts'
}
$bname = "php-devel-pack-$php_version$ts_part-Win32-vs16-x64.zip"
if (-not (Test-Path "C:\php\$bname")) {
    try {
        echo "Download: https://windows.php.net/downloads/releases/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/$bname" -OutFile "C:\php\$bname"
    } catch [System.Net.WebException] {
        echo "Downlaod: https://windows.php.net/downloads/releases/archives/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/archives/$bname" -OutFile "C:\php\$bname"
    }
}
$dname = "php-$php_version-devel-vs16-x64"
if (-not (Test-Path "C:\php\devel")) {
    Expand-Archive "C:\php\$bname" 'C:\php'
    if (-not (Test-Path "C:\php\$dname")) {
        $php_normalize_version = $php_version.Split("-")[0]
        $dname = "php-$php_normalize_version-devel-vs16-x64"
    }
    if (-not (Test-Path "C:\php\devel")) {
        Move-Item "C:\php\$dname" "C:\php\devel"
    }
}

# PHP binary: "C:\php\bin"
$bname = "php-$php_version$ts_part-Win32-vs16-x64.zip"
if (-not (Test-Path "C:\php\$bname")) {
    try {
        echo "Download: https://windows.php.net/downloads/releases/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/$bname" -OutFile "C:\php\$bname"
    } catch [System.Net.WebException] {
        echo "Download: https://windows.php.net/downloads/releases/archives/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/archives/$bname" -OutFile "C:\php\$bname"
    }
}
if (-not (Test-Path "C:\php\bin")) {
    Expand-Archive "C:\php\$bname" "C:\php\bin"
}