import numpy as np
import re
import os
from scipy.io import wavfile

# user parameters (change as needed)
header_filename = "Flute.h"
sampleRate = 44100
frequency = 110
length = 10 
output_dir = "."
interp = "linear"

# Basic checks
if not os.path.isfile(header_filename):
    raise FileNotFoundError(f"Header file '{header_filename}' not found.")

# Read header
with open(header_filename, "r") as f:
    content = f.read()

# Robust regex to find const arrays: supports int16_t,uint16_t,int8_t,...
pattern = r"const\s+(?:int16_t|uint16_t|int8_t|uint8_t|int32_t)\s+(\w+)\s*\[\s*\d+\s*\]\s*=\s*\{([^}]+)\}\s*;"
matches = re.findall(pattern, content, flags=re.DOTALL)

if not matches:
    print("No wavetables found in header.")
    raise SystemExit

# Prepare output folder
os.makedirs(output_dir, exist_ok=True)

# Helper: parse block into integer numpy array
def parse_block_to_array(block_text):
    # remove C-style comments
    block_text = re.sub(r'//.*', '', block_text)
    block_text = re.sub(r'/\*.*?\*/', '', block_text, flags=re.DOTALL)
    # split on commas and whitespace
    tokens = [tok.strip() for tok in re.split(r',|\s+', block_text) if tok.strip() != ""]
    nums = []
    for tok in tokens:
        # clean tokens from stray chars (e.g. trailing commas)
        tok_clean = re.sub(r'[^\-0-9]', '', tok)
        if tok_clean == "":
            continue
        nums.append(int(tok_clean))
    return np.array(nums, dtype=np.int64)

# For each wavetable in header: generate WAV
for (name, block) in matches:
    table_int = parse_block_to_array(block)
    if table_int.size == 0:
        print(f"Skipping {name}: empty table.")
        continue

    # normalize table to floats in [-1, 1]
    maxabs = np.max(np.abs(table_int))
    if maxabs == 0:
        table = table_int.astype(np.float64)
    else:
        table = table_int.astype(np.float64) / maxabs

    N = table.size

    # generate phases for output signal (vectorized)
    total_samples = int(sampleRate * length)
    # phase ranges 0..1 per sample: phase = (n * frequency / sampleRate) % 1.0
    n = np.arange(total_samples, dtype=np.float64)
    phase = (n * (frequency / sampleRate)) % 1.0

    # map phase to table index (0..N)
    idx = phase * N
    idx0 = np.floor(idx).astype(np.int64) % N
    frac = idx - np.floor(idx)

    if interp == "nearest":
        idx_nearest = np.rint(idx).astype(np.int64) % N
        out = table[idx_nearest]
    else:
        # linear interpolation between idx0 and idx1
        idx1 = (idx0 + 1) % N
        out = (1.0 - frac) * table[idx0] + frac * table[idx1]

    # scale to int16 and write
    maxint16 = np.iinfo(np.int16).max
    # ensure full dynamic range used, preserve sign
    m = np.max(np.abs(out))
    if m == 0:
        scaled = out.astype(np.int16)
    else:
        scaled = (out / m * maxint16).astype(np.int16)

    fname = f"{name}_{frequency}Hz.wav"
    out_path = os.path.join(output_dir, fname)
    wavfile.write(out_path, sampleRate, scaled)
    print(f"Wrote: {out_path} (table len {N}, samples {total_samples})")

print("Done.")
