function [] = precompute_weights_2d ( file,M,Z )

input=load(file);

kx = input(1:M/Z,1);
ky = input(1:M/Z,2);

kxy=[kx ky];

% compute the voronoi regions
[V,C] = voronoin(kxy,{'QJ'});

% the surface of the knots is written to area
area = [];

% sum of all surfaces
sum_area = 0;

% the maximum distance two nearest neighbour have
% to get the surface we store max_distance^2
max_distance=0;

% compute the surface of the knots 
for j= 1:length(kxy)
  x = V(C{j},1);
  y = V(C{j},2);
  lxy = length(x);
  if(lxy==0) % a knot exists more than one time
    A=0;
  else
    A = abs(sum(0.5*(x([2:lxy 1])-x(:)).* ...
        (y([2:lxy 1]) + y(:))));
  end
  area = [area A];
  min_distance = min((2*(x-kxy(j,1))).^2+(2*(y-kxy(j,2))).^2);
  max_distance = max([max_distance min_distance]);
end

% if the surface of a knot is bigger than max_distance^2
% or isnan or isinf, then take max_distance^2
for j=1:length(area),
  if(isnan(area(j)) | isinf(area(j))| area(j)>max_distance),
    area(j)=max_distance;
  end
  sum_area = sum_area + area(j);
end

% norm the weights
area = area / sum_area;


% stack the weights for every slice
out=[];
for z=0:Z-1,
  out=[out area];
end

save weights.dat -ascii out


