import numpy as np
import matplotlib.pyplot as plt
import re

# Route to .h file
filename = "Triangle_Wavetables.h"

# Load the content of the file
with open(filename, "r") as f:
    content = f.read()

# Find all the data in format: waveformN[...] = { ... };
matches = re.findall(r"const\s+int16_t\s+(\w+)\s*\[\d+\]\s*=\s*\{([^}]+)\};", content)

if not matches:
    print("No waveforms have been found.")
else:
    plt.figure(figsize=(10, 5))

    for i, (name, values) in enumerate(matches):
        # Convert text values to int
        numbers = np.array([int(x.strip()) for x in values.split(',') if x.strip()])
        t = np.linspace(0, 1, len(numbers), endpoint=False)
        
        # Plot
        plt.plot(t, numbers, label=name)

    plt.title("Waveforms from .h file")
    plt.xlabel("Normalized time [period]")
    plt.ylabel("Amplitude (int16_t)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()
