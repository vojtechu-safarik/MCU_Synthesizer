%% Generování grafů
clear; close all; clc;

% --- DATA ---
f = [10.301, 20.602, 30.868, 51.913, 69.296, 97.999, 195.998, 293.665, ...
     493.883, 698.456, 987.767, 1975.533, 2959.955, 4978.032, 7040, 9956.063, 11839.82];

data_y = {
    [-16.92, -23.84, -25.72, -26.28, -34.21, -34.22, -34.24, -34.35, -34.67, -35.12, -36.06, -40.07, -42.81, -32.98, -47.09, -68.28, -73.35], ... 
    [-16.77, -19.91, -25.68, -26.24, -34.03, -34.07, -34.07, -34.19, -34.46, -34.90, -35.79, -39.31, -41.10, -32.43, -38.66, -35.43, -30.5], ...  
    [5.47, 3.56, 3.27, 3.35, 3.04, 3.13, 2.94, 3.16, 3.40, 3.39, 3.46, 3.66, 3.84, 4.08, 3.81, 3.66, 4.11] 
};

titles_y = {'THD  [dB]', 'THD+N  [dB]', 'k  [dB]'};
filenames = {'Graf_THD.pdf', 'Graf_THDN.pdf', 'Graf_k.pdf'};
colors = {[0 0.447 0.741], [0.85 0.325 0.098], [0.466 0.674 0.188]}; 

for i = 1:3
    y_val = data_y{i};
    
    % Trend (Aproximace polynomem)
    p = polyfit(log10(f), y_val, 7);
    x_fine = logspace(log10(min(f)), log10(max(f)), 600);
    y_trend = polyval(p, log10(x_fine));

    fig = figure('Units', 'centimeters', 'Position', [2, 2, 36, 22]);
    hold on; box on;
    
    % --- FORMÁTOVÁNÍ MŘÍŽKY ---
    ax = gca;
    ax.XScale = 'log';
    ax.FontName = 'Arial Narrow'; 
    ax.FontSize = 24;
    ax.LineWidth = 1.2; % Silnější rámeček grafu

    % Hlavní mřížka (Major)
    ax.XGrid = 'on';
    ax.YGrid = 'on';
    ax.GridLineStyle = '-';      % Spojitá čára
    ax.GridColor = [0.55, 0.55, 0.55];
    ax.GridAlpha = 0.5;          % Větší viditelnost

    % Vedlejší mřížka (Minor)
    ax.XMinorGrid = 'on';
    ax.YMinorGrid = 'on';
    ax.MinorGridLineStyle = '--'; % Vedlejší čárkovaně pro kontrast
    ax.MinorGridColor = [0.75, 0.75, 0.75];
    ax.MinorGridAlpha = 0.4;

    % Popisky a osy
    xlabel('f  [Hz]', 'FontWeight', 'bold');
    ylabel(titles_y{i}, 'FontWeight', 'bold');
    xlim([10, 20000]);
    xticks([10, 100, 1000, 10000]);
    xticklabels({'10', '100', '1 000', '10 000'});
    ylim([min(y_val)-5, max(y_val)+5]);

    % --- VYKRESLENÍ ---
    % Spojnice trendu
    plot(x_fine, y_trend, 'Color', colors{i}, 'LineWidth', 2);
    % Naměřené body (Křížky)
    plot(f, y_val, '+', 'Color', colors{i}, 'MarkerSize', 20, 'LineWidth', 2);

    % Export
    exportgraphics(fig, filenames{i}, 'ContentType', 'vector');
    close(fig);
end

fprintf('Grafy s výraznou mřížkou byly vyexportovány.\n');