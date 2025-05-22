# build_all.ps1

param(
    [string]$Action = ""
)

if ($Action -eq "clean") {
    Write-Host "🧹 Cleaning Client_app..."
    Remove-Item -Recurse -Force "Client_app\build"

    Write-Host "🧹 Cleaning Server_app..."
    Remove-Item -Recurse -Force "Server_app\build"

    Write-Host "✅ Clean complete."
    exit
}

Write-Host "🔨 Building Client_app..."
cmake -S Client_app -B Client_app\build
mingw32-make -C Client_app\build

Write-Host "🔨 Building Server_app..."
cmake -S Server_app -B Server_app\build
mingw32-make -C Server_app\build
