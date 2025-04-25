
@echo off
:menu
cls

echo Which program do you want to run?
echo 1. Server
echo 2. Learner
echo 3. Mentor
echo 4. Exit
set /p choice=Enter choice (1, 2, 3, or 4): 

if "%choice%"=="1" (
    gcc server.c getPassword.c setColor.c -o server.exe
    if errorlevel 1 (
        echo Compilation failed.
        pause
        exit /b
    )
    server.exe
    goto menu
) else if "%choice%"=="2" (
    gcc learner.c getPassword.c setColor.c -o learner.exe -lws2_32
    if errorlevel 1 (
        echo Compilation failed.
        pause
        exit /b
    )
    call learner.exe
    goto menu
) else if "%choice%"=="3" (
    gcc mentor.c getPassword.c setColor.c -o mentor.exe -lws2_32
    if errorlevel 1 (
        echo Compilation failed.
        pause
        exit /b
    )
    call mentor.exe
    goto menu
) else if "%choice%"=="4" (
    echo Exiting...
    pause
    exit /b
) else (
    echo Invalid option.
    pause
    goto menu
)