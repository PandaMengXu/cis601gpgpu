function [y] = draw_boxplot_cpu(rootPath)


fontsize = 20;
fontname = 'Arial';
linewidth = 2;
headerLinesIn = 1;
% 
cpuOrCache = 'cpu';
programParam = '100_10';
%cpuOrCache = 'cache';
%programParam = '12288_100_30000';

boxplot_data = [];
boxplot_group = [];
j = 1;
for i = 1:1:10
    if i == 1
        data_filename = sprintf('%s/%s_alone_d1v1_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'alone-d1v1';
    elseif i == 2
        data_filename = sprintf('%s/%s_together_d1v1_d2v1_nopin_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'd1v1d2v1-nopin';
    elseif i == 3
        data_filename = sprintf('%s/%s_together_d1v1_d2v1_pinsamecore_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'd1v1d2v1-pinsamecore';
    elseif i == 4
        if strcmp(cpuOrCache, 'cpu')
            continue;
        else
            data_filename = sprintf('%s/%s_together_d1v1_d2v1_pindiffcore_%s.data', rootPath, cpuOrCache, programParam);
            boxplot_label = 'd1v1d2v1-pindiffcore';
        end
    elseif i == 5
        data_filename = sprintf('%s/%s_together_d1v5_d2v1_nopin_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'd1v5d2v1-nopin';
    elseif i == 6
        data_filename = sprintf('%s/HT_%s_alone_d1v1_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'HT-alone-d1v1';
    elseif i == 7
        data_filename = sprintf('%s/HT_%s_together_d1v1_d2v1_nopin_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'HT-d1v1d2v1-nopin';
    elseif i == 8
        data_filename = sprintf('%s/HT_%s_together_d1v1_d2v1_pinsamecore_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'HT-d1v1d2v1-pinsamecore';
    elseif i == 9
        data_filename = sprintf('%s/HT_%s_together_d1v5_d2v1_nopin_%s.data', rootPath, cpuOrCache, programParam);
        boxplot_label = 'HT-d1v5d2v1-nopin';
    elseif i == 10
        if strcmp(cpuOrCache, 'cpu')
            continue;
        else
            data_filename = sprintf('%s/HT_%s_together_d1v1_d2v1_pindiffcore_%s.data', rootPath, cpuOrCache, programParam);
            boxplot_label = 'HT-d1v1d2v1-pindiffcore';
        end
    end
    
    data = load(data_filename);
    
    if strcmp(cpuOrCache, 'cpu')
        boxplot_data = [boxplot_data; data(:,3)];
        boxplot_group = [boxplot_group; ones(numel(data(:,3)),1)*i];
    elseif strcmp(cpuOrCache, 'cache')
        boxplot_data = [boxplot_data; data(:,2)];
        boxplot_group = [boxplot_group; ones(numel(data(:,2)),1)*i];
    else
        display('ERR:cpuOrCache has to be cpu or cache!');
    end
    
    boxplot_labels{j} = boxplot_label;
    j = j + 1;
end

fig = figure;
boxplot(boxplot_data, boxplot_group, 'Labels', boxplot_labels, 'labelorientation','inline');

title_name = sprintf('Decision with %s resource', cpuOrCache);
title(title_name);
f_pdf = sprintf('%s/boxplot_%s_v2', rootPath, cpuOrCache);
saveas(fig,f_pdf,'pdf');
%set(gca,'XTickLabel',{' '})
%set(gca,'XTickLabel',boxplot_labels);
%set(gca,'XTickLabel',{'a','b','c','d'});
