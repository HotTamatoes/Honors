name = ['name here'];
lFolder = './lFactors.';
dFolder = './dFactors.';
extension = '.txt';

d = readmatrix(strcat(dFolder, name, extension));
d = sparse(diag(d));
tempmat = readmatrix(strcat(lFolder, name, extension));
i = tempmat(:, 1);
j = tempmat(:, 2);
val = tempmat(:, 3);
l = sparse(i, j, val);

clear lFolder dFolder name extension tempmat i j val
