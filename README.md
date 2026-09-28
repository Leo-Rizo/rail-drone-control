# RailDrone Control System

Sistema mecatrónico desarrollado para la materia de Control y Diseño de Interfaces Analógicas y Digitales.

## Objetivo

Diseñar e implementar una plataforma aérea acoplada a un carro guiado por riel capaz de controlar su posición vertical y orientación angular mediante motores BLDC, sensores inerciales y sensores de distancia.

## Arquitectura inicial

El sistema utilizará un ESP32 como controlador principal.

Principales subsistemas:

- ESP32
- IMU BNO085
- Sensor ToF
- Limit switches
- Motores BLDC
- ESC
- Hélices
- Gamepad PlayStation
- Sistema de alimentación
- Control de altura
- Control angular
- Motor mixer
- Telemetría
- Interfaz gráfica

## Estructura del repositorio

```text
firmware/       Firmware del ESP32
interface/      Dashboard y supervisión
simulation/     Modelado y simulación
electronics/    Electrónica y alimentación
mechanical/     Diseño mecánico
docs/           Documentación técnica
## Estado

Proyecto en fase inicial de arquitectura y diseño.

## Equipo

Proyecto académico desarrollado para la materia de Control y Diseño de Interfaces Analógicas y Digitales.
