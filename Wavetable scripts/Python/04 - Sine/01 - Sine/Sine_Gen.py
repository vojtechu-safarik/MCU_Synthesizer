import numpy as np

# Parameters
sample_count = 256
frequency = 1  # Normalised frequency for one cycle (0-1)
t = np.linspace(0, 1, sample_count, endpoint=False)

# Formulas for the individual waves
x = frequency * 2 * np.pi * t

y1 = np.sin(x)

# Function to normalise into range int16
def normalize_to_int16(y):
    y = y / np.max(np.abs(y))  # normalise between -1 and 1
    return (y * 32767).astype(np.int16)

# Normalise and convert
y1_int = normalize_to_int16(y1)

# Generate header file
def generate_header(name, data):
    lines = [f"const int16_t {name}[{len(data)}] = {{"]
    for i in range(0, len(data), 8):
        row = ', '.join(f"{v:6d}" for v in data[i:i+8])
        lines.append(f"    {row},")
    lines.append("};\n")
    return '\n'.join(lines)

# Structure of the .h file
header_content = (
    "#ifndef CUSTOM_WAVETABLES_H\n"
    "#define CUSTOM_WAVETABLES_H\n\n"
    "#include <stdint.h>\n\n" +
    generate_header("sine1", y1_int) +
    "#endif // CUSTOM_WAVETABLES_H\n"
)

# Saving the file
with open("Sine_Wavetables.h", "w") as f:
    f.write(header_content)

print("File 'Sine_Wavetables.h' has been successfully created.")
