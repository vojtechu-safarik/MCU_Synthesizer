import numpy as np

# Parameters
sample_count = 256
frequency = 1  # Normalised frequency for one cycle (0-1)
t = np.linspace(0, 1, sample_count, endpoint=False)
phase = (frequency * t) % 1.0

# RNG for reproducibility of "grit"
rng = np.random.default_rng(12345)

# Basic triangle generator: range [-1, 1]
def triangle_wave(phase_array):
    # triangle: 2*abs(2*phase-1)-1 gives range [-1,1], flipped peaks at phase=0
    return 2.0 * np.abs(2.0 * phase_array - 1.0) - 1.0

# Helpers used in grit variants
def add_harmonic_sizzle(y, phase_array, amount=0.2, max_harm=40):
    # add a bank of higher harmonics with decreasing amplitude (1/n)
    # amount controls overall added energy; max_harm controls highest harmonic index
    harmonics = np.zeros_like(y)
    # pick harmonics in a band to create "electric" high-frequency content
    start_h = 5
    for n in range(start_h, start_h + int(max_harm * amount) + 1):
        harmonics += (1.0 / n) * np.sin(2.0 * np.pi * n * phase_array + rng.uniform(0, 2*np.pi))
    # normalise harmonics to safe range and mix
    if np.max(np.abs(harmonics)) > 0:
        harmonics = harmonics / np.max(np.abs(harmonics))
    return y + amount * harmonics

def edge_jitter_phase(phase_array, jitter_amount=0.005, detail=1.0):
    # jitter phase slightly around each sample (small random perturbation)
    # jitter_amount = stdev of jitter in fraction of period
    jitter = rng.normal(loc=0.0, scale=jitter_amount * detail, size=phase_array.shape)
    p = (phase_array + jitter) % 1.0
    return p

def add_filtered_noise(y, amount=0.1, kernel_len=7):
    # add noise smoothed by a simple moving average (lowpass) -> controlled grit texture
    noise = rng.standard_normal(size=y.shape)
    k = np.ones(kernel_len) / kernel_len
    noise_smooth = np.convolve(noise, k, mode='same')
    if np.max(np.abs(noise_smooth)) > 0:
        noise_smooth = noise_smooth / np.max(np.abs(noise_smooth))
    return y + amount * noise_smooth

def bitcrush_amplitude(y, bits=6):
    # quantize amplitude to a small number of steps (signed)
    steps = max(2, 2 ** bits)
    # map -1..1 -> 0..(steps-1), round, map back
    scaled = (y + 1.0) * 0.5 * (steps - 1)
    q = np.round(scaled)
    yq = (q / (steps - 1)) * 2.0 - 1.0
    return yq

def logistic_modulate_phase(phase_array, r=3.9, seed=0.123):
    # produce a deterministic logistic sequence and use it to modulate phase
    N = phase_array.size
    x = np.empty(N)
    x[0] = seed
    for i in range(1, N):
        x[i] = r * x[i-1] * (1.0 - x[i-1])
    # normalize x to small modulation centered at 0
    xm = (x - 0.5)  # in [-0.5..0.5]
    # scale down to keep phase changes small
    return (phase_array + 0.02 * xm) % 1.0

def fold_edges(y, fold_amount=2.5):
    # multiple folding by scaling and reflecting into [-1,1]
    # technique: take value*fold_amount, map to saw in [0,2) then reflect
    z = y * fold_amount
    z = np.mod(z + 1.0, 2.0)  # now in [0,2)
    folded = 2.0 * np.abs(z - 1.0) - 1.0
    return folded

# Build 8 triangle-based wavetables (triangle1..triangle8)
wave_tables = []

# 1) pure triangle
y1 = triangle_wave(phase)
wave_tables.append(y1)

# 2) mild harmonic sizzle (little high-frequency harmonics)
y2 = add_harmonic_sizzle(triangle_wave(phase), phase, amount=0.12, max_harm=20)
wave_tables.append(y2)

# 3) edge jitter (phase jitter -> irregular edges)
p3 = edge_jitter_phase(phase, jitter_amount=0.008, detail=1.0)
y3 = triangle_wave(p3)
y3 = add_filtered_noise(y3, amount=0.05, kernel_len=5)  # subtle smoothing noise
wave_tables.append(y3)

# 4) stronger grit: harmonics + jitter + slight bitcrush
p4 = edge_jitter_phase(phase, jitter_amount=0.015, detail=1.2)
y4 = triangle_wave(p4)
y4 = add_harmonic_sizzle(y4, p4, amount=0.25, max_harm=40)
y4 = bitcrush_amplitude(y4, bits=5)
wave_tables.append(y4)

# 5) chaotic modulation (logistic map) + smoothed noise (electric, less regular)
p5 = logistic_modulate_phase(phase, r=3.88, seed=0.31415)
y5 = triangle_wave(p5)
y5 = add_filtered_noise(y5, amount=0.18, kernel_len=9)
y5 = add_harmonic_sizzle(y5, p5, amount=0.18, max_harm=50)
wave_tables.append(y5)

# 6) heavy bitcrush + sharp folds (very digital, buzzy)
y6 = triangle_wave(phase)
y6 = bitcrush_amplitude(y6, bits=2)
wave_tables.append(y6)

# 7) subtle alternating sharp/soft edges (combination deterministic)
# create alternating jitter pattern by multiplying phase with low-frequency pattern
lf = 0.5 * (1.0 + np.sin(2 * np.pi * (phase * 3.0 + 0.25)))  # slow pattern across cycle
p7 = (phase + 0.006 * (lf - 0.5)) % 1.0
y7 = triangle_wave(p7)
y7 = add_harmonic_sizzle(y7, p7, amount=0.22, max_harm=35)
y7 = add_filtered_noise(y7, amount=0.06, kernel_len=4)
wave_tables.append(y7)

# 8) extreme combination: jitter + many harmonics + fold + noise (aggressively "electrical")
p8 = edge_jitter_phase(phase, jitter_amount=0.022, detail=1.6)
y8 = triangle_wave(p8)
y8 = add_harmonic_sizzle(y8, p8, amount=0.45, max_harm=80)
y8 = fold_edges(y8, fold_amount=4.0)
y8 = add_filtered_noise(y8, amount=0.22, kernel_len=6)
y8 = bitcrush_amplitude(y8, bits=3)
wave_tables.append(y8)

# Ensure everything finite and in reasonable range
wave_tables = [np.nan_to_num(w, nan=0.0, posinf=0.0, neginf=0.0) for w in wave_tables]

# Function to normalise into range int16
def normalize_to_int16(y):
    y = y / np.max(np.abs(y))  # normalise between -1 and 1
    return (y * 32767).astype(np.int16)

# Normalise and convert
wave_tables_int = [normalize_to_int16(y) for y in wave_tables]

# Generate header file (same formatting style as before)
def generate_header(name, data):
    lines = [f"const int16_t {name}[{len(data)}] = {{"]
    for i in range(0, len(data), 8):
        row = ', '.join(f"{v:6d}" for v in data[i:i+8])
        lines.append(f"    {row},")
    lines.append("};\n")
    return '\n'.join(lines)

header_guard = "CUSTOM_TRIANGLE_WAVETABLES_H"
header_content = (
    f"#ifndef {header_guard}\n"
    f"#define {header_guard}\n\n"
    "#include <stdint.h>\n\n"
)

# Add each triangle table as triangle1..triangle8
for idx, data in enumerate(wave_tables_int, start=1):
    header_content += generate_header(f"triangle{idx}", data)

header_content += f"#endif // {header_guard}\n"

# Saving the file
with open("Triangle_Wavetables.h", "w") as f:
    f.write(header_content)

print("File 'Triangle_Wavetables.h' has been successfully created.")
