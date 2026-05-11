%% Laboratorní analýza syntezátoru
clear all; clc;

% --- NASTAVENÍ ---
[file, path] = uigetfile('Nahr.01.wav'); % Výběr souboru
[y, fs] = audioread(fullfile(path, file));
y = y(:, 1); % Použijeme pouze levý kanál (mono analýza)

start_sec = 1.5;  % Začátek analýzy (sekundy)
len_sec = 0.1;    % Délka analýzy (sekundy)
k_calib = 1.55;   % TVŮJ KALIBRAČNÍ KOEFICIENT (V / FS)

% Výřez dat
idx_start = round(start_sec * fs) + 1;
idx_end = idx_start + round(len_sec * fs);
sig = y(idx_start:idx_end);

fprintf('--- VÝSLEDKY ANALÝZY: %s ---\n', file);

%% SEKCE 1: Napěťová analýza
% Odkomentuj pro výpočet napětí
V_peak = max(abs(sig)) * k_calib;
V_rms = rms(sig) * k_calib;
fprintf('Špičkové napětí (V_peak): %.4f V\n', V_peak);
fprintf('Efektivní napětí (V_rms):  %.4f V\n', V_rms);


%% SEKCE 2: THD + N (Total Harmonic Distortion + Noise)
% Používá vestavěnou funkci (vyžaduje Signal Processing Toolbox)
% Pokud ho nemáš, lze dopočítat z FFT
[thd_val, power_dist] = thd(sig, fs); 
thd_pct = 100 * (10^(thd_val/20));
fprintf('THD+N: %.4f dB (%.4f %%)\n', thd_val, thd_pct);


%% SEKCE 3: Dynamický rozsah a SNR
% Pro SNR potřebuješ nahrávku "ticha" (zapnutý synťák, ale nehraje)
% Zde počítáme Crest Factor jako indikaci dynamiky aktuálního signálu
crest_factor = 20 * log10(V_peak / V_rms);
fprintf('Crest Factor: %.2f dB\n', crest_factor);


%% SEKCE 4: Vizualizace FFT
% Výpočet a zobrazení spektra
L = length(sig);
Y = fft(sig);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = fs*(0:(L/2))/L;

figure(1);
semilogx(f, 20*log10(P1)); % Logaritmická osa frekvence je pro audio lepší
grid on;
title('Frekvenční spektrum (FFT)');
xlabel('Frekvence [Hz]');
ylabel('Amplituda [dBFS]');
xlim([20 20000]); % Slyšitelné pásmo