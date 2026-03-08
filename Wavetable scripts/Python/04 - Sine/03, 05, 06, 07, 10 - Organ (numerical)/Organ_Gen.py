import numpy as np

# ----------------------
# Config: měň podle potřeby
# ----------------------
sample_count = 256          # wavetable length
normalize_bits = 16
rng = np.random.default_rng(1234)

# Basic helpers
def normalize_to_int16(y):
    y = y / np.max(np.abs(y))
    return (y * (2**(normalize_bits-1)-1)).astype(np.int16)


def save_header(filename, table_dict):
    lines = []
    guard = filename.upper().replace('.', '_') + "_H"
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}\n")
    lines.append("#include <stdint.h>\n")
    for name, arr in table_dict.items():
        lines.append(f"const int16_t {name}[{len(arr)}] = {{")
        for i in range(0, len(arr), 8):
            row = ', '.join(f"{v:6d}" for v in arr[i:i+8])
            lines.append(f"    {row},")
        lines.append("};\n")
    lines.append(f"#endif // {guard}\n")
    with open(filename, "w") as f:
        f.write('\n'.join(lines))
    print(f"Wrote header: {filename}")

# ----------------------
# Additive generator (drawbar-like)
# ----------------------
def additive_table(harmonics, phases=None, waveshape=None):
    """
    harmonics: list of tuples (multiplier, amplitude)
       multiplier: harmonic multiple (float)
       amplitude: linear amplitude
    phases: optional list of initial phases for each partial
    waveshape: optional function f(x)->x applied at the end (e.g. np.tanh)
    returns single-cycle wavetable float in [-1..1] (not normalized)
    """
    t = np.linspace(0, 1, sample_count, endpoint=False)
    y = np.zeros_like(t)
    if phases is None:
        phases = [0.0] * len(harmonics)
    for (i, (m, a)) in enumerate(harmonics):
        phi = phases[i] if i < len(phases) else 0.0
        y += a * np.sin(2.0 * np.pi * m * t + phi)
    if waveshape is not None:
        # gentle waveshape to simulate mild nonlinearity
        y = waveshape(y)
    return y

# ----------------------
# Leslie-like small detune stack (bake detune into single-cycle table)
# ----------------------
def create_leslie_expanded(harmonics, width=0.002, nvoices=3):
    """
    harmonics: [(multiplier, amp), ...]
    width: fractional detune amount (e.g. 0.002 ~ 0.2%)
    nvoices: odd number preferred (center + pairs). The amplitudes are split.
    returns expanded harmonics list with detuned multipliers and proportioned amplitudes
    """
    expanded = []
    center = nvoices // 2
    for (m, a) in harmonics:
        # create nvoices detuned around m
        for v in range(nvoices):
            # voice index centered at 0
            offset_idx = v - center
            # detune factor: small linear step times offset index, plus a tiny rng variance
            detune = 1.0 + offset_idx * width + rng.normal(0.0, width * 0.25)
            expanded.append((m * detune, a / nvoices))
    return expanded

# ----------------------
# Some useful amplitude envelopes for harmonic series
# ----------------------
def decay_amplitude_from_multipliers(multipliers, power=1.0, rolloff=1.0):
    """
    multipliers: list of harmonic multipliers
    power: exponent applied to multiplier (higher => faster decay)
    rolloff: global scale
    returns list of amplitudes (linear)
    """
    amps = []
    for m in multipliers:
        # avoid divide by zero
        amps.append(rolloff * (1.0 / (abs(m) ** power)))
    # normalize so max is 1 (caller can scale further)
    a = np.array(amps)
    if a.max() > 0:
        a = a / a.max()
    return a.tolist()

# ----------------------
# Instrument-specific generators
# Note: single-cycle wavetables can't recreate full time-dependent behaviour
# (attack, decay, tape flutter etc.) — but we bake characteristic spectral
# features (harmonic content, mild inharmonicity, ensemble/detune, waveshape)
# ----------------------

def create_vox_table(brightness=1.0, nasal=1.1):
    """
    Vox Continental-ish: nasal, strong odd harmonics, transistor-ish
    nasal boosts the 3rd partial; brightness controls high-end
    """
    multipliers = [1, 3, 5, 7, 9, 11, 13]
    base_amps = decay_amplitude_from_multipliers(multipliers, power=1.1, rolloff=brightness)
    # emphasize 3rd and some odd harmonics a bit more
    amps = [a * (nasal if m == 3 else 1.0) for a, m in zip(base_amps, multipliers)]
    # tiny transistor imperfections -> small detune
    harmonics = [(m * (1.0 + rng.normal(0, 0.0012)), a) for m, a in zip(multipliers, amps)]
    phases = [rng.uniform(0, 2*np.pi) for _ in harmonics]
    # no strong waveshaping, keep nasal character
    return additive_table(harmonics, phases=phases)


def create_farfisa_table(bite=1.0):
    """Already had a Farfisa model; keep and refine for 'thin' combo organ sound."""
    multipliers = [1, 2, 3, 4, 5, 6, 8, 10, 12]
    amps = decay_amplitude_from_multipliers(multipliers, power=0.8, rolloff=bite)
    amps = [a * (1.4 if m in (3,5,8) else 1.0) for a, m in zip(amps, multipliers)]
    # slightly more phase alignment -> 'honky' attack
    phases = [0.0 if i % 2 == 0 else rng.uniform(0, 2*np.pi) for i in range(len(multipliers))]
    harmonics = list(zip(multipliers, amps))
    return additive_table(harmonics, phases=phases)


def create_rhodes_table(bell_factor=1.2, metallic=0.6, ensemble_width=0.002, ensemble_voices=3):
    """
    Rhodes-ish: bell-like attack (strong fundamental + even harmonics), metallic overtones from tine, plus mild ensemble
    bell_factor: emphasizes bell-like partials (slower decay)
    metallic: amount of inharmonic higher partials
    ensemble: widen partials with small detune
    """
    base_multipliers = [1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14]
    amps = decay_amplitude_from_multipliers(base_multipliers, power=0.9, rolloff=bell_factor)
    # boost even harmonics relative to odd for that Rhodes warmth
    amps = [a * (1.4 if m % 2 == 0 else 1.0) for a, m in zip(amps, base_multipliers)]
    # add some metallic inharmonic partials (slightly shifted higher) as tiny extra tones
    harmonics = []
    for m, a in zip(base_multipliers, amps):
        # core harmonic
        harmonics.append((m, a * (1.0 - metallic*0.3)))
        # metallic shoulder
        harmonics.append((m * (1.0 + metallic * 0.01 + rng.normal(0, metallic*0.002)), a * (metallic*0.3)))
    # ensemble (baked chorus)
    harmonics = create_leslie_expanded(harmonics, width=ensemble_width, nvoices=ensemble_voices)
    phases = [rng.uniform(0, 2*np.pi) for _ in harmonics]
    # mild soft-saturation to emulate amp/overdrive
    return additive_table(harmonics, phases=phases, waveshape=lambda x: np.tanh(x * 0.9))


def create_mellotron_table(tape_noise=0.04, flutter_depth=0.0015, chorus_voices=3):
    """
    Mellotron-ish approximation: since Mellotron is sample/tape-based, we approximate by
    - relatively simple harmonic base (saw-like)
    - layered detuned copies for tape-ensemble
    - added band-limited noise (tape hiss)
    - slight inharmonicity + static 'flutter' baked as detuned partials

    This is an approximation; best realism comes from sampled loops or multi-frame wavetables.
    """
    multipliers = list(range(1, 30))
    amps = decay_amplitude_from_multipliers(multipliers, power=0.95, rolloff=1.0)
    # a mellotron tape has a slight midrange emphasis depending on instrument; boost mids
    amps = [a * (1.0 + 0.6*np.exp(-(i-4)**2/18)) for i,a in enumerate(amps)]
    harmonics = []
    for i, (m, a) in enumerate(zip(multipliers, amps)):
        # small inharmonic shift and a flutter component
        inh = 1.0 + rng.normal(0, 0.0008) + (flutter_depth * np.sin(i * 1.37))
        harmonics.append((m * inh, a))
    # bake a little ensemble / detune to simulate tape-head/motor differences
    harmonics = create_leslie_expanded(harmonics, width=0.0025, nvoices=chorus_voices)
    phases = [rng.uniform(0, 2*np.pi) for _ in harmonics]
    base = additive_table(harmonics, phases=phases)
    # add soft band-limited noise (tape hiss) mixed into table
    noise = rng.normal(0, 1.0, size=sample_count)
    # low-pass the noise by simple convolution kernel to avoid harshness
    k = 7
    kernel = np.ones(k) / k
    noise = np.convolve(noise, kernel, mode='same')
    y = base + noise * tape_noise
    # gentle waveshape to emulate tape saturation
    y = np.tanh(y * 0.95)
    return y


def create_pipe_organ_table(stops=None):
    """
    Pipe organ principal chorus: kept from original but refined slightly
    """
    multipliers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16]
    if stops is None:
        amps = decay_amplitude_from_multipliers(multipliers, power=0.9, rolloff=1.0)
        amps = [a * (1.2 if m == 1 else 1.0) for a, m in zip(amps, multipliers)]
    else:
        amps = [stops.get(m, 0.0) for m in multipliers]
    # slight random phase to avoid pure combing
    phases = [rng.uniform(0, 2*np.pi) for _ in multipliers]
    harmonics = list(zip(multipliers, amps))
    return additive_table(harmonics, phases=phases)

# ----------------------
# Build tables and save
# ----------------------
tables = {}

# Vox Continental
tables['vox_continental'] = normalize_to_int16(create_vox_table(brightness=1.0, nasal=1.2))
# Farfisa
tables['farfisa_combo'] = normalize_to_int16(create_farfisa_table(bite=1.2))
# Rhodes
tables['rhodes_tine'] = normalize_to_int16(create_rhodes_table(bell_factor=1.3, metallic=0.55, ensemble_width=0.0025, ensemble_voices=3))
# Mellotron (approximation)
tables['mellotron_approx'] = normalize_to_int16(create_mellotron_table(tape_noise=0.035, flutter_depth=0.0012, chorus_voices=3))
# Pipe organ (principal)
tables['pipe_organ_principal'] = normalize_to_int16(create_pipe_organ_table())

# Save header file (Organ_Wavetables.h)
save_header("Organ_Wavetables.h", tables)

print("Done generating organ & keys wavetables.")
