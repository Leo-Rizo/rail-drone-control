# Contributing to RailDrone Control System

Este documento define el flujo de trabajo colaborativo del proyecto.

## Regla principal

No se debe trabajar directamente sobre la rama `main`.

Todo cambio debe realizarse en una rama independiente y posteriormente integrarse mediante un Pull Request.

## Flujo de trabajo

Antes de comenzar una tarea:

```bash
git checkout main
git pull
```

Después se debe crear una rama nueva:

```bash
git checkout -b tipo/nombre-de-la-tarea
```

## Tipos de ramas

- `feature/` para nuevas funcionalidades
- `fix/` para correcciones
- `docs/` para documentación
- `test/` para pruebas
- `refactor/` para reorganización de código sin cambiar funcionalidad

### Ejemplos

```text
feature/bno085
feature/tof
feature/gamepad
feature/altitude-control
feature/angle-control
fix/imu-timeout
docs/control-architecture
```

## Commits

Los commits deben ser pequeños y describir claramente el cambio realizado.

Formato recomendado:

```text
tipo: descripción breve
```

### Ejemplos

```text
feat: add BNO085 sensor interface
fix: handle invalid ToF readings
docs: document control architecture
test: add PID unit tests
refactor: separate motor mixer module
```

Tipos recomendados:

- `feat` nueva funcionalidad
- `fix` corrección de error
- `docs` documentación
- `test` pruebas
- `refactor` reorganización de código
- `chore` mantenimiento del proyecto

## Antes de hacer un commit

Verificar los archivos modificados:

```bash
git status
```

Agregar los archivos correspondientes:

```bash
git add .
```

Crear el commit:

```bash
git commit -m "tipo: descripción"
```

Subir la rama:

```bash
git push -u origin nombre-de-la-rama
```

## Pull Requests

Todo cambio hacia `main` debe realizarse mediante Pull Request.

Antes de solicitar el merge se debe comprobar:

- El código compila.
- No se incluyen archivos generados.
- No se incluyen contraseñas, tokens o secretos.
- La funcionalidad fue probada cuando sea posible.
- La documentación fue actualizada si el cambio lo requiere.
- El Pull Request describe claramente qué se modificó.

## Firmware

El firmware del ESP32 se encuentra en:

```text
firmware/
```

El proyecto utiliza VS Code y PlatformIO.

Para compilar el firmware:

```bash
cd firmware
pio run
```

También se puede utilizar el botón Build de PlatformIO en VS Code.

El firmware debe compilar correctamente antes de integrarse a `main`.

## Estructura del proyecto

```text
firmware/       Código del ESP32
interface/      Dashboard y supervisión
simulation/     Modelado y simulación
electronics/    Electrónica y alimentación
mechanical/     Diseño mecánico
docs/           Documentación técnica
```

## Archivos que no deben subirse

No se deben agregar al repositorio:

- Archivos compilados de PlatformIO
- Carpetas `.pio/`
- Contraseñas
- Tokens
- Archivos `.env`
- Archivos temporales
- Archivos generados automáticamente por el IDE

## Integración

Una vez aprobado el Pull Request, los cambios se integran a `main`.

Después del merge, cada integrante debe actualizar su copia local:

```bash
git checkout main
git pull
```

La siguiente tarea debe comenzar desde una rama nueva creada a partir de la versión actualizada de `main`.
