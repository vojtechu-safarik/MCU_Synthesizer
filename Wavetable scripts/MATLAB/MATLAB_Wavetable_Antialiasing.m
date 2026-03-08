function wavetable_antialiased_processor()
    inputFilename = 'Square_Wavetables.h';
    outputFilename = 'Square_Wavetables_AA.h'; 

    % WAVETABLE_ANTIALIASED_PROCESSOR
    % Script for batch antialiasing processing of multiple wavetables
    % from one H-file into a new H-file.
    
    % =========================================================================
    % 1. SETUP AND CONSTANTS
    % =========================================================================
    
    % Original constants from the code
    TABLE_SIZE = 256;
    BANDWIDTH_FC = 0.468; 
    WINDOW_WIDTH = 32;   
    JUMP_THRESHOLD = 5000;
    
    processed_tables = struct('name', {}, 'data', {});
    
    % =========================================================================
    % 2. DATA LOADING & EXTRACTION (BATCH)
    % =========================================================================
    
    if ~exist(inputFilename, 'file')
        error('Input file not found: %s', inputFilename);
    end
    
    fprintf('Loading file: %s\n', inputFilename);
    fileText = fileread(inputFilename);
    
    % Regular expression to find all array definitions
    % Searches for: const int16_t (NAME) [size] = {(VALUES)};
    % Tokens: 1=NAME, 2=VALUES inside brackets
    
    array_matches = regexp(fileText, '(?:const\s+int16_t\s+)(\w+)\[\d+\]\s*=\s*\{([\s\S]*?)\};', 'tokens');
    
    if isempty(array_matches)
        warning('No arrays found in the file to process.');
        return;
    end
    
    % =========================================================================
    % 3. PROCESSING LOOP (FOR EACH TABLE)
    % =========================================================================
    
    for idx = 1:length(array_matches)
        
        match = array_matches{idx};
        array_name = match{1};
        data_string = match{2};
        
        fprintf('  - Processing table: %s\n', array_name);
        
        % Extracting numbers from the data string and converting to double
        raw_matches = regexp(data_string, '[-+]?\d+', 'match');
        all_numbers = str2double(raw_matches);
        
        if length(all_numbers) < TABLE_SIZE
             warning('Table "%s" contains too little data. Skipping.', array_name);
             continue;
        end
        % Take exactly TABLE_SIZE samples (the last ones)
        raw_data = all_numbers(end-TABLE_SIZE+1:end); 
        
        % Normalization to the range -1.0 to 1.0 for calculations
        x_raw = double(raw_data) / 32767.0;
        
        % --- Core Antialiasing Logic (FROM ORIGINAL CODE) ---
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
            fprintf('    * Normalization: Gain reduced by %.2f dB (Peak was %.2f)\n', ...
                20*log10(1/max_peak), max_peak);
        end
        
        % Conversion to int16 for export
        output_data_int = round(y_proc * 32767);
        output_data_int(output_data_int > 32767) = 32767;
        output_data_int(output_data_int < -32767) = -32767;
        
        % Saving the result
        processed_tables(end+1).name = array_name;
        processed_tables(end).data = output_data_int;
    end % End of processing a single table
    
    % =========================================================================
    % 4. FILE SAVING (MODIFIED FOR MULTIPLE TABLES)
    % =========================================================================
    
    if ~isempty(processed_tables)
        write_multiple_wavetables_header(outputFilename, processed_tables, inputFilename);
    end
end % End of main function

% =========================================================================
% --- HELPER FUNCTIONS (KEPT FROM ORIGINAL CODE) ---
% =========================================================================

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

% --- New function for writing multiple tables (based on the original write_header_file) ---
function write_multiple_wavetables_header(filename, tables, sourceName)
    fid = fopen(filename, 'w');
    [~, name, ~] = fileparts(filename);
    guard = upper(strcat('CUSTOM_', name, '_H'));
    
    fprintf(fid, '// Generated by MATLAB Antialiasing Script\n');
    fprintf(fid, '// Source: %s\n', sourceName);
    fprintf(fid, '#ifndef %s\n#define %s\n\n', guard, guard);
    fprintf(fid, '#include <stdint.h>\n\n');
    
    % Loop through all processed tables
    for j = 1:length(tables)
        current_table = tables(j);
        data = current_table.data;
        array_name = current_table.name;
        fprintf(fid, 'const int16_t %s[%d] = {\n', array_name, length(data));
        
        for i = 1:length(data)
            fprintf(fid, '%6d', data(i));
            if i < length(data), fprintf(fid, ','); end
            if mod(i, 8) == 0 && i < length(data)
                fprintf(fid, '\n'); 
            elseif i < length(data)
                fprintf(fid, ' '); 
            end
        end
        fprintf(fid, '};\n\n'); % Separator between tables
    end
    
    fprintf(fid, '#endif // %s\n', guard);
    fclose(fid);
    fprintf('Output saved to: %s\n', filename);
end