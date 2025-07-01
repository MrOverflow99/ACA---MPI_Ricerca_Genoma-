clc
clear
close all

% hash functions
hash_names = {'FNV-1a 64Bit','MurmurHash2','PolyHash','DJB2','AddShift-hash'};

% times (seconds)
times = [28.68, 26.89,  49.74, 25.08, 27.92];

% colours
colors = [1 0 0;    % rosso
          0 1 0;    % verde
          0 0 1;    % blu
          1 0 1;    % magenta
          1 1 0];   % giallo

% sorted times
[times_sorted, idx] = sort(times, 'descend');
hash_names_sorted = hash_names(idx);
colors_sorted = colors(idx, :);

% plot
figure; 
hold on;

for i = 1:numel(times_sorted)
    bar(i, times_sorted(i), 'FaceColor', colors_sorted(i,:), 'EdgeColor', 'none');
end

set(gca, 'XTick', 1:numel(times_sorted), 'XTickLabel', hash_names_sorted);
ylabel('Execution time (s)');
title('Light-cluster multiregion 4 Core');
grid on;
hold off;
