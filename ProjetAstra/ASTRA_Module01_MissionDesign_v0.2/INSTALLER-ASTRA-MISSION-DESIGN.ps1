$ErrorActionPreference = 'Stop'

function Write-ProjectFile {
    param(
        [Parameter(Mandatory = $true)][string]$Destination,
        [Parameter(Mandatory = $true)][string]$Source
    )

    $parent = Split-Path -Parent $Destination
    if (-not (Test-Path -LiteralPath $parent)) {
        New-Item -ItemType Directory -Path $parent -Force | Out-Null
    }

    if (Test-Path -LiteralPath $Destination) {
        $backupRoot = Join-Path $ProjectRoot 'Saved\ASTRA_Backups\MissionDesign_v0.2'
        New-Item -ItemType Directory -Path $backupRoot -Force | Out-Null
        $stamp = Get-Date -Format 'yyyyMMdd_HHmmss_fff'
        $backupName = (Split-Path -Leaf $Destination) + '.' + $stamp + '.bak'
        Copy-Item -LiteralPath $Destination -Destination (Join-Path $backupRoot $backupName) -Force
        Write-Host "Sauvegarde : $backupName" -ForegroundColor DarkYellow
    }

    Copy-Item -LiteralPath $Source -Destination $Destination -Force
}

$ProjectRoot = (Get-Location).Path
$uprojectFiles = @(Get-ChildItem -LiteralPath $ProjectRoot -Filter '*.uproject' -File)
if ($uprojectFiles.Count -ne 1) {
    throw "Place ce script dans le dossier qui contient exactement un fichier .uproject. Fichiers détectés : $($uprojectFiles.Count)"
}

$projectFile = $uprojectFiles[0]
$projectName = [IO.Path]::GetFileNameWithoutExtension($projectFile.Name)
$projectJson = Get-Content -LiteralPath $projectFile.FullName -Raw | ConvertFrom-Json
$runtimeModules = @($projectJson.Modules | Where-Object { $_.Type -eq 'Runtime' })
if ($runtimeModules.Count -lt 1) {
    throw 'Aucun module Runtime déclaré dans le fichier .uproject.'
}

$moduleName = [string]$runtimeModules[0].Name
if ($moduleName -notmatch '^[A-Za-z][A-Za-z0-9_]*$') {
    throw "Nom de module invalide : $moduleName"
}

$moduleRoot = Join-Path $ProjectRoot ("Source\" + $moduleName)
$buildCs = Join-Path $moduleRoot ($moduleName + '.Build.cs')
if (-not (Test-Path -LiteralPath $buildCs)) {
    throw "Build.cs introuvable : $buildCs. Aucun fichier n'a été installé."
}

$packageRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$sourcePublic = Join-Path $packageRoot 'Source\Public'
$sourcePrivate = Join-Path $packageRoot 'Source\Private'
$publicDestination = Join-Path $moduleRoot 'Public\Mission'
$privateDestination = Join-Path $moduleRoot 'Private\Mission'
$dataDestination = Join-Path $ProjectRoot 'Content\ASTRA\Data\Missions'

$apiMacro = ($moduleName.ToUpperInvariant() -replace '[^A-Z0-9_]', '') + '_API'

$files = @(
    @{ Source = (Join-Path $sourcePublic 'AstraMissionTypes.h'); Destination = (Join-Path $publicDestination 'AstraMissionTypes.h') },
    @{ Source = (Join-Path $sourcePublic 'AstraMissionSubsystem.h'); Destination = (Join-Path $publicDestination 'AstraMissionSubsystem.h') },
    @{ Source = (Join-Path $sourcePrivate 'AstraMissionSubsystem.cpp'); Destination = (Join-Path $privateDestination 'AstraMissionSubsystem.cpp') },
    @{ Source = (Join-Path $packageRoot 'Mission_001_LunarSurvey.json'); Destination = (Join-Path $dataDestination 'Mission_001_LunarSurvey.json') }
)

foreach ($item in $files) {
    if (-not (Test-Path -LiteralPath $item.Source)) {
        throw "Fichier du paquet manquant : $($item.Source)"
    }
}

foreach ($item in $files[0..2]) {
    $raw = Get-Content -LiteralPath $item.Source -Raw
    $raw = $raw.Replace('__API__', $apiMacro)
    $temp = Join-Path $env:TEMP ([guid]::NewGuid().ToString() + '.tmp')
    [System.IO.File]::WriteAllText($temp, $raw, (New-Object System.Text.UTF8Encoding($false)))
    Write-ProjectFile -Destination $item.Destination -Source $temp
    Remove-Item -LiteralPath $temp -Force
}

Write-ProjectFile -Destination $files[3].Destination -Source $files[3].Source

Write-Host ''
Write-Host 'ASTRA Mission Design System v0.2 installé.' -ForegroundColor Green
Write-Host "Projet : $projectName" -ForegroundColor Cyan
Write-Host "Module C++ : $moduleName ($apiMacro)" -ForegroundColor Cyan
Write-Host "Sources publiques : $publicDestination" -ForegroundColor Cyan
Write-Host "Source privée : $privateDestination" -ForegroundColor Cyan
Write-Host "Scénario : $($files[3].Destination)" -ForegroundColor Cyan
Write-Host 'Le catalogue Mission 001 peut être initialisé depuis Blueprint avec Initialize Mission 001 Defaults.' -ForegroundColor Yellow
Write-Host 'Étape suivante obligatoire : compiler le projet depuis UnrealBuildTool.' -ForegroundColor Yellow
