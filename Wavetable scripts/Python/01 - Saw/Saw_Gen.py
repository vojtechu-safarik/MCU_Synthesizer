import numpy as np

# Parameters
sample_count = 256
frequency = 1  # Normalised frequency for one cycle (0-1)
t = np.linspace(0, 1, sample_count, endpoint=False)

# Sawtooth formula
x = frequency * 2 * np.pi * t
y_base = -2 / np.pi * np.arctan(1 / np.tan(x / 2))

# === y1: Base sawtooth ===

y1 = y_base

# === y2: Base sawtooth + amplified 2nd harmonic ===

# FFT analysis
Y2 = np.fft.fft(y_base)
frequencies = np.fft.fftfreq(len(y_base), d=1/sample_count) # Universal for all Sawtooth functions

harmonic_number_y2 = 2
gain_y2 = 3.0

target_freq_y2 = frequency * harmonic_number_y2
target_index_y2 = np.argmin(np.abs(frequencies - target_freq_y2))

Y2[target_index_y2] *= gain_y2
Y2[-target_index_y2] *= gain_y2

y2 = np.fft.ifft(Y2).real

# === y3: Base sawtooth + amplified 3rd harmonic ===

# FFT analysis
Y3 = np.fft.fft(y_base)

harmonic_number_y3 = 3
gain_y3 = 3.0

target_freq_y3 = frequency * harmonic_number_y3
target_index_y3 = np.argmin(np.abs(frequencies - target_freq_y3))

Y3[target_index_y3] *= gain_y3
Y3[-target_index_y3] *= gain_y3

y3 = np.fft.ifft(Y3).real

# === y4: Base sawtooth + amplified 4th harmonic ===

# FFT analysis
Y4 = np.fft.fft(y_base)

harmonic_number_y4 = 4
gain_y4 = 3.0

target_freq_y4 = frequency * harmonic_number_y4
target_index_y4 = np.argmin(np.abs(frequencies - target_freq_y4))

Y4[target_index_y4] *= gain_y4
Y4[-target_index_y4] *= gain_y4

y4 = np.fft.ifft(Y4).real

# === y5: Base sawtooth + amplified 5th harmonic ===

# FFT analysis
Y5 = np.fft.fft(y_base)

harmonic_number_y5 = 5
gain_y5 = 4.0

target_freq_y5 = frequency * harmonic_number_y5
target_index_y5 = np.argmin(np.abs(frequencies - target_freq_y5))

Y5[target_index_y5] *= gain_y5
Y5[-target_index_y5] *= gain_y5

y5 = np.fft.ifft(Y5).real

# === y6: Base sawtooth + amplified 6th harmonic ===

# FFT analysis
Y6 = np.fft.fft(y_base)

harmonic_number_y6 = 6
gain_y6 = 4.0

target_freq_y6 = frequency * harmonic_number_y6
target_index_y6 = np.argmin(np.abs(frequencies - target_freq_y6))

Y6[target_index_y6] *= gain_y6
Y6[-target_index_y6] *= gain_y6

y6 = np.fft.ifft(Y6).real

# === y7: Base sawtooth + amplified even harmonics [2,4,6,8,10,12] ===
Y7 = np.fft.fft(y_base)

harmonics_to_boost_y7 = [2, 4, 6, 8, 10, 12]
gains_y7 = [3, 3, 4, 4, 5, 5] # respective amplification factors

for harmonic_number_y7, gain_y7 in zip(harmonics_to_boost_y7, gains_y7):
    
    target_freq_y7 = frequency * harmonic_number_y7
    target_index_y7 = np.argmin(np.abs(frequencies - target_freq_y7))
    
    Y7[target_index_y7] *= gain_y7
    Y7[-target_index_y7] *= gain_y7

y7 = np.fft.ifft(Y7).real

# === y8: Base sawtooth + amplified odd harmonics [1,3,5,7,9] ===
Y8 = np.fft.fft(y_base)

harmonics_to_boost_y8 = [1, 3, 5, 7, 9]
gains_y8 = [3, 3, 4, 4, 5] # respective amplification factors

for harmonic_number_y8, gain_y8 in zip(harmonics_to_boost_y8, gains_y8):
    
    target_freq_y8 = frequency * harmonic_number_y8
    target_index_y8 = np.argmin(np.abs(frequencies - target_freq_y8))
    
    Y8[target_index_y8] *= gain_y8
    Y8[-target_index_y8] *= gain_y8

y8 = np.fft.ifft(Y8).real

# === y9: Base sawtooth + amplified lower + higher harmonics [1,2,3,6,10] ===
Y9 = np.fft.fft(y_base)

harmonics_to_boost_y9 = [1, 2, 3, 6, 10]
gains_y9 = [3, 3, 3, 4, 5] # respective amplification factors

for harmonic_number_y9, gain_y9 in zip(harmonics_to_boost_y9, gains_y9):
    
    target_freq_y9 = frequency * harmonic_number_y9
    target_index_y9 = np.argmin(np.abs(frequencies - target_freq_y9))
    
    Y9[target_index_y9] *= gain_y9
    Y9[-target_index_y9] *= gain_y9

y9 = np.fft.ifft(Y9).real

# === y10: Base sawtooth + amplified various non-harmonic harmonics [1,4,7,11] ===
Y10 = np.fft.fft(y_base)

harmonics_to_boost_y10 = [1, 4, 7, 11]
gains_y10 = [3, 3, 4, 5] # respective amplification factors

for harmonic_number_y10, gain_y10 in zip(harmonics_to_boost_y10, gains_y10):
    
    target_freq_y10 = frequency * harmonic_number_y10
    target_index_y10 = np.argmin(np.abs(frequencies - target_freq_y10))
    
    Y10[target_index_y10] *= gain_y10
    Y10[-target_index_y10] *= gain_y10

y10 = np.fft.ifft(Y10).real

# === y11: Base sawtooth + amplified wide "non-musical" spectrum [2,5,9,13,17] ===
Y11 = np.fft.fft(y_base)

harmonics_to_boost_y11 = [2, 5, 9, 13, 17]
gains_y11 = [3, 3, 4, 5, 5] # respective amplification factors

for harmonic_number_y11, gain_y11 in zip(harmonics_to_boost_y11, gains_y11):
    
    target_freq_y11 = frequency * harmonic_number_y11
    target_index_y11 = np.argmin(np.abs(frequencies - target_freq_y11))
    
    Y11[target_index_y11] *= gain_y11
    Y11[-target_index_y11] *= gain_y11

y11 = np.fft.ifft(Y11).real

# === Normalize to int16 ===
def normalize_to_int16(y):
    y = y / np.max(np.abs(y)) if np.max(np.abs(y)) > 0 else y
    return (y * 32767).astype(np.int16)

y1_int = normalize_to_int16(y1)
y2_int = normalize_to_int16(y2)
y3_int = normalize_to_int16(y3)
y4_int = normalize_to_int16(y4)
y5_int = normalize_to_int16(y5)
y6_int = normalize_to_int16(y6)
y7_int = normalize_to_int16(y7)
y8_int = normalize_to_int16(y8)
y9_int = normalize_to_int16(y9)
y10_int = normalize_to_int16(y10)
y11_int = normalize_to_int16(y11)

# === Generate .h content ===
def generate_header(name, data):
    lines = [f"const int16_t {name}[{len(data)}] = {{"]
    for i in range(0, len(data), 8):
        row = ', '.join(f"{v:6d}" for v in data[i:i+8])
        lines.append(f"    {row},")
    lines.append("};\n")
    return '\n'.join(lines)

header_content = (
    "#ifndef SAW_WAVETABLES_H\n"
    "#define SAW_WAVETABLES_H\n\n"
    "#include <stdint.h>\n\n" +
    generate_header("saw1", y1_int) +
    generate_header("saw2", y2_int) +
    generate_header("saw3", y3_int) +
    generate_header("saw4", y4_int) +
    generate_header("saw5", y5_int) +
    generate_header("saw6", y6_int) +
    generate_header("saw7", y7_int) +
    generate_header("saw8", y8_int) +
    generate_header("saw9", y9_int) +
    generate_header("saw10", y10_int) +
    generate_header("saw11", y11_int) +
    "#endif // SAW_WAVETABLES_H\n"
)

with open("Saw_Wavetables.h", "w") as f:
    f.write(header_content)

print("Header file 'Saw_Wavetables.h' was successfully created.")
