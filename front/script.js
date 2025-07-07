let camposGlobales = [];
let tiposGlobales = [];
let discoConfigurado = false; // Bandera para rastrear si el disco está configurado

// Función que convierte el header a campos de búsqueda
function generarCamposBusqueda(encabezado) {
    const searchArea = document.getElementById("search-area");
    searchArea.innerHTML = ""; // Limpiar anterior

    // Inicializar select de ordenamiento
    const ordenarPorSelect = document.getElementById("ordenar-por");
    ordenarPorSelect.innerHTML = '<option value="">-- Selecciona atributo --</option>';

    // Inicializar checkboxes de columnas
    const columnasDiv = document.getElementById("columnas-checkboxes");
    columnasDiv.innerHTML = "";

    camposGlobales = [];
    tiposGlobales = [];

    const campos = encabezado.split(",");
    campos.forEach(campo => {
      const partes = campo.split(":");
      if (partes.length !== 3) {
          console.error(`Formato inválido para el campo: ${campo}`);
          return; // Saltar campos mal formateados
      }
      const [nombre, tipo, tam] = partes; // Desestructurar nombre, tipo y tamaño

      // Agregar a los arreglos globales
      camposGlobales.push(nombre);
      tiposGlobales.push(tipo);

      // Agregar opción de ordenamiento
      const option = document.createElement("option");
      option.value = nombre;
      option.textContent = nombre;
      ordenarPorSelect.appendChild(option);

      // Agregar checkbox de selección de columna
      const checkbox = document.createElement("input");
      checkbox.type = "checkbox";
      checkbox.name = "columnas";
      checkbox.value = nombre;
      checkbox.checked = true;

      const labelCheckbox = document.createElement("label");
      labelCheckbox.style.marginRight = "10px";
      labelCheckbox.appendChild(checkbox);
      labelCheckbox.appendChild(document.createTextNode(nombre));

      columnasDiv.appendChild(labelCheckbox);

      // Agregar input de búsqueda
      const div = document.createElement("div");
      div.classList.add("input-group");

      const label = document.createElement("label");
      label.textContent = `${nombre}:`;

      const input = document.createElement("input");
      input.type = "text";
      input.name = nombre;
      input.placeholder = tipo === "INT" ? "Ej: 18 o 18-30 o >18" : "Ej: Maria o Jose";

      div.appendChild(label);
      div.appendChild(input);

      searchArea.appendChild(div);
    });
}

async function guardarConfiguracion() {
    const configuracion = {
        platos: document.getElementById('platos').value,
        superficies: document.getElementById('superficies').value,
        pistas: document.getElementById('pistas').value,
        sectores: document.getElementById('sectores').value,
        capacidad: document.getElementById('capacidad').value
    };

    // Validar inputs
    if (!configuracion.platos || !configuracion.pistas || !configuracion.sectores || !configuracion.capacidad) {
        alert("Por favor, completa todos los campos de configuración.");
        return;
    }

    try {
        const response = await fetch('http://localhost:8080/configurar', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(configuracion)
        });

        const result = await response.json();
        if (result.status === 'success') {
            discoConfigurado = true;
            document.getElementById('contador-registros').textContent = 'Disco configurado. Listo para importar datos.';
            // Habilitar el botón de importar data
            document.querySelector('.data-import button').disabled = false;
            alert(result.message);
        } else {
            throw new Error(result.message);
        }
    } catch (error) {
        console.error('Error al configurar:', error);
        document.getElementById('contador-registros').textContent = `Error: ${error.message}`;
        alert(`Error al configurar: ${error.message}`);
    }
}

async function cargarHeaderDesdeTxt() {
    if (!discoConfigurado) {
        alert('Primero debes configurar el disco usando "Construir Disco".');
        return;
    }

    try {
        // Cargar datos
        const loadResponse = await fetch('http://localhost:8080/cargar-datos', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ archivo: 'datos.txt' })
        });
        const loadResult = await loadResponse.json();
     
        if (loadResult.status !== 'success') {
            throw new Error(loadResult.message);
        } else{
          console.log({loadResult: loadResult.header})
          // const lines = text.trim().split("\n");
          //generarCamposBusqueda(loadResult.header);
          alert(loadResult.message)
        }

    
      
    } catch (error) {
        console.error('Errorr cargando archivo:', error);
        console.error(error.message);
        document.getElementById('contador-registros').textContent = `Error: ${error.message}`;
        alert(`Errorr al cargar datoss: ${error.message}`);
    }
}

function buscarRegistros() {
    const campos_bus = [];
    const valores_bus = [];
    camposGlobales.forEach(attr => {
        const input = document.querySelector(`input[name='${attr}']`);
        if (input && input.value.trim()) {
            campos_bus.push(attr);
            valores_bus.push(input.value.trim());
        }
    });

    const operador = document.getElementById("operador-logico").value;
    const ordenarPor = document.getElementById("ordenar-por").value;
    const direccion = document.getElementById("orden-direccion").value;

    const columnas = Array.from(document.querySelectorAll("input[name='columnas']:checked"))
        .map(cb => cb.value);

    const payload = {
        campos_bus,
        valores_bus,
        operador,
        ordenar: {
            atributo: ordenarPor,
            direccion
        },
        columnas,
        tipos: tiposGlobales,
        nombres: camposGlobales
    };

    console.log("Enviando filtros:", payload);

    fetch('http://localhost:8080/buscar', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(payload)
    })
    .then(res => res.json())
    .then(data => {
        console.log("Respuesta:", data);
        mostrarResultados(data);
    })
    .catch(err => {
        console.error("Error al enviar datos:", err);
        alert(`Error al buscar: ${err.message}`);
    });
}

function buscarConsulta(){
    const query = document.getElementById("query").value;
    console.log({query})
    fetch('http://localhost:8080/buscar', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({query})
    })
    .then(res => res.json())
    .then(data => {
        console.log("Respuesta:", data.data, data.headers);
        mostrarResultados(data.data, data.headers);
    })
    .catch(err => {
        console.error("Error al enviar datos:", err);
        alert(`Error al buscar: ${err.message}`);
    });
}

function mostrarResultados(data, headers) {


    const thead = document.querySelector("#resultados thead");
    const tbody = document.querySelector("#resultados tbody");

    // Limpiar tabla anterior
    thead.innerHTML = "";
    tbody.innerHTML = "";

    // Cabeceras
    const headerRow = document.createElement("tr");
    headers.forEach(col => {
        const th = document.createElement("th");
        th.textContent = col;
        headerRow.appendChild(th);
    });
    thead.appendChild(headerRow);

    // Filas de datos
    data.forEach(fila => {        
        const row = document.createElement("tr");
        const tdid = document.createElement("td");
        tdid.textContent = `${fila.id}`;
        row.appendChild(tdid);
        fila.valores.forEach(celda => {
            const td = document.createElement("td");
            td.textContent = celda;
            row.appendChild(td);
        });
        tbody.appendChild(row);
    });
}