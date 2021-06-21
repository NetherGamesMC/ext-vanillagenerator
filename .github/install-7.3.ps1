$ErrorActionPreference = "Stop"

if (-not (Test-Path 'C:\php_7.3')) {
    [void](New-Item 'C:\php_7.3' -ItemType 'directory')
}

# PHP SDK
$bname = "php-sdk-2.2.0.zip"
if (-not (Test-Path C:\php_7.3\$bname)) {
    echo "Download: https://github.com/microsoft/php-sdk-binary-tools/archive/$bname"
    Invoke-WebRequest "https://github.com/microsoft/php-sdk-binary-tools/archive/$bname" -OutFile "C:\php_7.3\$bname"
}
$dname0 = "php-sdk-binary-tools-php-sdk-2.2.0"
$dname1 = "php-sdk-2.2.0"
if (-not (Test-Path "C:\php_7.3\$dname1")) {
    Expand-Archive "C:\php_7.3\$bname" "C:\php_7.3"
    Move-Item "C:\php_7.3\$dname0" "C:\php_7.3\$dname1"
}

# PHP releases
Invoke-WebRequest "https://windows.php.net/downloads/releases/releases.json" -OutFile "C:\php_7.3\releases.json"
$php_version = (Get-Content -Path "C:\php_7.3\releases.json" | ConvertFrom-Json | ForEach-Object {
    return "7.3.28"
})

$bname = "php-devel-pack-$php_version$ts_part-Win32-VC15-x64.zip"
if (-not (Test-Path "C:\php_7.3\$bname")) {
    try {
        echo "Download: https://windows.php.net/downloads/releases/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/$bname" -OutFile "C:\php_7.3\$bname"
    } catch [System.Net.WebException] {
        echo "Downlaod: https://windows.php.net/downloads/releases/archives/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/archives/$bname" -OutFile "C:\php_7.3\$bname"
    }
}

$dname = "php-$php_version-devel-VC15-x64"
if (-not (Test-Path "C:\php_7.3\devel")) {
    Expand-Archive "C:\php_7.3\$bname" 'C:\php_7.3'
    if (-not (Test-Path "C:\php_7.3\$dname")) {
        $php_normalize_version = $php_version.Split("-")[0]
        $dname = "php-$php_normalize_version-devel-VC15-x64"
    }
    if (-not (Test-Path "C:\php_7.3\devel")) {
        Move-Item "C:\php_7.3\$dname" "C:\php_7.3\devel"
    }
}

# PHP binary: "C:\php_7.3\bin"
$bname = "php-$php_version$ts_part-Win32-VC15-x64.zip"
if (-not (Test-Path "C:\php_7.3\$bname")) {
    try {
        echo "Download: https://windows.php.net/downloads/releases/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/$bname" -OutFile "C:\php_7.3\$bname"
    } catch [System.Net.WebException] {
        echo "Download: https://windows.php.net/downloads/releases/archives/$bname"
        Invoke-WebRequest "https://windows.php.net/downloads/releases/archives/$bname" -OutFile "C:\php_7.3\$bname"
    }
}

if (-not (Test-Path "C:\php_7.3\bin")) {
    Expand-Archive "C:\php_7.3\$bname" "C:\php_7.3\bin"
}