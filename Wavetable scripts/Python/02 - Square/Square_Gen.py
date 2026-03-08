import numpy as np

# Parameters
sample_count = 256
frequency = 1  # Normalised frequency for one cycle (0-1)
t = np.linspace(0, 1, sample_count, endpoint=False)
phase = (frequency * t) % 1.0

# Duty cycles in percent for the 8 shapes
duty_percents = [50, 47.5, 45, 42.5, 40, 37.5, 35, 32.5, 30, 27.5, 25, 22.5, 20, 17.5, 15, 12.5, 10, 7.5, 5, 2.5]

# Generate square waves with specified duty cycles
wave_tables = []
for d in duty_percents:
    duty = d / 100.0
    # 1 when phase < duty, otherwise -1
    y = np.where(phase < duty, 1.0, -1.0)
    wave_tables.append(y)

# Function to normalise into range int16
def normalize_to_int16(y):
    y = y / np.max(np.abs(y))  # normalise between -1 and 1 (already ±1 but keep for safety)
    return (y * 32767).astype(np.int16)

# Normalise and convert
wave_tables_int = [normalize_to_int16(y) for y in wave_tables]

# Generate header file
def generate_header(name, data):
    lines = [f"const int16_t {name}[{len(data)}] = {{"]
    for i in range(0, len(data), 8):
        row = ', '.join(f"{v:6d}" for v in data[i:i+8])
        lines.append(f"    {row},")
    lines.append("};\n")
    return '\n'.join(lines)

header_guard = "CUSTOM_SQUARE_WAVETABLES_H"
header_content = (
    f"#ifndef {header_guard}\n"
    f"#define {header_guard}\n\n"
    "#include <stdint.h>\n\n"
)

# Add each square table as square1..square8
for idx, data in enumerate(wave_tables_int, start=1):
    header_content += generate_header(f"square{idx}", data)

header_content += f"#endif // {header_guard}\n"

# Saving the file
with open("Square_Wavetables.h", "w") as f:
    f.write(header_content)

print("File 'Square_Wavetables.h' has been successfully created.")
