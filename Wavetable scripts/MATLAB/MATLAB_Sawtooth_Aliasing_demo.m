% -------------------------------------------------------------------------
% sawtooth_aliasing_clean.m
% Theoretically-based plot of a continuous-time sawtooth, its sampled points,
% the (analytical) harmonic spectrum and the replicas produced by sampling.
% 
% The figure uses Czech language intentionally
% -------------------------------------------------------------------------

clear; close all; clc;

%% Parameters (signal and display)
f0 = 5e3;        % sawtooth fundamental frequency [Hz]
fs = 48e3;       % sampling frequency [Hz]
num_intervals = 50;   % how many sample intervals to show on x-axis
K = 200;         % number of harmonics for the Fourier series (theory)
maxFreqPlot = 50e3;   % frequency axis limit for spectrum plots [Hz]

%% Time vectors
T_display = num_intervals / fs;          % total time to display (s)
oversample_factor = 50;                  % how fine the "continuous" approximation is
Fs_cont = fs * oversample_factor;        % high-rate sampling for time-domain drawing
t_cont = linspace(0, T_display, round(T_display*Fs_cont)); % dense time vector
n_sample = 0:num_intervals;              % sample indices to display (0..50)
t_sample = n_sample / fs;                % sampling instants

%% Construct theoretical sawtooth via Fourier (sine) series
% For a sawtooth that ramps from -1 to +1, the sine-only Fourier series is:
% x(t) = (2/pi) * sum_{k=1..∞} [(-1)^(k+1) * (1/k) * sin(2*pi*k*f0*t)]
% We truncate to K harmonics (K large gives nearly exact waveform).
k = (1:K).';
an_k = (2/pi) * ((-1).^(k+1)) ./ k;   % sine coefficients (real-valued)
% Build continuous-time waveform
x_cont = zeros(size(t_cont));
for idx = 1:K
    x_cont = x_cont + an_k(idx) * sin(2*pi*k(idx)*f0*t_cont);
end
% (x_cont is in range approximately [-1,1]).

% Sampled waveform values
x_samp = zeros(size(t_sample));
for idx = 1:K
    x_samp = x_samp + an_k(idx) * sin(2*pi*k(idx)*f0*t_sample);
end

%% Analytical harmonic spectrum (baseband continuous-time)
% harmonic frequencies and magnitudes (theoretical lines)
harm_freqs = k * f0;                    % k * f0
harm_mags  = abs(an_k);                 % magnitude of each harmonic (positive)

% Prepare top-right data: only harmonics <= maxFreqPlot
idx_plot = find(harm_freqs <= maxFreqPlot);
harm_freqs_plot = harm_freqs(idx_plot);
harm_mags_plot  = harm_mags(idx_plot);

%% Replicated spectrum due to sampling: exact theoretical aliasing copies
% We create lines at all |N*fs ± k*f0|, but WITHOUT summing their magnitudes.
% Every replica must have the same amplitude 2/(pi*k).

rep_freqs = [];
rep_mags  = [];

Nrange = -10:10;  % enough to fill up to 50 kHz

for N = Nrange
    for kk = 1:K
        f1 = N*fs + kk*f0;
        f2 = abs(N*fs - kk*f0);
        
        if f1 > 0 && f1 <= maxFreqPlot
            rep_freqs(end+1) = f1; %#ok<SAGROW>
            rep_mags(end+1)  = harm_mags(kk);
        end
        
        if f2 > 0 && f2 <= maxFreqPlot
            rep_freqs(end+1) = f2; %#ok<SAGROW>
            rep_mags(end+1)  = harm_mags(kk);
        end
    end
end

% Now simply plot these as individual spectral lines (NO aggregation)
[rep_freqs_sorted, idx_sorted] = sort(rep_freqs);
rep_mags_sorted = rep_mags(idx_sorted);

% Many lines will overlap (different N,k produce same frequency). We need to
% aggregate magnitudes of coincident lines (sum their contributions).
% We'll round frequencies to 1 Hz to merge near-equal lines.
freq_round = round(rep_freqs); % 1 Hz resolution
[uniq_freqs, ~, ic] = unique(freq_round);
agg_mags = accumarray(ic(:), rep_mags(:)); % sum magnitudes that fall to same rounded freq

% Keep only unique freqs <= maxFreqPlot
valid_idx = uniq_freqs <= maxFreqPlot;
agg_freqs = uniq_freqs(valid_idx);
agg_mags  = agg_mags(valid_idx);

% Sort for plotting
[agg_freqs, sidx] = sort(agg_freqs);
agg_mags = agg_mags(sidx);

%% Compute long-sampled FFT (to compare discrete sampled spectrum)
% This is just for reference overlay (not strictly necessary for theoretical lines)
Nfft = 65536;
% create long sampled signal (many periods)
t_s_long = (0:Nfft-1)/fs;
x_s_long = zeros(size(t_s_long));
for idx = 1:K
    x_s_long = x_s_long + an_k(idx) * sin(2*pi*k(idx)*f0*t_s_long);
end
X_s_long = abs(fft(x_s_long));
f_axis = (0:Nfft-1) * (fs / Nfft);
f_axis_pos = f_axis(1:Nfft/2);
X_s_pos = X_s_long(1:Nfft/2);
% normalize for plotting comparably
X_s_pos = X_s_pos / max(X_s_pos) * max(harm_mags_plot);

%% Plotting: 2x2 figure
figure('Name','Sawtooth & Aliasing (theoretical)','Color','w','Units','normalized','Position',[0.05 0.05 0.7 0.7]);

% Top-left: continuous-time sawtooth and sampled points
subplot(2,2,1);
plot(t_cont * fs, x_cont, 'b-', 'LineWidth', 1.0); hold on;
%stem(t_sample * fs, x_samp, 'k', 'filled','MarkerSize',4);
xlim([0 num_intervals]);
ylim([-1.1 1.1]);
xlabel('Diskrétní vzorky v čase [-]');
ylabel('Hodnota');
title('Spojitý signál');
grid on;

% Top-right: amplitude spectrum (analytical harmonics of continuous-time sawtooth)
subplot(2,2,2);
% convert to kHz on x-axis for similarity with example
stem(harm_freqs_plot/1e3, harm_mags_plot, 'b', 'LineWidth', 1.0);
xlim([0 maxFreqPlot/1e3]);
ylim([0 max(harm_mags_plot)*1.1]);
xlabel('Frekvence [kHz]');
ylabel('Hodnota');
title('Spektrální koeficienty spojitého signálu |C_k|');
grid on;

% Bottom-left: discrete sampled points (zoomed look)
subplot(2,2,3);
plot(t_sample * fs, x_samp, 'b.', 'MarkerSize',12); hold on;
plot(t_sample * fs, x_samp, 'b--', 'LineWidth', 0.8);
xlim([0 num_intervals]);
ylim([-1.1 1.1]);
xlabel('Diskrétní vzorky v čase [-]');
ylabel('Hodnota');
title('Diskrétní signál (vzorky)');
grid on;

% Bottom-right: replicated spectrum produced by sampling (theoretical lines)
%% Plot corrected replica spectrum
subplot(2,2,4);
stem(rep_freqs_sorted/1e3, rep_mags_sorted, 'b', 'LineWidth', 0.8); hold on;

% limit & labels
xlim([0 maxFreqPlot/1e3]);
ylim([0 max(harm_mags_plot)*1.1]);
xlabel('Frekvence [kHz]');
ylabel('Hodnota');
title('Spektrální koeficienty diskrétního signálu |C_k|');
grid on;

% % Global title
% sgtitle(sprintf('Sawtooth f_0 = %.1f kHz, Sampling f_s = %.1f kHz', f0/1e3, fs/1e3));

% Improve fonts
set(findall(gcf,'-property','FontSize'),'FontSize',11);

% Save picture
print(gcf,'-dpng','-r300','sawtooth_aliasing_figure.png');

% End of script
