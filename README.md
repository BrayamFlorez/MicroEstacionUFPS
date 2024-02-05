Descarga el codigo y completalo con tus credenciales de aws iot core despues de crear un objeto nuevo, 
en ese momento se te entregaran por unica vez los 3 certificados necesarios para agregar al codigo en el archivo secrets.h,
el otro archivo que debes modificar es WIFI.ino, en este pondras el nombre y la clave de tu red wifi, si deseas puedes modificar el codigo 
y agregar un modulo simcard para agregar datos moviles a la estacion, el metodo actual por default es wifi, para que siga siendo movil
se recomienda compartir wifi desde el dispositivo celular de uso personal. las credenciales de la red wifi compartida deben coincidir con las 
agrregadas al archivo WIFI.ino.

CONSIDERACIONES DE CONEXIONES

1) Valida que las conexiones del dispositivo esten de acuerdo a este el siguiente esquema de conexiones: 
https://drive.google.com/file/d/1gOeZlQP1T7H-9YJbJ0GjaJTIldvMvq3I/view?usp=sharing

COMO SUBIR EL CODIGO

1. **Instalar el IDE de Arduino:**
   Asegúrate de tener instalado el IDE de Arduino en tu computadora. Puedes descargarlo desde el sitio web oficial: [Arduino IDE](https://www.arduino.cc/en/software).

2. **Instalar el Soporte para ESP32:**
   Necesitas instalar el soporte para ESP32 en el IDE de Arduino. Para hacer esto, sigue los siguientes pasos:

   - Abre el IDE de Arduino.
   - Ve a "Archivo" -> "Preferencias".
   - En la ventana de preferencias, encuentra el campo "Gestor de URLs Adicionales de Tarjetas" y agrega la siguiente URL: `https://dl.espressif.com/dl/package_esp32_index.json`.
   - Haz clic en "OK" para cerrar la ventana.
   - Ve a "Herramientas" -> "Placa" -> "Gestor de tarjetas..."
   - Busca "ESP32" e instala el paquete correspondiente.

3. **Seleccionar la Tarjeta ESP32:**
   - Conecta tu ESP32 a la computadora mediante un cable USB.
   - En el IDE de Arduino, ve a "Herramientas" -> "Placa" y selecciona tu modelo de ESP32. Puede ser algo como "ESP32 Dev Module".

4. **Configurar el Puerto:**
   - Asegúrate de que el puerto COM correcto esté seleccionado. Puedes encontrar el puerto COM en "Herramientas" -> "Puerto".

5. **Compilar el Código:**
   - Abre tu archivo MicroEstacionUFPS.ino en el IDE de Arduino.
   - Haz clic en el icono de verificación (Check) en la esquina superior izquierda para compilar tu código. Asegúrate de que no haya errores.

6. **Cargar el Código en el ESP32:**
   - Después de la compilación exitosa, haz clic en el icono de flecha derecha (Subir) para cargar el código en tu ESP32.

7. **Verificar la Consola:**
   - Abre la consola en el IDE de Arduino para ver si hay mensajes durante la carga del programa. Esto te ayudará a identificar posibles problemas.

8. **Comprobar el Funcionamiento:**
   - Después de la carga exitosa, verifica que tu programa esté funcionando correctamente en el ESP32.

Con estos pasos, deberías poder cargar tu código en el ESP32 utilizando el IDE de Arduino. Asegúrate de tener los controladores USB correctos instalados para tu placa ESP32 y seleccionar la configuración adecuada en el IDE.

¿QUIERES USAR LOS DATOS YA SUBIDOS AL REPOSITORIO?

Usa estos endpoints publicos para descargar los datos

1) CONSULTA POR CODIGO POSTAL
https://olo9de8ie8.execute-api.us-east-2.amazonaws.com/measurements/cpCity?cpCity="ACA PON EL CODIGO POSTAL QUE DESEAS CONSULTAR"
2) CONSULTA DE VARIABLES REGISTRADAS EN LA BD
https://olo9de8ie8.execute-api.us-east-2.amazonaws.com/measurements/get_tittles
