clc
clear
close all

% number of cores
num_cores = ["1", "2" , "4", "8"];

% times (seconds) 
times = [4.01, 2.57, 1.73, 1.30];

% plot
figure;
bar(times);
set(gca, 'XTick', 1:numel(num_cores), 'XTickLabel', num_cores);
ylabel('Execution time (s)');
xlabel('Number of cores');
title('Strong Scalability');
grid on;
