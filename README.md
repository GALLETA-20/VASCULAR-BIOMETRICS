# Vessel Biometric
---
## Integrantes
- URBINA ARÉVALO VITALY PAOLO
- VIERA LIVIAPOMA MARITZA BELEN
- CASTRO NEYRA ADRIANA LIZBETH
- RIOFRIO CASTILLO JAIR ALONSO
- SANDOVAL COVEÑA ALEXIS JEAN PIERRE
- LOZADA LACHIRA LUIS DAVID
- VILCHEZ CRISANTO JOSUE JOEL

## Descripción

Este proyecto tiene como objetivo usar el algoritmo de Frangi multiescala
para filtrar una imagen infrarroja de la palma de la mano y extraer el
patrón vascular subcutáneo. El resultado es una imagen donde las venas
aparecen resaltadas sobre el tejido circundante.

---

## Fundamento físico

Para lograr que las venas sean visibles a través de la cámara,
se aplican tres estrategias fundamentales:

**Primera — longitud de onda infrarroja:** La luz dentro del rango de
720nm a 1000nm puede penetrar la piel, ya que el tejido es parcialmente
translúcido en esas longitudes de onda. La hemoglobina presente en la
sangre absorbe fuertemente la radiación infrarroja; la luz que no es
absorbida es reflejada por el tejido circundante hacia la cámara. El
resultado es un patrón de zonas oscuras que corresponden a las venas,
donde la luz fue absorbida y no llegó al sensor. Para este proyecto se
utilizaron LEDs de 3mm con una emisión infrarroja de 850nm.

**Segunda — sensor sin filtro IR:** La cámara debe ser capaz de percibir
esta longitud de onda. Se utilizó el módulo de cámara Raspberry Pi 5MP
OV5647, cuya característica clave es la ausencia del filtro de corte
infrarrojo (IR-cut filter) que traen la mayoría de cámaras convencionales.

**Tercera — filtro paso banda:** Al no tener filtro IR, el sensor captará
cualquier fuente de radiación infrarroja del entorno, lo que contamina la
imagen. Esto se resuelve con un filtro paso banda que solo permite el paso
de luz dentro del rango de 800nm a 1100nm, bloqueando tanto la luz visible
como el IR fuera de ese rango.

---

## Hardware

| Componente | Descripción |
|---|---|
| Raspberry Pi 4B | Unidad de procesamiento principal |
| OV5647 5MP | Cámara sin filtro IR |
| LEDs IR 850nm 3mm | Fuente de iluminación infrarroja |
| Filtro paso banda 800-1100nm | Bloquea IR ambiental no deseado |

---

## Software y lenguaje

Se utilizó **C++** como lenguaje principal debido a su alta eficiencia
y buena integración con el hardware. A diferencia de Python, que es un
lenguaje interpretado y consume recursos de forma considerable, C++ compila
a código nativo y consume únicamente los recursos necesarios, lo que evita
sobrecargar el procesador de la Raspberry Pi.

### Dependencias

- `libgpiod` — control de pines GPIO para activar los LEDs IR
- `rpicam-still` — captura de imagen desde la cámara OV5647
- `OpenCV 4` — procesamiento de imagen
- `Streamlit` — visualización de resultados en interfaz web local

---

## Resultados de procesamiento

A continuación se muestra el flujo de transformación de la imagen, desde la captura cruda en infrarrojo hasta la extracción final del patrón vascular mediante el algoritmo de Frangi.

<p align="center">
  <img src="images/1_ir_original.png" width="30%" alt="Imagen IR Original" />
  <img src="images/2_clahe_enhancement.png" width="30%" alt="Mejora con CLAHE" />
  <img src="images/3_frangi_vesselness.png" width="30%" alt="Resultado Filtro Frangi" />
</p>

<p align="center">
  <b>Figura 1.</b> Izquierda: Captura IR cruda. Centro: Mejora de contraste adaptativo (CLAHE). Derecha: Mapa de vesselness final de Frangi.
</p>

--
