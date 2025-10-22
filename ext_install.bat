@echo off
setlocal ENABLEDELAYEDEXPANSION

REM ====== НАСТРОЙКИ ======
set REPO_ROOT=%~dp0
set VCPKG_DIR=%REPO_ROOT%external\vcpkg
set PS=PowerShell -NoProfile -ExecutionPolicy Bypass

REM ====== 0) Предпроверки ======
where git >NUL 2>&1 || (echo [ERR] Git не найден в PATH & exit /b 1)
where cmake >NUL 2>&1 || (echo [ERR] CMake не найден в PATH (установи VS2022 с CMake) & exit /b 1)

REM ====== 1) Подтянуть сабмодуль vcpkg (РОВНО один раз хранится в external/vcpkg) ======
echo [INFO] Обновляем сабмодуль vcpkg...
git submodule sync --recursive || (echo [ERR] git submodule sync & exit /b 1)
git submodule update --init --recursive --depth 0 external/vcpkg || (echo [ERR] git submodule update & exit /b 1)

IF NOT EXIST "%VCPKG_DIR%\bootstrap-vcpkg.bat" (
  echo [ERR] Не найден %VCPKG_DIR%\bootstrap-vcpkg.bat
  exit /b 1
)

REM ====== 2) Обход tls12-download.exe и телеметрии ======
set VCPKG_FORCE_SYSTEM_BINARIES=1
set VCPKG_DISABLE_METRICS=1

REM ====== 3) Разблокируем возможные .exe (SmartScreen) ======
%PS% -Command "Get-ChildItem '%VCPKG_DIR%\scripts\*.exe' -ErrorAction SilentlyContinue ^| ForEach-Object { try { Unblock-File $_.FullName } catch {} }"

REM ====== 4) Ninja (не обязателен, но ускоряет). Попробуем найти; при отсутствии мягко продолжаем ======
where ninja >NUL 2>&1
IF ERRORLEVEL 1 (
  echo [WARN] Ninja не найден в PATH. Продолжаю без Ninja...
)

REM ====== 5) Bootstrap локального vcpkg из сабмодуля ======
IF EXIST "%VCPKG_DIR%\vcpkg.exe" (
  echo [INFO] vcpkg.exe уже существует, проверяем версию...
  "%VCPKG_DIR%\vcpkg.exe" version || del /q "%VCPKG_DIR%\vcpkg.exe"
)

IF NOT EXIST "%VCPKG_DIR%\vcpkg.exe" (
  echo [INFO] Запускаю bootstrap-vcpkg...
  call "%VCPKG_DIR%\bootstrap-vcpkg.bat" -disableMetrics
  IF ERRORLEVEL 1 (
    echo.
    echo [ERR] bootstrap-vcpkg завершился с ошибкой.
    echo Подсказки: проверь прокси (HTTP(S)_PROXY), добавь '%VCPKG_DIR%\scripts\' в исключения антивируса.
    exit /b 1
  )
)

IF NOT EXIST "%VCPKG_DIR%\vcpkg.exe" (
  echo [ERR] vcpkg.exe не создан. Проверь сеть/антивирус и повтори.
  exit /b 1
)
echo [OK] vcpkg готов.

REM ====== 6) Сборка пресетов ======
echo [BUILD] qt-client Debug (config)...
cmake --preset qt-client-vs2022-debug || exit /b 1
echo [BUILD] qt-client Debug (build)...
cmake --build --preset qt-client-vs2022-debug || exit /b 1

echo [BUILD] server Debug (config)...
cmake --preset server-vs2022-debug || exit /b 1
echo [BUILD] server Debug (build)...
cmake --build --preset server-vs2022-debug || exit /b 1

echo.
echo ============================
echo ✅ Готово. Бинарники в папках из пресетов (обычно out\build\...).
echo ============================
exit /b 0
