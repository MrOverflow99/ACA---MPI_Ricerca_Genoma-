clc
clear
close all

% hash functions
hash_names = {'All the others','DJB2','AddShift-hash','XOR','better XOR'};

% times (seconds)
collisions_rate = [0.05, 62.7744, 99.5455, 99.9232, 99.8956];

% colours
colors = [1 0 1;    % rosso
          0 1 0;    % verde
          0 0 1;    % blu
          1 0 0;    % magenta
          1 1 0];   % giallo

% sorted times
[collisions_sorted, idx] = sort(collisions_rate, 'descend');
hash_names_sorted = hash_names(idx);
colors_sorted = colors(idx, :);

% plot
figure; 
hold on;

for i = 1:numel(collisions_sorted)
    bar(i, collisions_sorted(i), 'FaceColor', colors_sorted(i,:), 'EdgeColor', 'none');
end

set(gca, 'XTick', 1:numel(collisions_sorted), 'XTickLabel', hash_names_sorted);
ylabel('Collision Rate (%)');
title('Collision Rates');
grid on;
hold off;
