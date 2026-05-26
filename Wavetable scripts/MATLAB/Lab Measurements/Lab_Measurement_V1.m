%% Laboratorní analýza syntezátoru
clear all; clc;

% --- NASTAVENÍ ---
[file, path] = uigetfile('*.wav'); % Výběr souboru
[y, fs] = audioread(fullfile(path, file));
y = y(:, 1); % Použijeme pouze levý kanál (mono)

start_sec = 23;  % Začátek analýzy
len_sec = 0.04;    % Délka analýzy
k_calib = 1.55; % Kalibrační koeficient (V / FS)

% Výřez dat
idx_start = round(start_sec * fs) + 1;
idx_end = idx_start + round(len_sec * fs);
sig = y(idx_start:idx_end);

% Odstranění DC offsetu
sig = detrend(sig, 0); 

fprintf('--- VÝSLEDKY ANALÝZY: %s ---\n', file);

%% SEKCE 1: Napěťová analýza
V_peak = max(abs(sig)) * k_calib;
V_rms = rms(sig) * k_calib;
fprintf('Špičkové napětí (V_peak): %.4f V\n', V_peak);
fprintf('Efektivní napětí (V_rms):  %.4f V\n', V_rms);

%% SEKCE 2: Harmonické zkreslení (Čisté THD)
% Analyzuje pouze harmonické složky (ignoruje šum mezi nimi)
[thd_db, ~] = thd(sig, fs); 
thd_pct = 100 * (10^(thd_db/20));
fprintf('THD (pouze harmonické): %.4f dB (%.4f %%)\n', thd_db, thd_pct);

%% SEKCE 3: Skutečné THD + N (Zkreslení + Šum)
% SINAD měří poměr (Signál) / (Zkreslení + Šum). 
% THD+N je pak převrácená hodnota tohoto poměru v záporných dB.
s_sinad = sinad(sig, fs); 
thdn_db = -s_sinad;
thdn_pct = 100 * (10^(thdn_db/20));
fprintf('THD+N (Zkreslení + Šum): %.4f dB (%.4f %%)\n', thdn_db, thdn_pct);

%% SEKCE 4: Činitel výkyvu (Crest Factor)
crest_factor_lin = V_peak / V_rms;
crest_factor_db = 20 * log10(crest_factor_lin);
fprintf('Činitel výkyvu: %.4f [-] (%.2f dB)\n', crest_factor_lin, crest_factor_db);

%% SEKCE 5: Vizualizace FFT
L = length(sig);
Y = fft(sig);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = fs*(0:(L/2))/L;

figure(1);
semilogx(f, 20*log10(P1)); 
grid on;
xlabel('Frekvence [Hz]');
ylabel('Úroveň spektra [dB]');
title(['FFT Analýza: ', file]);
xlim([20 20000]);