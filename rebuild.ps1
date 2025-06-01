# Clean previous build files
Write-Host "Cleaning previous build files..." -ForegroundColor Green
if (Test-Path out) {
    Remove-Item -Path out -Recurse -Force
}

# Regenerate makefiles
Write-Host "Regenerating makefiles..." -ForegroundColor Green
opp_makemake -f --deep

# Build the project
Write-Host "Building project..." -ForegroundColor Green
nmake -f Makefile.vc

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful!" -ForegroundColor Green
    Write-Host "Run the simulation with:" -ForegroundColor Cyan
    Write-Host ".\out\clang-release\src\CNETFINAL.exe -f simulations\omnetpp.ini" -ForegroundColor Cyan
} else {
    Write-Host "Build failed!" -ForegroundColor Red
} 