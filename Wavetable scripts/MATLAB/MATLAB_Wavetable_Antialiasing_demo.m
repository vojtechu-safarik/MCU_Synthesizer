% The figure uses Czech language intentionally

function wavetable_antialiased_visualizer()
    % =========================================================================
    % 1. SETUP AND CONSTANTS
    % =========================================================================
    inputFilename = 'Square_Wavetable.h';
    outputFilename = 'Square_Wavetable_AA.h';
    
    TABLE_SIZE = 256;
    SAMPLE_RATE_CONTEXT = 44100; % For graph X-axis visualization only (Hz)
    
    % Antialiasing Parameters
    % Ensures that the highest harmonics are attenuated before they "reflect" back as aliases.
    % BANDWIDTH_FC = real_FC (10320 Hz) / Nyquist frequency (22050 Hz)
    BANDWIDTH_FC = 0.468; 
    WINDOW_WIDTH = 32;   % Patch width on each side
    JUMP_THRESHOLD = 5000; % Edge detection threshold
    
    % =========================================================================
    % 2. DATA LOADING
    % =========================================================================
    if ~exist(inputFilename, 'file')
        % Fallback: Generating a test square wave if the file is missing
        fprintf('File not found, generating a test square wave.\n');
        raw_data = [32767*ones(1,128), -32767*ones(1,128)];
    else
        fprintf('Loading file: %s\n', inputFilename);
        fileText = fileread(inputFilename);
        raw_matches = regexp(fileText, '[-+]?\d+', 'match');
        all_numbers = str2double(raw_matches);
        if length(all_numbers) < TABLE_SIZE
             error('File contains too little data.');
        end
        raw_data = all_numbers(end-TABLE_SIZE+1:end);
    end
    
    % Normalization to -1.0 to 1.0 for calculations
    x_raw = double(raw_data) / 32767.0;
    
    % =========================================================================
    % 3. PROCESSING (ANTIALIASING)
    % =========================================================================
    correction_buffer = zeros(1, TABLE_SIZE);
    
    for i = 1:TABLE_SIZE
        idx_curr = i;
        idx_next = mod(i, TABLE_SIZE) + 1;
        
        diff = x_raw(idx_next) - x_raw(idx_curr);
        
        if abs(diff * 32767) > JUMP_THRESHOLD
            % Generating Sinc residual (PolyBLEP alternative)
            residual = generate_sinc_residual(diff, WINDOW_WIDTH, BANDWIDTH_FC);
            
            center_offset = WINDOW_WIDTH + 1;
            for k = 1:length(residual)
                target_idx = mod(i - (center_offset - 1) + (k - 1) - 1, TABLE_SIZE) + 1;
                correction_buffer(target_idx) = correction_buffer(target_idx) + residual(k);
            end
        end
    end
    
    y_proc = x_raw + correction_buffer;
    
    % Normalization (Gain staging)
    max_peak = max(abs(y_proc));
    if max_peak > 1.0
        y_proc = y_proc / max_peak;
        fprintf('Normalization: Gain reduced by %.2f dB (Peak was %.2f)\n', ...
            20*log10(1/max_peak), max_peak);
    end
    
    % Conversion to int16 for export
    output_data_int = round(y_proc * 32767);
    output_data_int(output_data_int > 32767) = 32767;
    output_data_int(output_data_int < -32767) = -32767;
    
% =========================================================================
    % 4. SPECTRAL ANALYSIS (ABSOLUTE MAGNITUDE)
    % =========================================================================
    % FFT of the baseband (0 - 22.05 kHz)
    fft_half_len = TABLE_SIZE/2 + 1;
    
    % --- Absolute magnitude of INPUT (without normalization to max=1) ---
    spec_raw_abs = abs(fft(x_raw));
    spec_raw_abs = spec_raw_abs(1:fft_half_len);
    
    % --- Absolute magnitude of OUTPUT (without normalization to max=1) ---
    spec_proc_abs = abs(fft(y_proc));
    spec_proc_abs = spec_proc_abs(1:fft_half_len);
    
    % --- MIRRORING MAGIC (UNFOLDING) ---
    
    % 1. Baseband (0 - Nyquist)
    part1_raw = spec_raw_abs;
    part1_proc = spec_proc_abs;
    
    % 2. Mirrored image (Nyquist - Fs)
    % Remove the first point (DC) and flip
    part2_raw = flip(spec_raw_abs(2:end));
    part2_proc = flip(spec_proc_abs(2:end));
    
    % 3. Zone (Fs - ...)
    % Here we use only a part so the graph fits into 50 kHz.
    len_zone3 = 50 * TABLE_SIZE / SAMPLE_RATE_CONTEXT; 
    part3_raw = spec_raw_abs(2:round(len_zone3)+1);
    part3_proc = spec_proc_abs(2:round(len_zone3)+1);
    
    % Horizontal concatenation (using ,)
    full_spec_raw = [part1_raw, part2_raw, part3_raw];
    full_spec_proc = [part1_proc, part2_proc, part3_proc];
    
    % Creating X-axis for extended data
    df = (SAMPLE_RATE_CONTEXT / 2) / (fft_half_len - 1);
    total_points = length(full_spec_raw);
    f_axis_ext = (0:total_points-1) * df;
    
    % Setting the Y-axis reference maximum
    % Absolute max (will be either the DC component (index 1) or F0 (index 2))
    Y_MAX_REF = max(full_spec_raw);
    
% =========================================================================
% 5. VISUALIZATION (According to your format with absolute amplitude)
% =========================================================================
fig = figure('Name','Wavetable Spectral Analysis','Color','w','Units','normalized','Position',[0.1 0.1 0.6 0.7]);
% --- TOP LEFT: Original signal ---
subplot(2,2,1);
plot(x_raw, 'r', 'LineWidth', 1.2);
axis tight; ylim([-1.2 1.2]); grid on;
title('Originální časový průběh');
xlabel('Vzorky [-]'); ylabel('Hodnota');
% --- TOP RIGHT: Processed signal ---
subplot(2,2,2);
plot(y_proc, 'b', 'LineWidth', 1.2);
axis tight; ylim([-1.2 1.2]); grid on;
title('Antialiasovaný časový průběh'); % (Sinc Corrected)
xlabel('Vzorky [-]'); ylabel('Hodnota');
% --- BOTTOM LEFT: Extended Spectrum RAW ---
subplot(2,2,3);
stem(f_axis_ext/1000, full_spec_raw, 'r', 'MarkerSize', 2, 'LineWidth', 0.5);
grid on;
xlim([0 20]); % CHANGE: Limiting X-axis to 20 kHz
ylim([0 Y_MAX_REF * 1.05]); % Using absolute maximum
xline(SAMPLE_RATE_CONTEXT/2000, 'k--', 'Nyquist (22.05 kHz)'); % This line will not be visible, but kept for context
title('Spektrum (originál)');
xlabel('Frekvence [kHz]'); ylabel('Hodnota [C_{k}]');
% --- BOTTOM RIGHT: Extended Spectrum ANTIALIASED ---
subplot(2,2,4);
stem(f_axis_ext/1000, full_spec_proc, 'b', 'MarkerSize', 2, 'LineWidth', 0.5);
grid on;
xlim([0 20]); % CHANGE: Limiting X-axis to 20 kHz
ylim([0 Y_MAX_REF * 1.05]); % Using absolute maximum
xline(SAMPLE_RATE_CONTEXT/2000, 'k--', 'Nyquist (22.05 kHz)'); % This line will not be visible, but kept for context
title('Spektrum (antialiasované)');
xlabel('Frekvence [kHz]'); ylabel('Hodnota [C_{k}]');
% Font improvements
set(findall(gcf,'-property','FontSize'),'FontSize', 10);
% Saving
write_header_file(outputFilename, output_data_int, inputFilename);
% =========================================================================
% 6. FILE SAVING
% =========================================================================
write_header_file(outputFilename, output_data_int, inputFilename);
end
% --- HELPER FUNCTIONS ---
function residual = generate_sinc_residual(step_height, half_width, fc)
    len = 2 * half_width + 1;
    residual = zeros(1, len);
    win = blackman(len)'; % Window for smoothing edges
    
    for n = 1:len
        % Shift by 0.5 samples (jump center is between samples)
        t = (n - (half_width + 1)) + 0.5;
        
        % Ideal Band-Limited Step (Sinc Integral)
        val_bl = 0.5 + sinint(pi * t * 2 * fc) / pi;
        
        % Ideal Naive Step
        val_naive = (t > 0);
        
        residual(n) = (val_bl - val_naive) * step_height * win(n);
    end
end
function write_header_file(filename, data, sourceName)
    fid = fopen(filename, 'w');
    [~, name, ~] = fileparts(filename);
    guard = upper(strcat('CUSTOM_', name, '_H'));
    
    fprintf(fid, '// Generated by MATLAB Antialiasing Script\n');
    fprintf(fid, '// Source: %s\n', sourceName);
    fprintf(fid, '#ifndef %s\n#define %s\n\n', guard, guard);
    fprintf(fid, '#include <stdint.h>\n\n');
    fprintf(fid, 'const int16_t %s[%d] = {\n', lower(name), length(data));
    
    for i = 1:length(data)
        fprintf(fid, '%6d', data(i));
        if i < length(data), fprintf(fid, ','); end
        if mod(i, 8) == 0, fprintf(fid, '\n'); else, fprintf(fid, ' '); end
    end
    fprintf(fid, '};\n#endif // %s\n', guard);
    fclose(fid);
    fprintf('Output saved to: %s\n', filename);

    % Save picture
print(gcf,'-dpng','-r300','wavetable_antialiased.png');
end
