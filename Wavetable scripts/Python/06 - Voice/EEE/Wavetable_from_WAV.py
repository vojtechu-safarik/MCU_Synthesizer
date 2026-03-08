#!/usr/bin/env python3
"""
wavetable_from_wav.py - headless verze (bez GUI)

Načte mono WAV, najde periodu (autodetekce nebo uživatelem zadaná),
vytvoří průměrné jednoperiodové vzorky, přepočítá na `sample_count`
a uloží jako C header `const int16_t <array_name>[N] = {...};`.

Kromě headeru vykreslí jednorázově statický plot (celý signál + zvětšený úsek).
"""

import os
import sys
import numpy as np
from scipy.io import wavfile
from scipy.signal import resample
import matplotlib.pyplot as plt

# =========================
# === User parameters ====
# =========================
#wav_path = r"D:\Škola\VUT\Bakalářská práce\Programy\Python\ArbitraryWavetableGen\file.wav"
wav_path = "EEE (sample).wav"

sample_count = 256        # počet vzorků ve výsledné wavetable
freq_override = 82.407   # např. 440.0 nebo None pro autodetekci
center_time = 0.8        # v s; None => střed nahrávky
window_duration = 0.02     # délka širšího úseku v sekundách, kolem center_time
out_header = "EEE.h"
array_name = "EEE_wavetable"
# =========================
# === End user params ====
# =========================

def detect_fundamental_autocorr(x, sr, fmin=20.0, fmax=5000.0):
    x = x - np.mean(x)
    N = len(x)
    if N < 3:
        return None, None
    r = np.correlate(x, x, mode='full')[N-1:]
    # vynechat lag 0 (sebeautokorelace)
    r[0] = 0.0
    min_lag = max(1, int(sr / fmax))
    max_lag = min(N-1, int(sr / fmin) if fmin > 0 else N-1)
    if min_lag >= max_lag:
        return None, None
    search = r[min_lag:max_lag+1]
    lag_rel = np.argmax(search)
    lag = lag_rel + min_lag
    freq = sr / lag if lag > 0 else None
    return freq, lag

def extract_cycles_avg(x, center_sample, period_samples, window_samples):
    half = window_samples // 2
    start = max(0, center_sample - half)
    end = min(len(x), center_sample + half)
    win = x[start:end]
    if len(win) < 2 or period_samples < 1:
        return None
    rel_peak = np.argmax(np.abs(win))
    peak = start + rel_peak
    left = (peak - start) // period_samples
    right = (end - peak - 1) // period_samples
    starts = []
    for k in range(-left, right+1):
        s = peak + k*period_samples
        if s >= start and (s + period_samples) <= end:
            starts.append(int(s))
    cycles = [x[s:s+period_samples] for s in starts]
    if len(cycles) == 0:
        s = center_sample - period_samples//2
        s = max(0, min(len(x)-period_samples, s))
        return x[s:s+period_samples].astype(float)
    arr = np.vstack(cycles)
    return np.mean(arr, axis=0)

def normalize_to_int16(y):
    m = np.max(np.abs(y)) if len(y)>0 else 0.0
    if m == 0:
        return y.astype(np.int16)
    y = y / m
    return (y * 32767).astype(np.int16)

def generate_header(name, data, guard_name=None):
    if guard_name is None:
        guard_name = f"{name.upper()}_H"
    lines = [f"#ifndef {guard_name}",
             f"#define {guard_name}",
             "",
             "#include <stdint.h>",
             ""]
    lines.append(f"const int16_t {name}[{len(data)}] = {{")
    for i in range(0, len(data), 8):
        row = ', '.join(f"{int(v):6d}" for v in data[i:i+8])
        lines.append(f"    {row},")
    lines.append("};")
    lines.append("")
    lines.append(f"#endif // {guard_name}")
    return '\n'.join(lines)

def process(wav_path, sample_count=256, freq_override=None,
            center_time=None, window_duration=1.0, out_header="out.h", array_name="wavetable"):
    if not os.path.exists(wav_path):
        raise FileNotFoundError(f"WAV file nenalezen: {wav_path}")
    sr, data = wavfile.read(wav_path)
    if data.ndim > 1:
        data = data.mean(axis=1).astype(data.dtype)
    # convert to float -1..1
    if data.dtype == np.int16:
        x = data.astype(np.float64) / 32768.0
    else:
        x = data.astype(np.float64)
        mx = np.max(np.abs(x))
        if mx > 0:
            x = x / mx

    total_seconds = len(x) / sr
    if center_time is None:
        center_time = total_seconds / 2.0
    center_time = float(center_time)
    center_sample = int(np.clip(int(center_time * sr), 0, len(x)-1))
    window_samples = max(1, int(round(window_duration * sr)))
    half = window_samples // 2
    wstart = max(0, center_sample - half)
    wend = min(len(x), center_sample + half)
    analysis_segment = x[wstart:wend]

    # detect or use override
    if freq_override is None:
        freq_detected, lag = detect_fundamental_autocorr(analysis_segment, sr)
        if freq_detected is None:
            raise RuntimeError("Detekce frekvence selhala. Zadej freq_override v hlavičce.")
        freq = freq_detected
        period_samples = int(round(lag))
        detected_text = f"{freq:.2f} Hz (lag {period_samples} samples)"
    else:
        freq = float(freq_override)
        period_samples = int(round(sr / freq))
        detected_text = f"{freq:.2f} Hz (user-specified, lag ~{period_samples} samples)"

    cycle = extract_cycles_avg(x, center_sample, period_samples, window_samples)
    if cycle is None:
        raise RuntimeError("Nepodařilo se extrahovat cyklus.")
    if len(cycle) != sample_count:
        cycle_resampled = resample(cycle, sample_count)
    else:
        cycle_resampled = cycle.astype(float)

    cycle_int16 = normalize_to_int16(cycle_resampled)
    header_text = generate_header(array_name, cycle_int16, guard_name=(array_name.upper()+"_H"))
    with open(out_header, "w", encoding="utf-8") as f:
        f.write(header_text)

    print(f"Header uložen: {out_header} ({len(cycle_int16)} samples). Detekce: {detected_text}")
    # return data for plotting
    return {
        "sr": sr, "x": x, "center_time": center_time, "wstart": wstart, "wend": wend,
        "cycle": cycle_resampled, "cycle_int16": cycle_int16, "freq": freq,
        "period_samples": period_samples, "header_text": header_text
    }

def plot_result(info, window_duration, out_fig=None):
    sr = info["sr"]
    x = info["x"]
    center_time = info["center_time"]
    wstart = info["wstart"]
    wend = info["wend"]
    cycle = info["cycle"]
    period_samples = info["period_samples"]

    t = np.arange(len(x)) / sr
    start = max(0.0, center_time - window_duration/2)
    end = min(len(x)/sr, center_time + window_duration/2)

    # top: full signal with red span
    fig, (ax_top, ax_bot) = plt.subplots(2,1, figsize=(10,6), gridspec_kw={"height_ratios":[1,1]})
    ax_top.plot(t, x, color='tab:blue', linewidth=0.6)
    ax_top.set_xlim(0, len(x)/sr)
    ax_top.set_title("Celý signál (modře) + vybraný úsek (červeně)")
    ax_top.set_xlabel("Čas [s]")
    ax_top.set_ylabel("Amp")
    ax_top.axvspan(start, end, color='red', alpha=0.25)
    ax_top.axvline(start, color='black', linestyle='--', linewidth=0.8)
    ax_top.axvline(end, color='black', linestyle='--', linewidth=0.8)

    # bottom: zoomed window and averaged cycle
    t_section = np.arange(wstart, wend) / sr
    section = x[wstart:wend]
    ax_bot.plot(t_section, section, color='k', linewidth=0.6, label='section')
    # draw averaged cycle centered in the section
    if cycle is not None and len(cycle) > 0:
        # place cycle centered at center_time, scale time to period_samples
        period_sec = period_samples / sr if period_samples>0 else 0
        t_cycle = np.linspace(center_time - period_sec/2, center_time + period_sec/2, len(cycle))
        ax_bot.plot(t_cycle, cycle, color='r', linewidth=1.2, label='averaged cycle')
        ax_bot.axvline(center_time, linestyle='--', linewidth=0.8)
        ax_bot.axvline(t_cycle[0], linestyle='--', linewidth=0.7)
        ax_bot.axvline(t_cycle[-1], linestyle='--', linewidth=0.7)

    ax_bot.set_xlim(t_section[0], t_section[-1])
    ax_bot.set_xlabel("Čas [s]")
    ax_bot.set_ylabel("Amp")
    ax_bot.set_title("Vybraný úsek a průměrná perioda")
    ax_bot.legend(loc='upper right')
    plt.tight_layout()
    if out_fig:
        fig.savefig(out_fig)
        print(f"Graf uložen: {out_fig}")
    plt.show()

def main():
    try:
        info = process(wav_path, sample_count=sample_count, freq_override=freq_override,
                       center_time=center_time, window_duration=window_duration,
                       out_header=out_header, array_name=array_name)
        # jednorázové vykreslení - nebude se nic "posouvat"
        plot_result(info, window_duration)
    except Exception as e:
        print("Chyba:", e)
        sys.exit(1)

if __name__ == "__main__":
    main()
