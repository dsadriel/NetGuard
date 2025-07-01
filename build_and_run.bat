@echo off
setlocal

set BUILD_DIR=build

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake ..
cmake --build .

if exist ..\bin\Debug\main.exe (
    start ..\bin\Debug\main.exe
) else if exist ..\bin\Release\main.exe (
    start ..\bin\Release\main.exe
) else if exist ..\bin\main.exe (
    start ..\bin\main.exe
) else (
    echo "ERRO: O executavel main.exe nao foi encontrado."
    exit /b 1
)

cd ..
endlocal