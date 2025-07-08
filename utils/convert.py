import csv
import re

# Leer el archivo struct_table.txt
with open('../struct_table.txt', 'r', encoding='utf-8') as f:
    sql = f.read()

# Parsear el SQL para generar el header personalizado
def parse_sql(sql):
    campos = []
    for line in sql.splitlines():
        line = line.strip().strip(',').strip(');')
        if not line or '(' in line and "CREATE TABLE" in line:
            continue
        partes = line.split()
        nombre = partes[0]
        tipo_raw = partes[1].lower()

        # Extraer el tamaño entre paréntesis
        size_match = re.search(r'\((\d+)', tipo_raw)
        size = size_match.group(1) if size_match else "10"

        if tipo_raw.startswith("integer"):
            campos.append(f"{nombre}:int:{size}")
        elif tipo_raw.startswith("varchar"):
            campos.append(f"{nombre}:varchar:{size}")
        elif tipo_raw.startswith("decimal"):
            campos.append(f"{nombre}:double:{size}")
    return '|'.join(campos)

# Leer el CSV con configuración correcta
with open('../taxables.csv', newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, quotechar='"', skipinitialspace=True)
    filas = list(reader)

# Escribir archivo de salida en formato compatible con tu sistema
with open('../datos.txt', 'w', encoding='utf-8') as out:
    header_txt = parse_sql(sql)
    out.write(header_txt + '\n')
    for fila in filas[1:]:  # Saltar encabezado del CSV
        fila_limpia = []
        for i, campo in enumerate(fila):
            campo = campo.strip().strip('"')
            if i == 1:  # Campo Item
                campo = campo.replace(',', ',')  # o .replace(',', ' ')
            fila_limpia.append(campo)
        out.write('|'.join(fila_limpia) + '\n')