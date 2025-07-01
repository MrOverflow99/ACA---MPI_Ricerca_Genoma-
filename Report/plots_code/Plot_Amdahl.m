clc
clear
close all

% percentuale parallelizzabile
p = 0.859;

% core
n = 1:100;  

% Legge di Amdahl
S = 1 ./ ((1 - p) + p ./ n);

% Plot
figure;
plot(n, S, '.', 'MarkerSize', 8);
xlabel('Number of cores');
ylabel('Speedup');
title(['Amdahl''s Law Speedup for p = ', num2str(p)]);
xlim([1 max(n)]);
ylim([1 max(S)+0.5]);
