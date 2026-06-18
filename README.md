# Ejercicio 2: Gastos Personales

## ¿Qué patrón desacopla qué parte del diseño?

1. **Factory Method (`make_exporter`)**: Desacopla la lógica de *creación* de los exportadores de la lógica cliente. El cliente no necesita usar condicionales (`if`/`else`) para instanciar manualmente el formato de salida; puede pedir directamente una función que devuelva un string.
2. **Strategy (`sort_with`)**: Desacopla el *comportamiento de ordenamiento* de los datos en sí. Permite inyectar lambdas o functors evaluables en tiempo de ejecución para decidir si los gastos se ordenan por monto, fecha o categoría, sin modificar la función que los ordena o la estructura de datos.
3. **Decorator (`AuditedExporter`, `SummaryExporter`)**: Desacopla el *añadido de nuevas funcionalidades* (como sumar el total o registrar un log de auditoría) de la exportación base (CSV, JSON, etc.). Permite envolver el comportamiento dinámicamente o en tiempo de compilación sin alterar el código de los exportadores originales.