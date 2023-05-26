c%rpm
fig = uifigure;
cg = uigauge(fig);
cg.Limits = [0 20000];
cg.MajorTicks = [0:4000:20000];
cg.MajorTickLabels = {'0','4000','8000','12000','16000','20000'};
cg.MinorTicks = [0:1000:20000];
cg.ScaleColors = {'yellow','red'};
cg.ScaleColorLimits = [12000 16000; 16000 20000];

%