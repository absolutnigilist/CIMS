@echo off
setlocal ENABLEDELAYEDEXPANSION
REM ============================================================
REM CIMS - инсталляция и сборка (всё из репозитория)
REM Требования: Git, CMake (VS2022), (опц.) Ninja, PowerShell
REM Запускать из корня репо (где лежит этот install.bat)
REM ============================================================

set "REPO_ROOT=%~dp0"
pushd "%REPO_ROOT%" >NUL

REM --- 0) Проверки инструментов ---
where git >NUL 2>&1 || (echo [ERR] Git не найден в PATH & exit /b 1)
where cmake >NUL 2>&1 || (echo [ERR] CMake не найден в PATH (установи VS2022 с CMake) & exit /b 1)

REM --- 1) Включить длинные пути в Git (если нет прав на system, делаем global) ---
git config --system core.longpaths true >NUL 2>&1
if ERRORLEVEL 1 (
  echo [WARN] Нет прав на --system, настраиваем --global...
  git config --global core.longpaths true || (echo [ERR] Не удалось включить longpaths & exit /b 1)
) else (
  echo [OK] Git longpaths включен
)

REM --- 2) Инициализация сабмодуля vcpkg (ровно один, по пути external/vcpkg) ---
echo [INFO] Инициализируем сабмодуль vcpkg...
git submodule sync --recursive || (echo [ERR] git submodule sync & exit /b 1)
git submodule update --init --recursive --depth 0 external/vcpkg || (echo [ERR] git submodule update & exit /b 1)

if not exist "external\vcpkg\bootstrap-vcpkg.bat" (
  echo [ERR] Не найден external\vcpkg\bootstrap-vcpkg.bat
  exit /b 1
)

REM --- 3) Настройки окружения для предсказуемого bootstrap ---
set "VCPKG_FORCE_SYSTEM_BINARIES=1"   REM обходим scripts\tls12-download.exe, используем системные cmake/ninja
set "VCPKG_DISABLE_METRICS=1"         REM отключаем телеметрию

REM --- 4) Разблокируем возможные .exe (SmartScreen/AV) ---
for /f "usebackq delims=" %%P in (`where powershell 2^>NUL`) do set "PS=%%P"
if defined PS (
  "%PS%" -NoProfile -ExecutionPolicy Bypass -Command ^
    "Get-ChildItem '%CD%\external\vcpkg\scripts\*.exe' -ErrorAction SilentlyContinue ^| ForEach-Object { try { Unblock-File $_.FullName } catch {} }" >NUL 2>&1
)

REM --- 5) Ninja (не обязателен). Если есть — ок; если нет — просто предупреждение ---
where ninja >NUL 2>&1
if ERRORLEVEL 1 (
  echo [WARN] Ninja не найден в PATH. Продолжаем без Ninja...
) else (
  echo [OK] Ninja найден
)

REM --- 6) Bootstrap локального vcpkg (только если нужно) ---
set "VCPKG_EXE=external\vcpkg\vcpkg.exe"
if exist "%VCPKG_EXE%" (
  echo [INFO] vcpkg уже существует. Проверяем версию...
  "%VCPKG_EXE%" version >NUL 2>&1
  if ERRORLEVEL 1 (
    echo [WARN] Старая/битая версия vcpkg. Переинициализируем...
    del /q "%VCPKG_EXE%" >NUL 2>&1
  )
)

if not exist "%VCPKG_EXE%" (
  echo [INFO] Запускаем bootstrap vcpkg (локальный сабмодуль)...
  call external\vcpkg\bootstrap-vcpkg.bat -disableMetrics
  if ERRORLEVEL 1 (
    echo.
    echo [ERR] bootstrap-vcpkg завершился с ошибкой.
    echo Подсказки: проверь прокси (HTTP_PROXY/HTTPS_PROXY), добавь ^"external\vcpkg\scripts\^" в исключения антивируса.
    exit /b 1
  )
)

if not exist "%VCPKG_EXE%" (
  echo [ERR] vcpkg.exe не создан. Проверь сеть/антивирус и повтори.
  exit /b 1
)
echo [OK] vcpkg готов.

REM --- 7) Сборка пресетов ---
echo [BUILD] Qt-клиент Debug: configure...
cmake --preset qt-client-vs2022-debug || exit /b 1
echo [BUILD] Qt-клиент Debug: build...
cmake --build --preset qt-client-vs2022-debug || exit /b 1

echo [BUILD] Сервер Debug: configure...
cmake --preset server-vs2022-debug || exit /b 1
echo [BUILD] Сервер Debug: build...
cmake --build --preset server-vs2022-debug || exit /b 1

echo.
echo ============================================
echo  ✅ Готово. Бинарники в папках из пресетов
echo  (.\out\build\...\Debug\.exe)
echo ============================================
popd >NUL
exit /b 0
